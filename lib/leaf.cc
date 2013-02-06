/*
 *  leaf.cpp
 *  ABNFParser
 *
 *  Created by Evelina Agostini on 18/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "leaf.h"
#include <action-factory.h>

Leaf::Leaf() : Composite(){
	type = std::string("Leaf");
};
Leaf::Leaf(std::string s) : Composite(s){
	type = std::string("Leaf");
};
Leaf::Leaf(const Leaf& c) : Composite(c){
	type = std::string("Leaf");
};

State* Leaf::runTest(){
	if(strategy == 0)
		getAction();
	State* res = strategy->runAction(this);
	return res;
};
