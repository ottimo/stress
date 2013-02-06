/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * trunk
 * Copyright (C) Matteo Rosi 2009 <matteo.rosi@gmail.com>
 * 
 * trunk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * trunk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "read-action.h"
#include <value-string-state.h>
#include <read-packet-state.h>
#include <value-hex-state.h>
#include <composite.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <test-case-manager.h>

ReadAction::ReadAction(){
	my_action = std::string("ReadAction");
};

ReadAction::~ReadAction(){};

State* ReadAction::runAction(Composite* c){
	State* res = NULL;
	std::vector<uint8_t> read_data;
	std::vector<State*>::iterator it;
	std::vector<State*> result;

	socket->resetTimeout();
	int n = 0;
	// TODO ciclo while con controllo sul check dei figli
	while(n>=0){

		 n = socket->read(read_data);

		 if(n>0){
			  res = new ReadPacketState(c->getId());
			  TestCaseManager::getInstance()->sendNotificationPacketRead();

			  std::cout << "Data Read " << std::dec << n <<" bytes: " ;
			  std::cout << std::endl;
			  uint8_t* data = new uint8_t[n];
			  for(int i=0; i<n; i++){
					if(!(i%4) && i!=0)
						 std::cout << " ";
					if(!(i%32) && i!=0)
						 std::cout << std::endl;
					data[i] = read_data[i];
					std::cout << std::setw(2) << std::setfill('0')<<std::hex << (int)data[i];
			  }
			  std::cout << std::dec << std::endl;
			  res->setName("Data received");
			  res->setRawValue(data, n);
			  delete [] data;

		 }else{
			  //res= new ValueStringState(c->getId());
 			  res= new ReadPacketState(c->getId());
			  res->setName("No Packet Received");
			  res->setFailValue();
			  //std::vector<State*> result = runSonsAction(c);
			  //res->setPacketValue();
		 }
		 State* last=TestCaseManager::getInstance()->getLastFrame();
		 if(last!= 0)
			delete last;
 		 TestCaseManager::getInstance()->setLastFrame(res);
		 result = runSonsAction(c);
		 *res << result;
		 if( !thereIsAFail(result) )
			  n=-1;
	}
	TestCaseManager::getInstance()->setLastFrame(0);
	if(res->isFailValue())
		 return res;


	res->setPacketValue();

	return res;
};
