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

#include "dot-output.h"
#include <configurator.h>
#include <sstream>
#include <iostream>
#include <fstream>

DotOutput::DotOutput() : AbstractOutput(){};

DotOutput::~DotOutput(){};

void DotOutput::printTree(State* s, uint64_t id, int run){
	std::string outfilename = getFilename(id, run);
	outfilename+= ".dot";
	std::ostringstream abnfTreeString;
	runTreeState(&abnfTreeString, s);
	
	std::ofstream myfile (outfilename.c_str());
	if (myfile.is_open())
	{
		myfile << "digraph{" << std::endl;
		myfile << "graph [ overlap=false ]" << std::endl;
		myfile << abnfTreeString.str();
		myfile << "}" << std::endl;
		myfile.close();
	}
	else 
		std::cerr<< std::string("Unable to open file ") << outfilename<<std::endl;
};

void DotOutput::runTreeState(std::ostringstream* oss, State* s){
	std::vector<State*>& derived_state = s->getSons();
	std::vector<State*>::iterator sons_iterator;
	for(sons_iterator = derived_state.begin(); sons_iterator != derived_state.end(); sons_iterator++){
		*oss << s->getId() << "[label=\"" <<  s->getName() << " \"];" << std::endl;
		*oss << (*sons_iterator)->getId() << "[label=\"" << (*sons_iterator)->getName();
		*oss << "\"]" << std::endl; 
		
		*oss << s->getId() << "->" << (*sons_iterator)->getId()  << " ;" << std::endl;
		
		//(*sons_iterator)->printTestTree(oss);
		runTreeState(oss, (*sons_iterator));
	}
};
