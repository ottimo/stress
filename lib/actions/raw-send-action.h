#ifndef _RAW_SEND_ACTION_H
#define _RAW_SEND_ACTION_H

#include <state.h>
#include <send-action.h>
#include <string>
#include <socket.h>
#include <command.h>

class RawSendAction : public SendAction, Command {
	private:
	
	protected:
	
	public:
		RawSendAction();
        RawSendAction(Socket*);
		~RawSendAction();
		//virtual State* runAction(Composite*);
        Action* clone();
	private:
	
	protected:
	
	public:

};

static RawSendAction* raw_send_action = new RawSendAction();

#endif
