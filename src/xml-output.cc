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

#include <xml-output.h>
#include <configurator.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>
#include <value-state.h>
#include <value-hex-state.h>
#include <send-packet-state.h>
#include <read-packet-state.h>

XMLOutput::XMLOutput() : AbstractOutput(){};

XMLOutput::~XMLOutput(){};

void XMLOutput::printTree(State* s, uint64_t id, int run){
	std::string outfilename = getFilename(id, run);
	outfilename+= ".xml";
	std::ostringstream abnfTreeString;
	runTreeState(&abnfTreeString, s);
	
	std::ofstream myfile (outfilename.c_str());
	if (myfile.is_open())
	{
		myfile << "<?xml version=\"1.0\" ?>" << std::endl;
		myfile << "<testcase id=\""<< id <<"\" run=\""<<run<<"\">" << std::endl;
		myfile << abnfTreeString.str();
		myfile << "</testcase>" << std::endl;
		myfile.close();
	}
	else 
		std::cerr<< std::string("Unable to open file ") << outfilename<<std::endl;
};

void XMLOutput::runTreeState(std::ostringstream* oss, State* s){
	std::vector<State*>& derived_state = s->getSons();
	std::vector<State*>::iterator sons_iterator;
	
/* 	if(dynamic_cast<ValueHexState*>(s)){
 		for(int i=0; i < s->getRawLenght();i++){
			char ptr[4];
			memset(ptr, 0, 4);
			sprintf(ptr, "%02x ", (unsigned char)s->getRawValue()[i]);
			*oss << ptr;
		} 
	}else */
	if(ValueState* vs =dynamic_cast<ValueState*>(s)){
//		*oss << "<state id=\"" << s->getId() << "\" name=\"" <<  s->getName() <<"\" ";
		*oss << "<" << vs->getTag();
		*oss <<  " id=\"" << s->getId() << "\" name=\"" <<  s->getName() <<"\" ";
		*oss << "type=\"" << s->getType() << "\" ";
		*oss << "data=\""; 
		*oss << vs->getValueForLog();
		*oss << "\" />" << std::endl; 
	//	*oss << "</state>" << std::endl; 
	} 
	
	//*oss << "<sons>" << std::endl; 
	for(sons_iterator = derived_state.begin(); sons_iterator != derived_state.end(); sons_iterator++){
		runTreeState(oss, (*sons_iterator));
	}
	//*oss << "</sons>" << std::endl; 

};
