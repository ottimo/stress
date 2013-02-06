#ifndef _UDP_SEND_ACTION_H
#define _UDP_SEND_ACTION_H

#include <state.h>
#include <send-action.h>
#include <string>
#include <socket.h>
#include <command.h>

class UdpSendAction : public SendAction, Command {
	private:
	
	protected:

	public:
		UdpSendAction();
		UdpSendAction(Socket*);
		~UdpSendAction();
		Action* clone();
	private:
	
	protected:
	
	public:

};
static UdpSendAction* udp_send_action = new UdpSendAction();

#endif
