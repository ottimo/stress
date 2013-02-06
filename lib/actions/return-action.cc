#include <return-action.h>
#include <ok-state.h>
#include <fail-state.h>
#include <value-hex-state.h>
#include <value-int-state.h>
#include <value-bin-state.h>
#include <value-string-state.h>
#include <composite.h>

ReturnAction::ReturnAction(){
	my_action = std::string("ReturnAction");
};

ReturnAction::~ReturnAction(){};

std::vector<State*> ReturnAction::runSonsAction(Composite* c){
	std::vector<Composite*>::iterator sons_iterator;
	std::vector<State*> results;
	std::vector<Composite*>& sons = c->getSons();
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
            State* res = (*sons_iterator)->runTest();
            results.push_back( res );
            if(res->isFailValue())
                break; // it's orrible, I know
	}
	return results;
};

State* ReturnAction::runAction(Composite* c){
	//TODO in caso di nodo terminale riconoscimento nome e valore da inserire
	//nello state di ritorno
	//OkState* ret = new OkState(c->getId());
	//ret->setStringValue(c->getName());
	State* ret=NULL;
	State* last = NULL;
	std::vector<State*> result = runSonsAction (c);
	last = *(result.end()-1);
	if( !last->isFailValue() )
		ret = new OkState(c->getId());
	else
		ret = new FailState(c->getId());
	ret->setStringValue(c->getName());
	*ret << result;
	
	return (State*)ret;
};

State* ReturnAction::runAction (Leaf* c){
  	OkState* ret = new OkState(c->getId());
	ret->setName( c->getName());
	return (State*)ret;
};

State* ReturnAction::runAction (HexLeaf* l){
	ValueHexState* ret = new ValueHexState(l->getId());
	//ret->setStringValue( l->getName());
	ret->setAbnfHex( l->getName());
	//std::cout << l->getName() << " -> " << ret->getValueForLog()  << " : "<< ret->getStringToSend() << std::endl;
	return (State*)ret;
}; 
State* ReturnAction::runAction (DecLeaf* l){
	ValueIntState* ret = new ValueIntState(l->getId());
	//ret->setStringValue( l->getName());
	ret->setAbnfInt( l->getName());
	//std::cout << l->getName() << " -> " << ret->getValueForLog()  << " : "<< ret->getStringToSend() << std::endl;
	return (State*)ret;
}; 
State* ReturnAction::runAction (BinLeaf* l){
	ValueBinState* ret = new ValueBinState(l->getId());
	ret->setStringValue( l->getName());
	return (State*)ret;
}; 
State* ReturnAction::runAction (StringLeaf* l){
	ValueStringState* ret = new ValueStringState(l->getId());
	//ret->setStringValue( l->getName());
	ret->setAbnfString( l->getName());
	//std::cout << l->getName() << " -> " << ret->getValueForLog()  << " : "<< ret->getStringToSend() << std::endl;
	return (State*)ret;
}; 
