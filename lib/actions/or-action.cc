#include <or-action.h>
#include <ok-state.h>
#include <composite.h>

OrAction::OrAction(){
	my_action = std::string("OrAction");
};

OrAction::~OrAction(){};

State* OrAction::runAction(Composite* c){
	//TODO in caso di nodo terminale riconoscimento nome e valore da inserire
	//nello state di ritorno
	OkState* ret = new OkState(c->getId());
	ret->setStringValue(c->getName());
	return (State*)ret;
};
