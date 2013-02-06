#include <tcp-send-action.h>
#include <socket-factory.h>
#include <composite.h>
#include <ok-state.h>
#include <fail-state.h>

TcpSendAction::TcpSendAction():Command("<%TCP_send%>"){
	my_action = std::string("TcpSendAction");
	//socket = SocketFactory::getInstance()->getSocket(this);
};

TcpSendAction::TcpSendAction(Socket* s):Command("<%TCP_send%>"){
	my_action = std::string("TcpSendAction");
	socket = s;
};

TcpSendAction::~TcpSendAction(){};

Action* TcpSendAction::clone(){ 
	return new TcpSendAction ( SocketFactory::getInstance()->getSocket(this) ); 
//	return new TcpSendAction ( socket ); 
};
