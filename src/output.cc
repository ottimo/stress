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

#include "output.h"
#include <configurator.h>
#include <sstream>

AbstractOutput::AbstractOutput(){
	filename = Configurator::getInstance()->getOutputFile();
};

std::string AbstractOutput::getFilename(uint64_t id, int run){
	std::ostringstream ss;
	std::string idrun, idid;
	std::string zero("0");
	ss << run;
	for(int i = 0; i<12-ss.str().size(); i++)
		idrun += zero;
	idrun += ss.str();
	ss.str("");
	 
	ss << (long long int) id;
	for(int i = 0; i<12-ss.str().size(); i++)
		idid += zero;
	idid += ss.str();
		
	// reset stringstream ed inizio creazione nomefile
	ss.str("");
	ss << filename;
	ss << "-";
	ss << idid;
	ss << "-";
	ss << idrun;
	//ss << run;
	return ss.str();
};
