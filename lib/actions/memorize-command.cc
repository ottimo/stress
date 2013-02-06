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

#include "memorize-command.h"
#include "composite.h"
#include <test-case-manager.h>
#include <ok-state.h>
#include <value-hex-state.h>

MemorizeCommand::MemorizeCommand() : Command("<%Memorize%>"){
	my_action = std::string("MemorizeCommand");
};

MemorizeCommand::~MemorizeCommand(){
	my_action.erase();
};

Action* MemorizeCommand::clone(){ 
	return new MemorizeCommand(); 
};

/*!

\func State* MemorizeCommand::runAction(Composite* c)
\brief Memorize the input value in a variable and return that value

First child is the command node;
Second child is mandatory and it's the variable name;
Third child is mandatory too and it's the value that this action will store
inside the variable.

 */
State* MemorizeCommand::runAction(Composite* c){
	// this method implements action
	State* ret=NULL;
	std::vector<State*> result = runSonsAction(c);

	if( TestCaseManager::getInstance()->checkVariable( result[1]->getStringValue() ) ){
		ret = new ValueHexState( c->getId());
		ret->setStringValue( TestCaseManager::getInstance()->getVariable(result[1]->getStringValue())->getStringValue() );
		std::string value = TestCaseManager::getInstance()->getVariable(result[1]->getStringValue())->getStringToSend();
		ret->setRawValue((uint8_t*)value.c_str(), value.size());

	}else{
		ret = new ValueHexState( c->getId());
		TestCaseManager::getInstance()->setVariable(result[1]->getStringValue(),
																  result[2]);
		std::string value = result[2]->getStringToSend();
		ret->setRawValue((uint8_t*)value.c_str(), value.size());
		
	}
	*ret << result;
	return ret;
};

std::vector<Composite**> MemorizeCommand::getDataSons(Composite* c){
	std::vector<Composite**> datasons;
	datasons.push_back( &(c->getSons()[2]) );
	return datasons;
};
