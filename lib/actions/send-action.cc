#include <send-action.h>
//#include <value-string-state.h>
#include <send-packet-state.h>
#include <fail-state.h>
#include <composite.h>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include "test-case-manager.h"



SendAction::SendAction(){
	my_action = std::string("SendAction");
	
};

SendAction::~SendAction(){};

State* SendAction::runAction(Composite* c){
	State* res = NULL;
	std::vector<uint8_t> data_to_send;
	std::vector<State*>::iterator sons_iterator;
	
	std::vector<State*> result = runSonsAction(c);
	

	for(sons_iterator = result.begin(); sons_iterator != result.end(); sons_iterator++){
		std::cout << "+";
		if(sons_iterator != result.begin()){
			std::string str = (*sons_iterator)->getStringToSend();
			std::cout << str;
			for(int i=0; i<str.size(); i++){
				data_to_send.push_back( str[i] );
			}
		}
	}
	std::stringstream ss;
	std::cout << "Data to send " ;
	for(int i=0; i<data_to_send.size(); i++){
		printf("%02x ", (unsigned char)data_to_send[i]);
		ss << (char) data_to_send[i];
	}
	std::cout << std::endl;
	std::cout << "String to send \"" << ss.str() << "\""<< std::endl;
	
	
	int n = socket->send(data_to_send);
	if(n>=0){
            res = new SendPacketState(c->getId());
            TestCaseManager::getInstance()->sendNotificationPacketSent();
            res->setName("Data sent");
            std::string s;
            s+="\\\"";
            s+= ss.str();
            s+="\\\"";
            //std::cout << "PROVA OUTPUT " << s <<std::endl;
            res->setStringValue(s);
	}else{
            //res= new FailState(c->getId());
			res= new SendPacketState(c->getId());
			res->setFailValue();
	}
		
	*res << result;
	res->setPacketValue();
	return res;
};



