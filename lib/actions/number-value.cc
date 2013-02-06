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

#include "number-value.h"
#include <iostream>

NumberValue::NumberValue() : number_of_value_state(0){};

int NumberValue::retrieveNumberValue(std::vector<State*>& vs){
	std::vector<State*>::iterator it;
	for(it = vs.begin(); it != vs.end(); it++){
		//if( (*it)->getName().compare("ValueState")!= 0)
		try{
			retrieveNumberValue( dynamic_cast<ValueState*>(*it) );
		}catch(std::exception e){
			retrieveNumberValue( (*it) );
		};
			
	}
	return number_of_value_state;
};
void NumberValue::retrieveNumberValue(State* s){
	retrieveNumberValue( s->getSons() );
};
void NumberValue::retrieveNumberValue(ValueState* vs){
	number_of_value_state++;
	results.push_back(vs);
};
ValueState* NumberValue::getValueState(int i){
	if(i < results.size())
	    return results[i];
	else 
		return NULL;
};
