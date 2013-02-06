#ifndef _SEND_ACTION_H
#define _SEND_ACTION_H

#include <state.h>
#include <value-state.h>
#include <action.h>
#include <string>
#include <socket.h>

class SendAction : public Action {
	private:	

	protected:

		//	virtual void buildSocket()=0;


	public:
		SendAction();
		~SendAction();
		virtual State* runAction(Composite*);

	private:

	protected:

		Socket* socket;
		
	public:

};

#endif
