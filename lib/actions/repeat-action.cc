#include <repeat-action.h>
#include <ok-state.h>
#include <composite.h>
#include <sstream>

RepeatAction::RepeatAction(){
	my_action = std::string("RepeatAction");
};

RepeatAction::~RepeatAction(){};

State* RepeatAction::runAction(Composite* c){
	State* ret = 0;
	std::vector<State*> results;
	std::string cname =	c->getName();
	int min, max = 0;
	std::stringstream ss;
	ss << cname;
	ss >> max;
	std::cout << "Repeat "<< max << std::endl;
	for (min = 0; min < max; min++){
		ret = ReturnAction::runAction(c);
		results.push_back( ret);
	}
	OkState* okret = new OkState (c->getId ());
	okret->setStringValue(c->getName());
	*okret << results;
	return (State*)okret;	
};
