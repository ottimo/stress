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

#include "length-command.h"
#include "composite.h"
#include <value-hex-state.h>
#include <string.h>
#include <stdio.h>

LengthCommand::LengthCommand() : Command("<%Length%>"){
	my_action = std::string("LengthCommand");
};

LengthCommand::~LengthCommand(){};

Action* LengthCommand::clone(){ 
	return new LengthCommand(); 
};

/*!
 *	\func LengthCommand::runSonsAction(Composite* c)
 *	
 *	\brief This action return the byte length of the second child
 *	
 *	First child will be the CommandComposite <%Length%> give this action to
 *	Composite* c;
 *	Second child is mandatory and its byte length will be the result of this
 *	action;
 *	Third child is optional and it set the size of the result this action will
 *	return, if it absents the results size will be depend to its value;
 *	for example if the length is a value between 0 and 255, the result will have
 *	one byte length, if the value is between 256 and 65535, the result will have two
 *	byte length, and so on...
 *
 */

State* LengthCommand::runAction(Composite* c){
	// this method implements action
	ValueHexState* ret=  new ValueHexState(c->getId());
	std::vector<State*> result = runSonsAction(c);

	if(result.size()==3){
		int result_size = result[2]->getIntegerValue();
		unsigned int value = 0;
		uint8_t* ptr = new uint8_t[result_size];
		uint8_t* pint = NULL;
		memset(ptr, 0, result_size);
	 	value = (unsigned int) result[1]->getStringToSend().size();
		pint = (uint8_t*) &value;
  		std::cout<< "value "<<value << " result_size "<< result_size<< std::endl;
		for(int i=0; i< result_size; i++){
			ptr[result_size-1-i] = pint[i];
			printf("%02x ", pint[i]);
		}
		for(int i=0; i< result_size; i++)
			printf("%02x ", ptr[i]);
		ret->setRawValue(ptr, result_size);
		delete [] ptr;
	}else if(result.size()==2){
		//TODO intelligent result length setting
	}

	*ret << result;
	return ret;
};
