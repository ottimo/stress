/*
 *  myParser.h
 *  ABNFParser
 *
 *  Created by Evelina Agostini on 29/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _MYPARSER_H
# define _MYPARSER_H

#include <composite.h>
#include <parsertop.h>
#include <driver.h>





class ABNFParser : public ParserTop {
	
public:
	ABNFParser(std::string, std::string, std::string);
	ABNFParser(std::string, std::string);
	ABNFParser(std::string);
	~ABNFParser();
	void saveTree();
	void saveResult();
	bool parse();
	
	void setTraceScanning(bool b){driver.trace_scanning=b;}
	void setTraceParsing(bool b){driver.trace_parsing=b;}
	
	void error(int type, const yy::location& l, const std::string& m,  const std::string& ch);
	void error(int type, const yy::location& l, const std::string& ch);
	void error(int type, const std::string& m);
	void setParsingResult (bool result){parsingResult=result;};
	
	Composite* getTree();

private:

	std::ostringstream abnfTreeString; 	
	std::string  abnfTreeFileName;
	std::string inputFile;

	std::string  stateTreeFileName;

	ParserDriver driver;
	Composite* root;
	bool parsingResult;
	
};

#endif // _MYPARSER_H
