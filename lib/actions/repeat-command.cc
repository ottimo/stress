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

#include "repeat-command.h"
#include <value-int-state.h>
#include <value-string-state.h>
#include <composite.h>
#include <math.h>

RepeatCommand::RepeatCommand() : Command("<%Repeat%>"){
	my_action = std::string("RepeatCommand");
};

RepeatCommand::~RepeatCommand(){};

Action* RepeatCommand::clone(){ 
	return new RepeatCommand(); 
};

State* RepeatCommand::runAction(Composite* c){
	// this method implements action
	State* ret=NULL;
	std::vector<State*> result = runSonsAction(c);
    
    // there should be a check for missing arguments and/or wrong numbers.
	std::string blob = result[1]->getStringValue();
	float times = 0;
    if (result[2]->getIntegerValue()>0)
		times = (float)result[2]->getIntegerValue();
    
    std::string s;
    for( int i=0; i<times; i++ )
        s += blob;
    
	ValueStringState* res = new ValueStringState(c->getId());
	res->setStringValue(s);

	return res;
};
