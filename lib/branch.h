/*
 *  branch.h
 *  ABNFParser
 *
 *  Created by Evelina Agostini on 12/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BRANCH_H
#define _BRANCH_H


#include <composite.h>
class Composite;

class Branch
	{
	public:
		Branch():root(), referenced(false){}
		Branch(const Branch& b):root(b.root),referenced(b.referenced){}
		Branch(Composite* r):root(r), referenced(false){}
		
		~Branch(){}
		
		Composite* getRoot(){return root;}
		void setRoot(Composite* r){root=r;}
		
		void setReferenced(bool value){referenced=value;}
		bool getReferenced(){return referenced;}
	private:
		Composite* root;
		bool referenced;
	};


#endif