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

#include "addition-command.h"
#include <value-int-state.h>
#include <composite.h>

AdditionCommand::AdditionCommand() : Command("<%Sum%>"){
	my_action = std::string("AdditionCommand");
};

AdditionCommand::~AdditionCommand(){};

Action* AdditionCommand::clone(){ 
	return new AdditionCommand(); 
};

State* AdditionCommand::runAction(Composite* c){
	// this method implements action
	std::vector<State*> result = runSonsAction(c);
	std::vector<State*>::iterator it;
	int sum = 0;
	for(it=result.begin(); it!=result.end(); it++){
		sum += (*it)->getIntegerValue();
	}
	State* res = new ValueIntState(c->getId());
	res->setIntegerValue(sum);
	return res;
};
