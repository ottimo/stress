/*
 *  ParserTop.h
 *  ABNFParser
 *
 *  Created by Evelina Agostini on 13/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PARSERTOP_HH_
# define _PARSERTOP_HH_

#define ERROR 100
#define WARNING 101
#include <location.hh>
class ParserTop{

public:
	
	virtual void error(int type, const yy::location& l, const std::string& m,  const std::string& ch)=0;
	virtual void error(int type, const yy::location& l, const std::string& ch)=0;
	virtual void error(int type, const std::string& m)=0;
	
	virtual void setParsingResult (bool result)=0;
private:
	
	
};

#endif