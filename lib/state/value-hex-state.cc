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

#include "value-hex-state.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ValueHexState::ValueHexState(int i) : ValueState(i){
	composite_id = i;
	my_name = std::string("ValueHexState");
	my_type = std::string("ValueHexState");
	state_value = OK;
};

ValueHexState::ValueHexState(int i,int n) : ValueState(i,n){
	composite_id = i;
	my_name = std::string("ValueHexState");
	my_type = std::string("ValueHexState");
	state_value = FAULT_VALUE;
};

std::string ValueHexState::getStringToSend(){
	std::string str = getStringValue();	
	std::string res;
	int it;
	//if(str.find("%x") != std::string::npos ){
		const uint8_t* u8ptr = getRawValue();
		std::stringstream ss;
		ss.write((const char*)u8ptr, getRawLenght());
		res = ss.str();
	//}
	return res;
};

std::string ValueHexState::getValueForLog(){
	std::stringstream ss;
	char ptr[3];
	memset(ptr,0,3);
	for(int i=0; i<getRawLenght(); i++){
		int c = (int)getRawValue()[i];
		if(c<32 || c>126){
			//ss << ".";
			ss << "%x";
			sprintf(ptr,"%02x", getRawValue()[i]);
			ss << ptr << "";
		}
		// rimosso (perchè ce l'ho messo?!?!?!)
		//else if(c == 34) // quando ci sono le virgolette dobbiamo mettere un backslash davanti
		//	ss << "\\" << (char)c;
		else
			parseChar( (const char*)&c, &ss);
//		sprintf(ptr,"%02x", getRawValue()[i]);
//		ss << ptr;
	}
	return ss.str();
};

int ValueHexState::getIntegerValue(){
	int res=0;
	for(int i = 0; i<raw_lenght; i++)
		res+= (int)raw_value[raw_lenght-1-i] * pow(2,(8*i));
	return res;
};
