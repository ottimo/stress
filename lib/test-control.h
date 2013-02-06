#ifndef _TEST_CONTROL_H
#define _TEST_CONTROL_H

#include <memory>

class TestControl {
	public:

	~TestControl();

	static	TestControl*	getInstance();

				int 				getId();

	protected:

	TestControl();

	private:

	public:

	protected:

	private:

	static	std::auto_ptr<TestControl>	_instance;
				int								next_id;

};

#endif
