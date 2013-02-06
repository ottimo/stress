#include <repeat-composite.h>
#include <action-factory.h>

RepeatComposite::RepeatComposite() : Composite(){
	type = std::string("RepeatComposite");
};
RepeatComposite::RepeatComposite(std::string s) : Composite(s){
	type = std::string("RepeatComposite");
};
RepeatComposite::RepeatComposite(const RepeatComposite& c) : Composite(c){
	type = std::string("RepeatComposite");
};

Composite* RepeatComposite::clone(){return new RepeatComposite(*this);};

Action* RepeatComposite::getAction(){
	if(strategy == 0)
		strategy = ActionFactory::getInstance()->getAction(this);
	return strategy;
};