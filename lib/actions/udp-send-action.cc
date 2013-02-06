#include <udp-send-action.h>
#include <socket-factory.h>
#include <composite.h>
#include <ok-state.h>
#include <fail-state.h>

UdpSendAction::UdpSendAction():Command("<%UDP_send%>") {
	my_action = std::string("UdpSendAction");
	//socket = SocketFactory::getInstance()->getSocket(this);
};

UdpSendAction::UdpSendAction(Socket* s):Command("<%UDP_send%>") {
	my_action = std::string("UdpSendAction");
	socket =s;
};

UdpSendAction::~UdpSendAction(){};

Action* UdpSendAction::clone(){
	return new UdpSendAction (SocketFactory::getInstance()->getSocket(this));
};
