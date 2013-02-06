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

#include "value-bin-state.h"

ValueBinState::ValueBinState(int i) : ValueState(i){
	composite_id = i;
	my_name = std::string("ValueBinState");
	my_type = std::string("ValueBinState");
	state_value = OK;
};

ValueBinState::ValueBinState(int i,int n) : ValueState(i,n){
	composite_id = i;
	my_name = std::string("ValueBinState");
	my_type = std::string("ValueBinState");
	state_value = FAULT_VALUE;
};

std::string ValueBinState::getStringToSend(){
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

std::string ValueBinState::getValueForLog(){
	return getStringToSend();
};
