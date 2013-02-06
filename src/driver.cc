
#include <driver.h>
#include <parser.hh>

#include <symbol.h>
#include <composite-factory.h>
#include <or-composite.h>

#define TOLERANCE 0

ParserDriver::ParserDriver ()  : trace_scanning (false), trace_parsing (false), file(), values(), top(NULL), factory()
{

}

ParserDriver::ParserDriver (ParserTop* t)  : trace_scanning (false), trace_parsing (false), file(), values(), top(t), factory()
{

}

ParserDriver::ParserDriver(const ParserDriver& p) : trace_scanning (p.trace_scanning), trace_parsing (p.trace_parsing), 
	 file(p.file), values(p.values), top(p.top), factory(){}


ParserDriver::~ParserDriver ()
{
	 deleteValueMap();
}

	 Composite*
ParserDriver::parse (const std::string &f)
{
	 CompositeFactory factory;
	 file = f;	
	 Composite* root=NULL;	

	 if (scan_begin()) 
	 {
		  yy::Parser parser (*this, factory,f, &root);
		  parser.set_debug_level (trace_parsing);
		  if (parser.parse ()==0)
		  {
				if (top!=NULL) top->setParsingResult(true);

				std::vector<Composite*> undefinedRules = root->buildTree(&values);

				//printValueMap();
				std::vector<Composite*> unreferencedRules= checkUnreferencedSymbols();

				printUndefinedSymbols(undefinedRules);
				printUnreferencedSymbols(unreferencedRules);

		  }

		  //printValueMap();
	 }
	 scan_end ();
	 return root;
}

	 void
ParserDriver::error (const yy::location& l, const std::string& m,  const std::string& ch)
{
	 if (top!=NULL) top->error(ERROR, l,m,ch);
	 else 
		  std::cerr << l << ": " << m << " "<< ch << std::endl;

}

	 void
ParserDriver::error (const yy::location& l, const std::string& m)
{
	 if (top!=NULL) top->error(ERROR, l,m);
	 else 
		  std::cerr << l << ": " << m << std::endl;

}


	 void
ParserDriver::error (const std::string& m)
{
	 if (top!=NULL) top->error(ERROR, m);
	 else 
		  std::cerr << m << std::endl;

}


void ParserDriver::addValue(Symbol* s){
	 if (!(s->getNodes().empty())) addValue(s->getNodes().front());
}

void ParserDriver::addValue(Composite* value){
	 Branch b(value);
	 values.insert(make_pair(value->getName(),b));

}


vector<Composite*> ParserDriver::checkUnreferencedSymbols(){

	 vector<Composite*> unrSymbols;

	 for(map<string,Branch>::iterator it = values.begin(); it != values.end();it++ ) 
	 {
		  if (!(it->second.getReferenced()))
		  {	
				unrSymbols.push_back(it->second.getRoot());


		  }

	 }		
	 return unrSymbols;
}

void ParserDriver::printUndefinedSymbols(vector<Composite*> undS){

	 vector<Composite*>::iterator it;

	 int type=0;

	 if (TOLERANCE==0) type=ERROR;
	 else type=WARNING;

	 for (it= undS.begin(); it!=undS.end(); it++)
	 {
		  if (top!=NULL)
				top->error(type, "undefined symbol: " + (*it)->getName());
		  else
				std::cerr << "undefined symbol: " << (*it)->getName() << std::endl;

	 }

}

void ParserDriver::printUnreferencedSymbols(vector<Composite*> unrS){

	 vector<Composite*>::iterator it;


	 int type=0;

	 if (TOLERANCE==0) type=ERROR;
	 else type=WARNING;

	 for (it= unrS.begin(); it!=unrS.end(); it++)
	 {
		  if (top!=NULL)

				top->error(type, "unreferenced symbol: " + (*it)->getName());
		  else
				std::cerr << "unreferenced symbol: " << (*it)->getName() << std::endl;

	 }

}

void ParserDriver::printValueMap(){
	 for(map<string,Branch>::iterator it = values.begin(); it != values.end();it++ ) 
	 {
		  std::cout << it->first << ":" << it->second.getRoot()->getId() ;

		  if (it->second.getReferenced()) std::cout << "**";
		  else std::cout << "xx";
		  std::cout << std::endl;

	 }		

}

void ParserDriver::deleteValueMap(){
	 for(map<string,Branch>::iterator it = values.begin(); it != values.end();it++ ) 
	 {
		  //	delete it->second; quindi non serve più? FIXME

	 }		
	 values.clear();
}

Symbol* ParserDriver::addNonTerminalNode(Composite* node,string name, Symbol* sx, Symbol* dx){


	 Symbol* node_symbol=new Symbol(name,node);
	 node_symbol->addTreeLink(sx);
	 if (dx!=NULL)	node_symbol->addTreeLink(dx);

	 return node_symbol;

}

bool ParserDriver::addIncrAlternative(Symbol* rule, Symbol* alternative){

	 Composite* ruleValue;
	 if (!(rule->getNodes().empty())) 
	 {		
		  ruleValue=rule->getNodes().front();


		  map<string,Branch>::iterator it=values.find(ruleValue->getName());

		  if(it!=values.end())
		  {
				Composite* a=it->second.getRoot();
				Composite* sonsOfA=NULL;

				if(a->getSons().size()==1)
				{
					 sonsOfA=a->getSons().front();//FIXME
				}
				else if(a->getSons().size()>1)
				{
					 sonsOfA=factory.getNode(std::string("()"));
					 sonsOfA->setSons(a->getSons());
				}

				Composite* orNode=NULL;
				bool cambiaFiglio=true;
				std::vector<Composite*> sonsOfOr;
				OrComposite* orN = dynamic_cast<OrComposite*>(sonsOfA);//se a ha gia' come figlio un nodo or non creo un nuovo nodo or ma aggiungo l'alternativa in quello già esistente
				if (orN)
				{
					 orNode=orN;
					 cambiaFiglio=false;

				}
				else
				{
					 orNode= factory.getOrNode(std::string("/"));
					 sonsOfOr.push_back(sonsOfA);
				}




				//creo le alternative e le metto in sondOfOr
				if (alternative->getNodes().size()==1)
				{
					 sonsOfOr.push_back(alternative->getNodes().front());
				}
				else if (alternative->getNodes().size()>1)
				{
					 Composite* altGroup=factory.getNode(std::string("()"));
					 altGroup->setSons(alternative->getNodes());
					 sonsOfOr.push_back(altGroup);
				}



				if (cambiaFiglio){
					 orNode->setSons(sonsOfOr);
					 std::vector<Composite*> newSonsOfA;
					 newSonsOfA.push_back(orNode);
					 a->setSons(newSonsOfA);
				}
				else{
					 orNode->addSons(sonsOfOr);
				}

				return true;
		  }
	 }
	 return false;
}
