#include <composite-factory.h>
#include <string-leaf.h>
#include <bin-leaf.h>
#include <dec-leaf.h>
#include <hex-leaf.h>
#include <or-composite.h>
#include <repeat-composite.h>
#include <command-composite.h>

CompositeFactory::CompositeFactory(){};
CompositeFactory::~CompositeFactory(){};

Composite* CompositeFactory::getNode(std::string s){
	return new Composite(s);
};

Composite* CompositeFactory::getOrNode(std::string s){
	return new OrComposite(s);
};

Composite* CompositeFactory::getRepeatNode(std::string s){
	return new RepeatComposite(s);
};

Composite* CompositeFactory::getOptionNode(std::string s){
	return new Composite(s);
};

Composite* CompositeFactory::getStringNode(std::string s){
	return new StringLeaf(s);
};

Composite* CompositeFactory::getBinNode(std::string s){
	return new BinLeaf(s);
};

Composite* CompositeFactory::getDecNode(std::string s){
	return new DecLeaf(s);
};

Composite* CompositeFactory::getHexNode(std::string s){
	return new HexLeaf(s);
};

Composite* CompositeFactory::getCommandNode(std::string s){
	return new CommandComposite(s);
};

