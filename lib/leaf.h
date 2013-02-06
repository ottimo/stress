/*
 *  leaf.h
 *  ABNFParser
 *
 *  Created by Evelina Agostini on 18/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LEAF_H
#define _LEAF_H


#include <composite.h>
#include <string>

class Leaf : public Composite{
 
public:
	
	Leaf();
	Leaf(std::string s);
	Leaf(const Leaf& c);

//	virtual Composite* clone(){return new Leaf(*this);};
	
	virtual bool isLeaf(){ return true;};
	
	virtual State* runTest()=0;
	
private:

};


#endif
