#include <or-composite.h>
#include <action-factory.h>

OrComposite::OrComposite() : Composite(){
	type = std::string("OrComposite");
};
OrComposite::OrComposite(std::string s) : Composite(s){
	type = std::string("OrComposite");
};
OrComposite::OrComposite(const OrComposite& c) : Composite(c){
	type = std::string("OrComposite");
};

Composite* OrComposite::clone(){return new OrComposite(*this);};

Action* OrComposite::getAction(){
	if(strategy == 0)
		strategy = ActionFactory::getInstance()->getAction(this);
	return strategy;
};
