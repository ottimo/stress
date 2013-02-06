#include <log.h>
#include <iostream>
#include <string>
#include <fstream>
#include <xml-output.h>
#include <txt-output.h>
#include <dot-output.h>
#include <configurator.h>

std::auto_ptr<Log> Log::_instance;

Log* Log::getInstance(){
	if(_instance.get() == 0)
		_instance = std::auto_ptr<Log>( new Log() );
	return _instance.get();
};

Log::Log() : 	abnfTreeFileName("/tmp/abnftree.dot"),
stateTreeFileName("/tmp/statetree.dot"){
	std::string type = Configurator::getInstance()->getTypeOutput();
	if(type.compare("dot")==0)
		output = new DotOutput();
	else
        if(type.compare("txt")==0)
            output = new TXTOutput();
	else
            output = new XMLOutput();
		//TODO syslog?
};

Log::~Log(){};

void Log::printTree(State* statetree, uint64_t id, int run){
	/* std::ostringstream abnfTreeString; 	
	 statetree->printTree(&abnfTreeString);
	 printTree(abnfTreeString, stateTreeFileName); */
	output->printTree(statetree, id, run);
}

void Log::printTree(Composite* comptree){
	std::ostringstream abnfTreeString; 	
	comptree->printTree(&abnfTreeString);
	printTree(abnfTreeString, abnfTreeFileName);
}

void Log::printTree(std::ostringstream& abnfTreeString, std::string filename){
	std::ofstream myfile (filename.c_str());
	if (myfile.is_open())
	{
		myfile << "digraph{" << std::endl;
		myfile << "graph [ overlap=false ]" << std::endl;
		myfile << abnfTreeString.str();
		myfile << "}" << std::endl;
		myfile.close();
	}
	else 
		std::cerr<< std::string("Unable to open file ") << abnfTreeFileName<<std::endl;
};

void Log::closeAll(){
	if(output)
		delete output;
};
