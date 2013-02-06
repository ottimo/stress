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

#include <r-t-t-monitor.h>
#include <monitor-report.h>
#include <configurator.h>
#include <sstream>
#include <iostream>

RTTMonitor::RTTMonitor():send_time(boost::posix_time::pos_infin),rtts(){};

RTTMonitor::~RTTMonitor(){};

State* RTTMonitor::getReport(){
	std::stringstream ss;
	boost::posix_time::time_duration rtt = stop - start;
	double timeout = 2*Configurator::getInstance()->getDelay();
	rtt = rtt - boost::posix_time::time_duration(boost::posix_time::milliseconds(timeout));
	MonitorReport* res = new MonitorReport(0);
	res->setName("Test Case Duration");
	ss << rtt;
	res->setStringValue(ss.str());
	std::cout << "RTT " << start << " "<< stop  << " " << Configurator::getInstance()->getDelay() << " " << rtt <<  std::endl;
        
        std::vector<boost::posix_time::time_duration>::iterator it;
        for(it = rtts.begin(); it != rtts.end(); it++){
			std::cout << "RTT " << (*it) << std::endl;
            ss.str("");
            MonitorReport* r = new MonitorReport(0);
            r->setName("RTT Report");
            ss << (*it);
            r->setStringValue(ss.str());
            (*res) << r;
        }

        rtts.clear();
        return (State*)res;
};

void RTTMonitor::start_test(){
	start = boost::posix_time::microsec_clock::universal_time();
        send_time = boost::posix_time::pos_infin;
};

void RTTMonitor::finish_test(){
	stop = boost::posix_time::microsec_clock::universal_time();
};

void RTTMonitor::packet_sent(){
    send_time = boost::posix_time::microsec_clock::universal_time();
};
void RTTMonitor::packet_read(){
    if(send_time != boost::posix_time::pos_infin)
        rtts.push_back( boost::posix_time::microsec_clock::universal_time() - send_time );
};
