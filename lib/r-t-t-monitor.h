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

#ifndef _R_T_T_MONITOR_H_
#define _R_T_T_MONITOR_H_

#include <abstract-monitor.h>
#include <sys/time.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>

class RTTMonitor : public AbstractMonitor{
	public:
		RTTMonitor();
		~RTTMonitor();

		State* getReport();
		void start_test();
		void finish_test();
        void packet_sent();
        void packet_read();
	protected:

	private:
		boost::posix_time::ptime start;
		boost::posix_time::ptime stop;
        boost::posix_time::ptime send_time;
        std::vector<boost::posix_time::time_duration> rtts;
};

#endif // _R_T_T_MONITOR_H_
