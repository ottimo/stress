#include <raw-send-action.h>
#include <socket-factory.h>

RawSendAction::RawSendAction():Command("<%RAW_send%>"){
	my_action = std::string("RawSendAction");
};

RawSendAction::RawSendAction(Socket* s):Command("<%RAW_send%>"){
	my_action = std::string("RawSendAction");
	socket = s;
};

RawSendAction::~RawSendAction(){};

Action* RawSendAction::clone(){
	return new RawSendAction( SocketFactory::getInstance()->getSocket(this) );
};
