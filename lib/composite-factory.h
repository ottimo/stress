#ifndef _COMPOSITE_FACTORY_H
#define _COMPOSITE_FACTORY_H

#include <composite.h>
#include <string>
#include <vector>

class CompositeFactory{
	public:

		CompositeFactory();
		~CompositeFactory();

		Composite* getNode(std::string);
		Composite* getOrNode(std::string);
		Composite* getRepeatNode(std::string);
		Composite* getOptionNode(std::string);
		Composite* getStringNode(std::string);
		Composite* getBinNode(std::string);
		Composite* getDecNode(std::string);
		Composite* getHexNode(std::string);
		Composite* getCommandNode(std::string);

	protected:

	private:

	public:

	protected:

	private:
		std::vector<Composite*> composites;
};

#endif
