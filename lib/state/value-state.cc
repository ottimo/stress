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

#include "value-state.h"
#include <sstream>
#include <string.h>
#include <iostream>

ValueState::ValueState(int i){
	composite_id = i;
	my_name = std::string("ValueState");
	my_type = std::string("ValueState");
	state_value = OK;
};

ValueState::ValueState(int i,int n){
	composite_id = i;
	my_name = std::string("ValueState");
	my_type = std::string("ValueState");
	state_value = FAULT_VALUE;
};


std::string ValueState::getStringToSend(){
	std::string str = getStringValue();	
	std::string res;
	int it;
	if(str.find("\\\"") != std::string::npos){
		while( ( it = str.find("\\\"") ) != std::string::npos ){
			str.erase(it, 2);
		}
		res = str;
	}else if(str.find("%x") != std::string::npos ){
		const uint8_t* u8ptr = getRawValue();
		std::stringstream ss;
		ss.write((const char*)u8ptr, getRawLenght());
		res = ss.str();
	}else if(str.find("%d") != std::string::npos ){
		std::stringstream ss;
		ss << getIntegerValue();
		res = ss.str();
	}else
		res = str;
	return res;
};

void ValueState::parseChar(const char* ccptr, std::stringstream* ssptr){
	if(strncmp( ccptr, "<",1)==0)
		(*ssptr)<< "&lt;";
	else if(strncmp( ccptr, ">",1)==0)
		(*ssptr)<< "&gt;";
	else if(strncmp( ccptr, "&",1)==0)
		(*ssptr)<< "&amp;";
	else if(strncmp( ccptr, "\'",1)==0)
		(*ssptr)<< "&apos;";
	else if(strncmp( ccptr, "\"",1)==0)
		(*ssptr)<< "&quot;";
	else if( strncmp( ccptr, "`",1)==0){
		(*ssptr)<< "\\";
		(*ssptr) << (*ccptr);
	}else
		(*ssptr) << (*ccptr);
};
