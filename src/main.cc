#include <iostream>
#include <sstream>
#include <configurator.h>
#include <ABNFParser.h>
#include <log.h>
#include <action-factory.h>
#include <test-case-manager.h>
#include "socket-factory.h"

#include <AnomalyInjector.h>

using namespace std;

int main(int argc, char* argv[]){
	bool result_parsing = false;

	Configurator::getInstance()->parseCmdLine(argc, argv);

	ABNFParser* abnfparser = new ABNFParser(Configurator::getInstance()->getAbnfFile());
	result_parsing =	abnfparser->parse();

	Composite* tree = abnfparser->getTree();
	tree->getAction();


	AnomalyInjector* ai = new AnomalyInjector();
	Configurator* conf = Configurator::getInstance();
	ai->configure(conf->getAutoInject());

	if(ai->isActive()){
		std::vector<Composite*> send_actions;
        ai->findSendComposite(tree, send_actions);
        
        std::vector<Composite*>::iterator sendRoot;
		bool injected = true;
		for(sendRoot = send_actions.begin(); sendRoot!= send_actions.end() && injected; sendRoot++){
			injected = ai->inject( (*sendRoot) );
		}
		ai->write(tree);
		send_actions.clear();
	}
	delete ai;

	if(tree)
		LOG->printTree(tree);
	if(!result_parsing)
		exit(1);

	uint64_t number_injector = 1;

	
	for(uint64_t u64 = 0; u64 < number_injector; u64++){
		std::cerr << "EXEC "<< u64 ;
		int num_run = TestCaseManager::getInstance()->getRun(u64);
		for(int i = 0; i< num_run; i++){
			std::cerr << std::endl<<" run " << i << std::endl;
			SocketFactory::getInstance()->reconnectSocket();
			if(Configurator::getInstance()->getMonitor())
				TestCaseManager::getInstance()->sendNotificationStartTestCase();
			State* result = tree->runTest();

			if(Configurator::getInstance()->getMonitor()){
				TestCaseManager::getInstance()->sendNotificationFinishTestCase();
				State* report = TestCaseManager::getInstance()->getReport();
				(*result) << report;
			}else
				 TestCaseManager::getInstance()->waitTimeout();
			TestCaseManager::getInstance()->prepareNextTestCase();

			//LOG->printTree(result);
			LOG->printTree(result, u64, i);
			delete result;
		}
		std::cerr  << std::endl;
		if(number_injector == 1)
			number_injector = ActionFactory::getInstance()->getNextInjectionId();
	}

	TestCaseManager::getInstance()->closeMonitor();

	Log::getInstance()->closeAll();
	SocketFactory::getInstance()->releaseAllSocket();
	ActionFactory::getInstance()->clearCommands ();
	delete tree;

	delete abnfparser;

	return 0;
}
