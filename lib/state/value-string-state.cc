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

#include "value-string-state.h"
#include "string.h"

ValueStringState::ValueStringState(int i) : ValueState(i){
	composite_id = i;
	my_name = std::string("ValueStringState");
	my_type = std::string("ValueStringState");
	state_value = OK;
};

ValueStringState::ValueStringState(int i,int n) : ValueState(i,n){
	composite_id = i;
	my_name = std::string("ValueStringState");
	my_type = std::string("ValueStringState");
	state_value = FAULT_VALUE;
};

std::string ValueStringState::getStringToSend(){
	std::string str = getStringValue();	
	std::string res("");
	int it;
//	if(str.find("\\\"") != std::string::npos){
//		while( ( it = str.find("\\\"") ) != std::string::npos ){
// 			str.erase(it, 2);
//		}
		res = str;
//	}
	return res;
};

std::string ValueStringState::getValueForLog(){
	std::string s = getStringToSend();
	std::stringstream ss;
	if(state_value != PACKET_VALUE){
		for(int i =0; i< s.size(); i++){
                    parseChar(&(s.c_str()[i]), &ss);
		}
	}else{
		//ss << State::getValueForLog();

		std::vector<State*>::iterator it;
		for(it = derived_state.begin(); it!=derived_state.end(); it++){
                    ss << (*it)->getValueForLog();
                }
	}
	 return ss.str();
};
