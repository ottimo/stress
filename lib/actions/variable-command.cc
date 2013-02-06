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

#include "variable-command.h"
#include <cstdlib>
#include <iostream>
#include <test-case-manager.h>
#include <ok-state.h>
#include <value-hex-state.h>
#include <string.h>

VariableCommand::VariableCommand() : Command("<%Variable%>"){
	my_action = std::string("VariableCommand");
};

VariableCommand::~VariableCommand(){
	my_action.erase();
};

Action* VariableCommand::clone(){ 
	return new VariableCommand(); 
};

State* VariableCommand::runAction(Composite* c){
	// this method implements action
	if(c->getSons().size() != 4 && c->getSons().size() != 2) // command code + variable name + start + lenght
		usage();
	State* res = NULL;
	std::vector<State*> result = runSonsAction(c);
	if( TestCaseManager::getInstance()->checkVariable( result[1]->getStringValue() ) ){
		res = new ValueHexState( c->getId());
		res->setName(std::string("Variable: ")+TestCaseManager::getInstance()->getVariable(result[1]->getStringValue())->getName());
		std::string value = TestCaseManager::getInstance()->getVariable(result[1]->getStringValue())->getStringToSend();
		uint8_t* u8p = new uint8_t[value.size()];
		memset(u8p, 0, value.size());
		memcpy(u8p, value.c_str(), value.size());

		res->setRawValue(u8p, value.size());

		delete [] u8p;
	} else {
		State* frame = TestCaseManager::getInstance()->getLastFrame();
		const uint8_t* data = frame->getRawValue();
		int lenght = frame->getRawLenght();
		res = new ValueHexState(c->getId());
		uint8_t* u8p =(uint8_t*) data + result[2]->getIntegerValue() ;

		res->setRawValue(u8p, result[3]->getIntegerValue());
		//std::cout << result[1]->getStringValue() <<" variabile lunga "<<      result[3]->getIntegerValue() << " " << result[3]->getValueForLog() << std::endl;
		TestCaseManager::getInstance()->setVariable(result[1]->getStringValue(), res);			
	}
	*res << result;
	return res;
};

void VariableCommand::usage(){
	std::cerr << "VariableCommand: Syntax error" << std::endl;
	std::cerr << "   usage: "<< command_name << " String(Variable name) Integer(Starting byte) Integer(Lenght in byte)" << std::endl;
	exit(-1);
};

std::vector<Composite**> VariableCommand::getDataSons(Composite* c){
	std::vector<Composite**> datasons;
	return datasons;
};
