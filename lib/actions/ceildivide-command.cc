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

#include "ceildivide-command.h"
#include <value-int-state.h>
#include <composite.h>
#include <math.h>

CeilDivideCommand::CeilDivideCommand() : Command("<%CeilDivide%>"){
	my_action = std::string("CeilDivideCommand");
};

CeilDivideCommand::~CeilDivideCommand(){};

Action* CeilDivideCommand::clone(){ 
	return new CeilDivideCommand(); 
};

State* CeilDivideCommand::runAction(Composite* c){
	// this method implements action
	State* ret=NULL;
	std::vector<State*> result = runSonsAction(c);

	float numerator = 0.0;
	float denominator = 1.0;

	numerator = (float)result[1]->getIntegerValue();
	if (result[2]->getIntegerValue()>0)
		denominator = (float)result[2]->getIntegerValue();

	float res = ceil(numerator / denominator);
	std::cout << res << " = " << numerator << " / " << denominator << std::endl;
	ret = new ValueIntState(c->getId());
	ret->setIntegerValue((int)res);

	return ret;
};
