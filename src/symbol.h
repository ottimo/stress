/*
 *  symbol.h
 *  
 *
 *  Created by Evelina Agostini on 07/02/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SYMBOL
# define SYMBOL


#include <iostream>
#include <fstream>

#include <sstream>
#include <vector>
#include <map>
#include <composite.h>
#include <composite-factory.h>
using namespace std;

class Symbol{
public:
	Symbol():name(""),nodes(){};
	
	Symbol(string n): name(n),nodes(){};
	Symbol(string n, Composite* node);
	Symbol(string n,vector<Symbol*> symbols);
	Symbol(string n, Symbol* a, Symbol* b=NULL, Symbol* c=NULL, Symbol* d=NULL, Symbol* e=NULL);
	~Symbol();
	
	
	void addNode(Composite* node){nodes.push_back(node);}
	void addNodes (vector<Composite*> new_nodes);
	bool addTreeLink(Symbol* end);
	
	vector<Composite*> getNodes(){return nodes;}
	string getName(){return name;}
	
	
	void clear();
	
private:
	void addTreeSimpleLink(Composite* start, Composite* end);
	string name;
	vector<Composite*> nodes;
	
};



#endif // not SYMBOL
