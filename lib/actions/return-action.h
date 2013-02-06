#ifndef _RETURN_ACTION_H
#define _RETURN_ACTION_H

#include <state.h>
#include <action.h>
#include <string>
#include <hex-leaf.h>
#include <bin-leaf.h>
#include <dec-leaf.h>
#include <string-leaf.h>
#include <command-composite.h>
#include <vector>

class ReturnAction : public Action {
	private:
	
	protected:
	
	public:
		ReturnAction();
		~ReturnAction();
                virtual std::vector<State*> runSonsAction(Composite*);
		State* runAction(Composite*);
		State* runAction(Leaf*);
		State* runAction(HexLeaf*);
		State* runAction(BinLeaf*);
		State* runAction(StringLeaf*);
		State* runAction(DecLeaf*);
		
	private:
	
	protected:
	
	public:

};

#endif
