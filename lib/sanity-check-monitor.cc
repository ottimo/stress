/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
 * stress is free software: you can redistribute it and/or modify it
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

#include "sanity-check-monitor.h"
#include "socket-factory.h"
#include <monitor-report.h>
#include <sstream>
#include <iostream>

SanityCheckMonitor::SanityCheckMonitor(){};

SanityCheckMonitor::~SanityCheckMonitor(){};

State* SanityCheckMonitor::getReport(){
	std::stringstream ss;
	MonitorReport* res = new MonitorReport(0);
	res->setName("SanityCheck");
	ss << socket_check_result;
	res->setStringValue(ss.str());
	std::cout << "SanityCheck result: " << socket_check_result <<  std::endl;
    return (State*)res;
};

void SanityCheckMonitor::start_test(){
	socket=SocketFactory::getInstance()->getSocket();
	socket_check_result = false;
};

void SanityCheckMonitor::finish_test(){
	if(socket){
		socket_check_result = socket->check();
		SocketFactory::getInstance()->releaseSocket(socket);
		socket = 0;
	}
};
