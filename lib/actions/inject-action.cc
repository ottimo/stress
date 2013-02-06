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

#include "inject-action.h"
#include "value-state.h"
#include "state.h"
#include "number-value.h"
#include "test-case-manager.h"
#include <sstream>


InjectAction::InjectAction():injection_id(0){
	my_action = std::string("InjectAction");
	TestCaseManager::getInstance()->registerInjector(this);
};

InjectAction::InjectAction(uint64_t id):injection_id(id){
	my_action = std::string("InjectAction");
	TestCaseManager::getInstance()->registerInjector(this);
};

InjectAction::~InjectAction(){};

State* InjectAction::runAction(Composite* c){
	//TODO in caso di nodo terminale riconoscimento nome e valore da inserire
	//nello state di ritorno
	//OkState* ret = new OkState(c->getId());
	//ret->setStringValue(c->getName());             
	std::stringstream ss;

	std::cout << "Injection ID " << injection_id << std::endl;
	std::vector<State*> result = runSonsAction (c);
	TestCaseManager::getInstance()->setSonsNumber(injection_id, result.size());
	int value_num = TestCaseManager::getInstance()->updateInjector(injection_id);
	//NumberValue visitorValue;
	//int num_of_value = visitorValue.retrieveNumberValue( result );
	std::cerr << " Value Num " << value_num << " "; // << std::endl;
	//ValueState* ret = new ValueState(c->getId(), 0);

	State* res = result[value_num];
	ss <<"Anomaly-";
	ss << value_num <<"";
	std::vector<State*>::iterator it;
	for(it = result.begin(); it!=result.end(); it++){
		if((*it)!=res)
			delete (*it);
	}
	result.clear();
	
	//ValueState* value = visitorValue.getValueState(value_num);
	//ret->setStringValue( result[value_num]->getStringToSend() );
	//std::cout << "Fault Value "<<result[value_num]->getStringToSend() << std::endl;
	res->setFaultValue();
	res->setName(ss.str());
	return res;
};
