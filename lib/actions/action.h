#ifndef _ACTION_H
#define _ACTION_H

#include <state.h>
#include <string>
#include <vector>

class Composite;
class Leaf;
class HexLeaf;
class BinLeaf;
class DecLeaf;
class StringLeaf;

class Action {
	private:
	
	protected:
	
	public:
	Action();
	virtual ~Action();
	virtual std::vector<State*> runSonsAction(Composite*);
	virtual State* runAction(Composite*)=0;
	virtual State* runAction(Leaf*);
	virtual State* runAction(HexLeaf*);
	virtual State* runAction(BinLeaf*);
	virtual State* runAction(DecLeaf*);
	virtual State* runAction(StringLeaf*);
	virtual std::string& getName(){ return my_action; };
	virtual std::vector<Composite**> getDataSons(Composite*);
	virtual bool thereIsAFail(std::vector<State*>&);
	private:
	
	protected:
	std::string	my_action;
	
	public:

};

#endif
