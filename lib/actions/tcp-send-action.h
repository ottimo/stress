#ifndef _TCP_SEND_ACTION_H
#define _TCP_SEND_ACTION_H

#include <state.h>
#include <send-action.h>
#include <string>
#include <socket.h>
#include <command.h>

class TcpSendAction : public SendAction, Command {
	private:
	
	protected:
	
	public:
		TcpSendAction();
		TcpSendAction(Socket*);
		~TcpSendAction();
		Action* clone();
	private:
	
	protected:
	
	public:
};

static TcpSendAction* tcp_send_action = new TcpSendAction();

#endif
