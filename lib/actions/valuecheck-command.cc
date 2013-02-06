/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * trunk
 * Copyright (C) Matteo Rosi 2010 <matteo.rosi@gmail.com>
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

#include "valuecheck-command.h"
#include "composite.h"
#include <string.h>
#include <ok-state.h>
#include <fail-state.h>
#include <value-hex-state.h>
#include <test-case-manager.h>
#include <cstdlib>
#include <iomanip>

int wrmemcmp(const char* first, const char* second, int length){
	int disequal=0;
	for(int i = 0; i< length; i++)
		disequal+= (int) (first[i] ^ second[i]);
	return disequal;
};

ValueCheckCommand::ValueCheckCommand() : Command("<%ValueCheck%>"){
	my_action = std::string("ValueCheckCommand");
};

ValueCheckCommand::~ValueCheckCommand(){};

Action* ValueCheckCommand::clone(){ 
	return new ValueCheckCommand(); 
};

State* ValueCheckCommand::runAction(Composite* c){
	// this method implements action
	State* ret=NULL;
	std::vector<State*> result = runSonsAction(c);
	State* lastframe = TestCaseManager::getInstance()->getLastFrame();
	int start = atoi(result[1]->getStringToSend().c_str());
	int size = atoi(result[2]->getStringToSend().c_str());
	int disequal = wrmemcmp(lastframe->getStringToSend().c_str()+start, 
									result[3]->getStringToSend().c_str(), 
									size);

/**/	
	std::cout << "ValueCheckCommand " << start << size << disequal << std::endl;
	for(int i=0; i< size; i++)
		std::cout << std::setw(2) << std::setfill('0')<<std::hex << (int)lastframe->getStringToSend().c_str()[i];
	std::cout<<std::endl;
	for(int i=0; i< size; i++)
		std::cout << std::setw(2) << std::setfill('0')<<std::hex << (int)result[3]->getStringToSend().c_str()[i];
	std::cout<<std::endl;
 /**/	
	if(!disequal){
		OkState* okret = new OkState(c->getId());
		okret->setStringValue(c->getName());
		ret = (State*)okret;
	}else{
		FailState* failret = new FailState(c->getId());
		failret->setStringValue("ValueCheck Fail");
		failret->setFailValue();
		ret = (State*)failret;
		
		ValueHexState* received =  new ValueHexState(c->getId());
		received->setRawValue((uint8_t*)lastframe->getStringToSend().c_str()+start, size);
		result.push_back(received);
	}
	*ret << result;
	return ret;
};
