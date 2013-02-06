#include <test-control.h>

std::auto_ptr<TestControl> TestControl::_instance;

TestControl::TestControl() : next_id(0){};

TestControl::~TestControl(){};

TestControl* TestControl::getInstance(){
	if(_instance.get() == 0){
		std::auto_ptr<TestControl> tmp(new TestControl());
		_instance = tmp;
	}
	return _instance.get();
};

int	TestControl::getId(){
	return next_id++;
};
