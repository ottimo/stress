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

#include "maskcheck-command.h"
#include "composite.h"
#include <ok-state.h>
#include <fail-state.h>
#include <test-case-manager.h>
#include <stdio.h>

MaskCheckCommand::MaskCheckCommand() : Command("<%MaskCheck%>"){
	my_action = std::string("MaskCheckCommand");
};

MaskCheckCommand::~MaskCheckCommand(){};

Action* MaskCheckCommand::clone(){ 
	return new MaskCheckCommand(); 
};

State* MaskCheckCommand::runAction(Composite* c){
	// this method implements action
	std::vector<State*> result = runSonsAction (c);
	State* lastframe = TestCaseManager::getInstance()->getLastFrame();

	int minlenght = lastframe->getStringToSend().size();
	bool equal = true;
	std::vector<State*>::iterator it;
	State* ret = NULL;
	for(it = result.begin()+1; it != result.end(); it++){
		if( (*it)->getStringToSend().size() < minlenght)
			minlenght = (*it)->getStringToSend().size();
	}
	for(int i=0; i<minlenght && equal == true; i++){
//		printf("%d: %02x <---> %02x \n",	 i,	
//		    			lastframe->getStringToSend()[i] & result[1]->getStringToSend()[i], 
//		    			result[2]->getStringToSend()[i] & result[1]->getStringToSend()[i]);
		if( (lastframe->getStringToSend()[i] & result[1]->getStringToSend()[i]) !=
		    	(result[2]->getStringToSend()[i] & result[1]->getStringToSend()[i]) )
			equal = false;
	}
	if(equal){
		OkState* okret = new OkState(c->getId());
		okret->setStringValue(c->getName());
		ret = (State*)okret;
	}else{
		FailState* failret = new FailState(c->getId());
		failret->setStringValue("MaskCheck Fail");
		failret->setFailValue();
		ret = (State*)failret;
	}
	*ret << result;
	return ret;
	
};
