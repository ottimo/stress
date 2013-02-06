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

#ifndef _TEST_CASE_MANAGER_H_
#define _TEST_CASE_MANAGER_H_

#include <map>
#include <vector>
#include <inject-action.h>
#include <abstract-monitor.h>
#include <state.h>
#include <ts-generator.h>

class InjectState{
	public:
		InjectState(InjectAction*);
		InjectState(const InjectState&);
		~InjectState();
		void setNumSons(int n){ num_sons = n;};
		int getNumSons(){ return num_sons;};
		void setNext(InjectState* isp){next = isp;};
		uint64_t getId();
		int getState();
		void resetState(){last = 1;};
		bool increaseState();
		InjectState& operator=(const InjectState&);
	protected:
	
	private:
		InjectAction* owner;
		int num_sons;
		int last;
		InjectState* next;
};

class TestCaseManager
{
	public:
		~TestCaseManager();
		static TestCaseManager* getInstance();
		void registerInjector(InjectAction*);
		void unregisterInjector(InjectAction*);
		void setSonsNumber(uint64_t, int);
		int updateInjector(uint64_t);
		void prepareNextTestCase();
		bool checkVariable(std::string);
		void setVariable(std::string, State*);
		State* getVariable(std::string);
		void setLastFrame(State*);
		State* getLastFrame();
		int getRun(uint64_t);
		uint64_t getId(){return actual_id;};
		void sendNotificationStartTestCase();
		void sendNotificationFinishTestCase();
        void sendNotificationPacketRead();
        void sendNotificationPacketSent();
		State* getReport();
		void closeMonitor();
		void waitTimeout();
	protected:
		TestCaseManager();
	private:
		void updateInjectorState();
		void resetVariable();
		static TestCaseManager* _instance;
		std::map<uint64_t,InjectState*> observer;
		std::map<std::string, State*> variables;
		std::vector<InjectState*> test_case_state;
		uint64_t actual_id;
		State* last_frame;
		std::vector<AbstractMonitor*> monitors;
};

#endif // _TEST_CASE_MANAGER_H_
