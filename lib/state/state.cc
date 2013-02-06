#include <state.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string.h>

State::State() :	raw_value(NULL), 
						string_value(""),
						int_value(0),	
						raw_lenght(0),
						my_type("State"),
						state_value(OK)
{};

State::State(int id) :	raw_value(NULL), 
						string_value(""),
						int_value(0),	
						raw_lenght(0),
						state_value(OK),
						composite_id(composite_id),
						my_name("State"),
						my_type("State")
{};

State::State(State& s): raw_value(s.raw_value), 
								string_value(s.string_value),
								int_value(s.int_value),	
								state_value(s.state_value),
								composite_id(s.composite_id),
								my_name("State"),
								my_type("State")
{};

State::~State(){
	std::vector<State*>::iterator sons_iterator;
	for(sons_iterator = derived_state.begin(); sons_iterator != derived_state.end(); sons_iterator++){
		delete (*sons_iterator);
	}
	resetValues();
	derived_state.clear();
};


void State::printTestTree(std::ostringstream* oss){
	std::vector<State*>::iterator sons_iterator;
	for(sons_iterator = derived_state.begin(); sons_iterator != derived_state.end(); sons_iterator++){
		*oss << getId() << "[label=\"" <<  getName() << " \"];" << std::endl;
		*oss << (*sons_iterator)->getId() << "[label=\"" << (*sons_iterator)->getName();
		*oss << "\"]" << std::endl; 
		
		*oss << getId() << "->" << (*sons_iterator)->getId()  << " ;" << std::endl;
		(*sons_iterator)->printTestTree(oss);
	}
};


std::string State::getStringValue(){
	return string_value;
};

const uint8_t* State::getRawValue(){
	return raw_value;
};

int State::getRawLenght(){
	return raw_lenght;
};

int State::getIntegerValue(){
	return int_value;
};

void State::setStringValue(std::string s){
	resetValues();
	
	string_value = s;

	if(s[0] == '%')
		s.erase(0,2);

	const char* ccp = s.c_str();
	int length = s.length()/2 + s.length()%2;
	uint8_t* u8p = new uint8_t[length];
	raw_lenght = length;
	uint8_t* buf = u8p;
	memset(u8p, '\0', length);
	
	for(int i=0; i<length; i++){
		std::istringstream iss(s.substr(i*2, 2));
		unsigned int n;
		iss >> std::hex >> n;
		buf[0] = n;
		buf++;
	}

	raw_value = const_cast<uint8_t*>(u8p);
	int_value = atoi(ccp);
};

void State::setRawValue(uint8_t* u8p, int i){
	resetValues();
	
	//set raw value
	uint8_t* ptr = new uint8_t[i+1];
	for(int n=0; n<i; n++)
		ptr[n] = u8p[n];
	raw_value = const_cast<uint8_t*>(ptr);
	raw_lenght = i;

	//set string value
	std::stringstream ss;
	ss.write((const char*)u8p, i);
	string_value.clear();
	unsigned char ch;
	for(int n=0; n<i; n++){
		 ss >> ch;
		 string_value += ch;
	}
	//set integer value
	int_value = *(int*)u8p;
};

void State::setIntegerValue(int i){
	resetValues();
	
	// set integer value
	int_value = i;

	// set string value
	std::stringstream ss;
	ss << i;
	string_value.clear();
	string_value += ss.str();

	//set raw value
	uint8_t* ptr = (uint8_t*)(&i);
	uint8_t* u8p = new uint8_t[sizeof(int)];

	for(int n=0; n<sizeof(int); n++)
		u8p[n] = ptr[n];
	raw_value = const_cast<uint8_t*>(u8p);
	raw_lenght = sizeof(int);
};

const State& State::operator=(const State& s){
	raw_value = s.raw_value;
	string_value = s.string_value;
	int_value = s.int_value;
	state_value = s.state_value;
	my_name = s.my_name;
	my_type = s.my_type;
	return *this;
};

void State::operator<<(State* s){
	this->derived_state.push_back(s);
};

void State::operator<<(std::vector<State*>& v){
	std::vector<State*>::iterator it;
	for(it = v.begin(); it != v.end(); it++){
		this->derived_state.push_back( (*it) );
	}
};

std::string State::getStringToSend(){
	std::string ret;
	std::vector<State*>::iterator it;

	for(it = getSons().begin(); it!= getSons().end(); it++)
		ret += (*it)->getStringToSend();
	return ret;	
};

std::string State::getValueForLog(){
	//std::cout << "State::getValueForLog()" << std::endl;
	std::stringstream ss;
	std::vector<State*>::iterator it;
	for(it = derived_state.begin(); it!=derived_state.end(); it++)
		ss << (*it)->getValueForLog();
	return ss.str();
};

void State::setAbnfString(std::string str){
	int it= 0;
	if(str[0]=='\\' && str[1]=='"')
		str.erase(0,2);
	if(str[str.size()-2]=='\\' && str[str.size()-1]=='"')
		str.erase(str.size()-2,2);
	while( ( it = str.find("\\\"") ) != std::string::npos ){
		str.erase(it, 1);
	}
	string_value = str;
	setStringValue (str);
};
void State::setAbnfHex(std::string str){
	int it=str.find("%x");
	if(it == 0)
		str.erase(0,2);
	setStringValue (str);
	//setRawValue ((uint8_t*) str.c_str(), str.size());
};
void State::setAbnfInt(std::string str){
	int it=str.find("%d");
	if(it == 0)
		str.erase(0,2);
	setIntegerValue (atoi(str.c_str()));
};
void State::setAbnfBin(std::string){

};

void State::resetValues (){
	if(raw_lenght>0){
		raw_lenght = 0;
		delete [] raw_value;
		raw_value = NULL;
	}
	string_value.clear();
	int_value = 0;
};

