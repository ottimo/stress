#include <ok-state.h>

OkState::OkState(int i){
	composite_id = i;
	my_name = std::string("OKState");
	my_type = std::string("OKState");
	state_value = OK;
};

//OkState::~OkState(){};
