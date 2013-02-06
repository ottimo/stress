#include <iostream>
#include <driver.h>
#include <composite.h>
#include <leaf.h>
#include <ABNFParser.h>

int not_main (int argc, char *argv[])
{
	
	if (argc>1)
	{
		
		if (argc==4)
		{
			ABNFParser parser(argv[1],argv[2],argv[3]);
			if (parser.parse())
			{	parser.saveTree();
				parser.saveResult();
				std::cout << std::endl << "Parsing succeded" << std::endl;
			}
			else 
				std::cout << std::endl << "Parsing failed" << std::endl;
		}
		else
		if (argc==3)
		{
			ABNFParser parser(argv[1],argv[2]);
			if (parser.parse())
			{	
				parser.saveResult();
				std::cout << std::endl << "Parsing succeded" << std::endl;
			}
			else 
				std::cout << std::endl << "Parsing failed" << std::endl;
		}
		else
		if (argc==2)
		{
			ABNFParser parser(argv[1]);
			if (parser.parse())
				std::cout << std::endl << "Parsing succeded" << std::endl;
			else 
				std::cout << std::endl << "Parsing failed" << std::endl;
		}
		
	}
	
	//std::cout << "all ok" << std::endl;
	return 0;
}


ABNFParser::ABNFParser(std::string in, std::string out, std::string out2) : 
				inputFile(in),stateTreeFileName(out),abnfTreeFileName(out2),
				driver(this),root(),parsingResult(false) {

};

ABNFParser::ABNFParser(std::string in,std::string out) : 
				inputFile(in),stateTreeFileName(out),abnfTreeFileName(""),
				driver(this),root(),parsingResult(false){

};

ABNFParser::ABNFParser(std::string in) : 
				inputFile(in),stateTreeFileName(""),abnfTreeFileName(""),
				driver(this),root(),parsingResult(false){

};

ABNFParser::~ABNFParser(){
}

void ABNFParser::saveResult(){
	if (stateTreeFileName!="" && root!=NULL)
	{
		ostringstream abnfTreeString; 	

		State* state = root->runTest();
		state->printTree(&abnfTreeString);
			
		ofstream myfile (stateTreeFileName.c_str());
		if (myfile.is_open())
		{
			myfile << "digraph{" << endl;
			myfile << "graph [ overlap=false ]" << endl;
			myfile << abnfTreeString.str();
			myfile << "}" << endl;
			myfile.close();
		}
		else error(WARNING,"Unable to open file " + stateTreeFileName );
	}
}



void ABNFParser::saveTree(){
	if (abnfTreeFileName!="" && root!=NULL)
	{
		ostringstream abnfTreeString; 	

		root->printTree(&abnfTreeString);

		ofstream myfile (abnfTreeFileName.c_str());
		if (myfile.is_open())
		{
			myfile << "digraph{" << endl;
			myfile << "graph [ overlap=false ]" << endl;
			myfile << abnfTreeString.str();
			myfile << "}" << endl;
			myfile.close();
		}
		else error(WARNING,"Unable to open file " + abnfTreeFileName );
	}
}


bool ABNFParser::parse(){
	root=driver.parse (inputFile);
	return parsingResult;
}


void ABNFParser::error(int type, const yy::location& l, const std::string& m,  const std::string& ch){

	if (type==WARNING)
	std::cerr << "WARNING: ";
	if (type==ERROR)
	{
		parsingResult=false;
		std::cerr << "ERROR: ";
	}
	
	std::cerr << l << ": " << m << " "<< ch << std::endl;
}

void ABNFParser::error(int type, const yy::location& l, const std::string& m){
	if (type==WARNING)
		std::cerr << "WARNING: ";
	if (type==ERROR)
	{
		parsingResult=false;
		std::cerr << "ERROR: ";
	}
	std::cerr << l << ": " << m << std::endl;
}

void ABNFParser::error(int type, const std::string& m){
	if (type==WARNING)
		std::cerr << "WARNING: ";
	if (type==ERROR)
	{
			parsingResult=false;
		std::cerr << "ERROR: ";
	}
	std::cerr << m << std::endl;
}

Composite* ABNFParser::getTree(){
	return root;
};
