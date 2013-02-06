/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
stress is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * stress is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ts-generator.h"
#include <iostream>

ExpTSGenerator::ExpTSGenerator(){
	std::cout << "ExpTSGenerator loaded;" << std::endl;
};

bool ExpTSGenerator::enable(int id, int injector_id){
	if(id & injector_id)
		return true;
	else 
		return false;
};

uint64_t ExpTSGenerator::getNextInjectionId(uint64_t& next_injection_id){
	uint64_t n = next_injection_id;
	if(n == UINT64_MAX){
		//std::cerr << "Maximum number of fault reached! " << next_injection_id << std::endl;
		return n;
	}
	next_injection_id <<=1;
	return n;
};

SeqTSGenerator::SeqTSGenerator(){
	std::cout << "SeqTSGenerator loaded;" << std::endl;
};

bool SeqTSGenerator::enable(int id, int injector_id){
	if(id == injector_id)
		return true;
	else 
		return false;
};

uint64_t SeqTSGenerator::getNextInjectionId(uint64_t& next_injection_id){
	uint64_t n = next_injection_id;
	next_injection_id +=1;
	return n;
};