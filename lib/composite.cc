#include <composite.h>
#include <iostream>
#include <test-control.h>
#include <sstream>
#include <map>
#include <ok-state.h>
#include <action-factory.h>

Composite::Composite() : my_name(""),
sons(),id(0),strategy(0), type("Composite")
{
	
};

Composite::Composite(std::string s) : my_name(s),
sons(),
id(TestControl::getInstance()->getId()),strategy(0), type("Composite")
{
	
};



Composite::Composite(const Composite& c):
my_name(c.my_name),sons(),id(TestControl::getInstance()->getId()) ,strategy(0), type("Composite")
{
	std::vector<Composite*>::const_iterator sons_iterator;
	for(sons_iterator = c.sons.begin(); sons_iterator != c.sons.end(); sons_iterator++){
		//this->sons.push_back(new Composite(**sons_iterator));
		this->sons.push_back((*sons_iterator)->clone());
	}
	
}

Composite::~Composite(){
	std::vector<Composite*>::iterator sons_iterator;
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++) {
		delete *sons_iterator;
	}
	if(strategy)
		delete strategy;
	sons.clear();
};

Composite* Composite::clone(){
	return new Composite (*this);
}

std::vector<Composite*> Composite::cloneSons(){
	std::vector<Composite*> v;
	std::vector<Composite*>::const_iterator sons_iterator;
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
		v.push_back((*sons_iterator)->clone());
	}
	return v;
}

void Composite::printTree(){
	std::vector<Composite*>::iterator sons_iterator;
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
		std::cout << getName() << " -> " << (*sons_iterator)->getName(); 
		std::cout << " ;" << std::endl;
		(*sons_iterator)->printTree();
	}
};

void Composite::printTree(std::ostringstream* out){
	std::vector<Composite*>::iterator sons_iterator;
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
		*out << getId() << "[label=\"" <<  getFullName() << " " << getType() << " ";
		
		*out << "\"];" << std::endl << (*sons_iterator)->getId() << "[label=\"" << (*sons_iterator)->getFullName() << " "<< (*sons_iterator)->getType();
		
		*out << "\"]" << std::endl; 
		
		*out << getId() << "->" << (*sons_iterator)->getId()  << " ;" << std::endl;
		(*sons_iterator)->printTree(out);
	}
};


std::vector<Composite*> Composite::buildTree(std::map<std::string,Branch> *values){
	
	std::vector<Composite*> undefinedRules;
	
	std::map<std::string,Branch>::iterator values_it=values->find(my_name);
	if (values_it!=values->end() )
	{
		setSons(values_it->second.getRoot()->cloneSons());
		values_it->second.setReferenced(true);
	}
	std::vector<Composite*>::iterator sons_iterator;
	
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
		std::vector<Composite*> sonUndefinedRules=(*sons_iterator)->buildTree(values);
		undefinedRules.insert(undefinedRules.end(),sonUndefinedRules.begin(),sonUndefinedRules.end());
	}
	
	if (sons.size()==0 && !(isLeaf()))
		undefinedRules.push_back(this);
	
	return undefinedRules;
	
}

void Composite::addSons(std::vector<Composite*> v){
	std::vector<Composite*>::iterator it=sons.begin();
	this->sons.insert(it,v.begin(),v.end());
}

void Composite::operator<<(Composite* c){
	this->sons.push_back(c);
}

State* Composite::runTest(){
	if(strategy == 0)
		getAction();
	State* res = strategy->runAction(this);
	
	return res;
};

Action* Composite::getAction(){
	if(strategy == 0)
		strategy = ActionFactory::getInstance()->getAction(this);
	
	std::vector<Composite*>::iterator sons_iterator;
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++)
		(*sons_iterator)->getAction();
	return strategy;
};

std::vector<Composite**> Composite::getDataSons(){
	std::vector<Composite**> datasons;
	if(strategy != 0){
		datasons = strategy->getDataSons(this);
	}
	return datasons;
};
