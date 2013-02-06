/*
 *  symbol.cpp
 *  
 *
 *  Created by Evelina Agostini on 07/02/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <symbol.h>
#include <or-composite.h>

Symbol::Symbol(string n, Composite* node):name(n),nodes(){
	nodes.push_back(node);
};

Symbol::~Symbol(){
	for(vector<Composite*>::iterator it = nodes.begin(); it != nodes.end();it++ ) {
	 	
		delete *it;
	}
	nodes.clear();
}

Symbol::Symbol(string n, vector<Symbol*> symbols) : name(n),nodes(){
	
	for(vector<Symbol*>::iterator it = symbols.begin(); it != symbols.end();it++ ) {
		addNodes((*it)->getNodes());
	}
}



Symbol::Symbol (string n, Symbol* a, Symbol* b, Symbol* c, Symbol* d, Symbol* e) : name(n),nodes()
{
	if (a!=NULL) addNodes(a->getNodes());
	if (b!=NULL) addNodes(b->getNodes());
	if (c!=NULL) addNodes(c->getNodes());
	if (d!=NULL) addNodes(d->getNodes());
	if (e!=NULL) addNodes(e->getNodes());
}

void Symbol::addNodes (vector<Composite*> new_nodes){
	for(vector<Composite*>::iterator it = new_nodes.begin(); it != new_nodes.end();it++ ) {
		nodes.push_back(*it);
	}
	
	
}



void Symbol::addTreeSimpleLink(Composite* start, Composite* end){
	
	*start << end;
	
}


bool Symbol::addTreeLink(Symbol* end){
	
	bool result=true;
	
	Composite* link_start;
	if (!(getNodes().empty())) 
	{
		link_start=getNodes().front();
		
	
		vector<Composite*> vals = end->getNodes();
		for(vector<Composite*>::iterator it2 = vals.begin(); it2 != vals.end();it2++ ) {
			Composite* link_end=*it2;
			
			OrComposite* start_or = dynamic_cast<OrComposite*>(link_start);
			OrComposite* end_or = dynamic_cast<OrComposite*>(link_end);
			if (start_or && end_or )	
			{	
				start_or->addSons(end_or->getSons());// serve per mettere insieme tutti i figli dell'or
			}			
			else
			{
				
				addTreeSimpleLink(link_start,link_end);
			}
			
			
		//	if (link_start->getName()!=link_end->getName()) //FIXME serviva a qualcosa il controllo?
		//		addTreeSimpleLink(link_start,link_end);
		//	else{
		//		result=false;
		//		break;
		//	}
		}	
	}
	return result;
	
}

void Symbol::clear(){
	nodes.clear();
}
