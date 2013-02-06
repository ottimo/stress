#include<configurator.h>
#include<action-factory.h>
#include<udp-send-action.h>
#include<tcp-send-action.h>
#include<udp-read-action.h>
#include<tcp-read-action.h>
#include<return-action.h>
#include<or-action.h>
#include<repeat-action.h>
#include<or-composite.h>
#include<repeat-composite.h>
#include<inject-action.h>
#include<test-case-manager.h>
#include <stdint.h>
#include<ts-generator.h>

ActionFactory* ActionFactory::_instance;

ActionFactory::~ActionFactory(){
	clearCommands();
};

ActionFactory::ActionFactory():next_injection_id(1){};

ActionFactory* ActionFactory::getInstance(){
	if(_instance == NULL)
		_instance = new ActionFactory();
	return _instance;
} 

uint64_t ActionFactory::getNextInjectionId(){
//	uint64_t n = next_injection_id;
//	if(n == UINT64_MAX){
//		//std::cerr << "Maximum number of fault reached! " << next_injection_id << std::endl;
//		return n;
//	}
//	next_injection_id <<=1;
//	return n;

	TSGenerator* ts_generator = Configurator::getInstance()->getTSGenerator();
	uint64_t n = ts_generator->getNextInjectionId(next_injection_id);
	return n;
}

Action* ActionFactory::getAction(RepeatComposite* orcomp){
	return new RepeatAction();
}

Action* ActionFactory::getAction(OrComposite* orcomp){
	InjectAction* inj = new InjectAction(getNextInjectionId());
	//TestCaseManager::getInstance()->setSonsNumber(inj->getInjectionId(), orcomp->getSons().size());
	return inj;
}

Action* ActionFactory::getAction(Leaf* leaf){
	return new ReturnAction();
}

Action* ActionFactory::getAction(Composite* comp){
	Action* action = NULL;
	if(comp->getSons().size() == 0)
		action = new ReturnAction();
	else if(CommandComposite* cc = dynamic_cast<CommandComposite*>(comp->getSons()[0]))
		action = getActionFromFirstSon(cc);
	else  
		action = getActionFromFirstSon(comp->getSons()[0]);
	return action;
}
Action* ActionFactory::getActionFromFirstSon(CommandComposite* ccomp){
	Action* action = NULL;
	std::cout << "Command "<< ccomp->getName() <<" " << commands[ccomp->getName()] << std::endl;
	Command* cmd = commands[ccomp->getName()];
	if(cmd)
		action = cmd->clone();
	else
		action = new ReturnAction();
	if(action == NULL)
		std::cerr << "Action NULL";
	return action;
};

Action* ActionFactory::getActionFromFirstSon(Composite* comp){
	Action* action = NULL;
	action = new ReturnAction();
	return action;
};

void ActionFactory::registerCommand(std::string name, Command* cmd){
	if(!commands[name]){
		std::cout << "ActionFactory registering command: " << name << std::endl;
		commands[name] = cmd;
	}
};

void ActionFactory::clearCommands (){
	std::map<std::string, Command*>::iterator it;
	for(it = commands.begin(); it!= commands.end(); it++){
//		std::cout << "Deleting " << it->second->getName() << std::endl;
		delete (it->second);
	}
	commands.clear();
};
