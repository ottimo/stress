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

#include "test-case-manager.h"
#include <configurator.h>
#include <monitor.h>
#include <r-t-t-monitor.h>
#include <sanity-check-monitor.h>
#include <ok-state.h>

InjectState::InjectState(InjectAction* inj): owner(inj),  num_sons(-1), next(NULL),last(1){};
InjectState::InjectState(const InjectState& inj): owner(inj.owner),  num_sons(inj.num_sons), next(inj.next),last(inj.last){};
InjectState::~InjectState(){};
InjectState& InjectState::operator=(const InjectState& is2){
	InjectState* is = new InjectState(is2);
	return (*is);
};
uint64_t InjectState::getId(){
	return owner->getInjectionId();
};
int InjectState::getState(){
	return last;
};
bool InjectState::increaseState(){
	last++;
	if(last>=num_sons){
		resetState();
		return false;
	}
	return true; 
};

TestCaseManager* TestCaseManager::_instance;

TestCaseManager::TestCaseManager():last_frame(0){
	//TODO add a more smart monitor load system
	if(Configurator::getInstance()->getMonitor()){
		//monitors.push_back( new Monitor() );
		monitors.push_back( new RTTMonitor() );
		monitors.push_back( new SanityCheckMonitor() );
	}
};

TestCaseManager::~TestCaseManager(){
// TODO	delete monitors;
	
};

TestCaseManager* TestCaseManager::getInstance(){
	if(_instance == 0)
			_instance = new TestCaseManager();
	return _instance;
};

void TestCaseManager::registerInjector(InjectAction* injector){
	InjectState* last = NULL;
	if(!observer.empty())
		last = observer.rbegin()->second;



	observer[injector->getInjectionId()] = new InjectState(injector);
	if(last){
		observer.rbegin()->second->setNext(last);
	}

};

void TestCaseManager::unregisterInjector(InjectAction* injector){
	observer.erase(injector->getInjectionId());
};

void TestCaseManager::setSonsNumber(uint64_t id, int number){
	if(observer[id]->getNumSons() < 0)
		observer[id]->setNumSons(number);
};

int TestCaseManager::getRun(uint64_t id){
	std::map<uint64_t,InjectState*>::iterator it;
	int result = 1;
	TSGenerator* ts_generator = Configurator::getInstance()->getTSGenerator();

	actual_id = id;
	test_case_state.clear();
	
	for(it = observer.begin(); it!=observer.end(); it++){
		//if(id & it->first){
		if( ts_generator->enable(id, it->first) ){
			result *= (it->second->getNumSons()-1);
			it->second->resetState();
			test_case_state.push_back(it->second);
			std::cout << "Anomalies active " << result;
		}
	}
	return result;
};

int TestCaseManager::updateInjector(uint64_t id){
	std::vector<InjectState*>::iterator it;
	for(it = test_case_state.begin(); it != test_case_state.end(); it++){
		if((*it)->getId() == id)
			return (*it)->getState();
	}
	return 0;
};

void TestCaseManager::prepareNextTestCase(){
	resetVariable();
	updateInjectorState ();
};

void TestCaseManager::updateInjectorState(){
	std::vector<InjectState*>::iterator it;
	it = test_case_state.begin();
	if( !test_case_state.empty() &&  !(*it)->increaseState() ){
		std::vector<InjectState*>::iterator it2;
		for(it2 = test_case_state.begin()+1; it2!=test_case_state.end() && !(*it2)->increaseState() ;it2++){
		}
	}
};

void TestCaseManager::resetVariable(){
	std::map<std::string, State*>::iterator it;
	std::cout << "Used variables: " << std::endl;
	for(it=variables.begin(); it!=variables.end(); it++){
		std::cout<< "        " <<(*it).first << " -> " << (*it).second->getStringValue() << std::endl;
		std::cout << " ( ";
		std::string res = (*it).second->getStringValue();
		for( int i=0; i<res.length(); i++ )
			std::cout << std::hex << "0x" << int(res[i]) << " " << std::dec;
		std::cout << ")" << std::endl;
		}
	variables.clear();
};

bool TestCaseManager::checkVariable(std::string var){
	if(variables[var])
		return true;
	else
		return false;
};

void TestCaseManager::setVariable(std::string name, State* s){
	if(!checkVariable (name))
		variables[name] = s;
};

State* TestCaseManager::getVariable(std::string name){
	return variables[name];
};

void TestCaseManager::setLastFrame(State* s){
	last_frame = s;
};

State* TestCaseManager::getLastFrame(){
	return last_frame;
};

void TestCaseManager::sendNotificationStartTestCase(){
	std::cout << "Test case starting" << std::endl;
	std::vector<AbstractMonitor*>::iterator it;
	for( it = monitors.begin(); it!=monitors.end(); it++)
		(*it)->start_test();
	waitTimeout();
};

void TestCaseManager::sendNotificationFinishTestCase(){
	std::cout << "Test case ended" << std::endl;
	waitTimeout();
	std::vector<AbstractMonitor*>::iterator it;
	for( it = monitors.begin(); it!=monitors.end(); it++)
		(*it)->finish_test();
};

void TestCaseManager::sendNotificationPacketRead(){
	//std::cout << "Test case ended" << std::endl;
	std::vector<AbstractMonitor*>::iterator it;
	for( it = monitors.begin(); it!=monitors.end(); it++)
		(*it)->packet_read();
};

void TestCaseManager::sendNotificationPacketSent(){
	//std::cout << "Test case ended" << std::endl;
	std::vector<AbstractMonitor*>::iterator it;
	for( it = monitors.begin(); it!=monitors.end(); it++)
		(*it)->packet_sent();
};

State* TestCaseManager::getReport(){
	State* res = new OkState(0);
	std::vector<AbstractMonitor*>::iterator it;
	for( it = monitors.begin(); it!=monitors.end(); it++)
		(*res) << (*it)->getReport();

	return res;
};

void TestCaseManager::closeMonitor(){
//TODO	delete monitor;
};

void TestCaseManager::waitTimeout(){
	usleep(Configurator::getInstance()->getDelay() * 1000);
};
