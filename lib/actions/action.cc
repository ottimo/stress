#include <action.h>
#include <composite.h>
#include <leaf.h>
#include <bin-leaf.h>
#include <dec-leaf.h>
#include <hex-leaf.h>
#include <string-leaf.h>
#include <ok-state.h>
#include <fail-state.h>

Action::Action() : my_action("Action"){

};

Action::~Action(){
	my_action.erase();
};

std::vector<State*> Action::runSonsAction(Composite* c){
	std::vector<Composite*>::iterator sons_iterator;
	std::vector<State*> results;
	std::vector<Composite*>& sons = c->getSons();
	for(sons_iterator = sons.begin(); sons_iterator != sons.end(); sons_iterator++){
		results.push_back((*sons_iterator)->runTest());
	}
	return results;
}; 

State* Action::runAction(Leaf* l){
	return (State*) new FailState(l->getId());
};
State* Action::runAction(HexLeaf* l){
	return (State*) new FailState(l->getId());
};
State* Action::runAction(DecLeaf* l){
	return (State*) new FailState(l->getId());
};
State* Action::runAction(BinLeaf* l){
	return (State*) new FailState(l->getId());
};
State* Action::runAction(StringLeaf* l){
	return (State*) new FailState(l->getId());
};

/*!

\func std::vector<Composite*> Action::getDataSons(Composite* c)
\brief Return the vector of Composite's children with information of frame data

This method will be overwrittend inside that action will use some child for its
functionalities and that are not involved when autoinjection system are active!

 */
std::vector<Composite**> Action::getDataSons(Composite* c){
	std::vector<Composite**> datasons;
	std::vector<Composite*>::iterator it;
	for(it =	c->getSons().begin(); it != c->getSons().end(); it++)
		datasons.push_back( &(*it) );
	return datasons;
};

bool Action::thereIsAFail(std::vector<State*>& vect){
	std::vector<State*>::iterator it;
	for(it = vect.begin(); it != vect.end(); it++){
		if( (*it)->isFailValue() )
			return true;
	}
	return false;
};
