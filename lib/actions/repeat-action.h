#ifndef _REPEAT_ACTION_H
#define _REPEAT_ACTION_H

#include <state.h>
#include <return-action.h>
#include <string>

class RepeatAction : public ReturnAction {
	private:
	
	protected:
	
	public:
	RepeatAction();
	~RepeatAction();
	State* runAction(Composite*);

	private:
	
	protected:
	
	public:

};

#endif
