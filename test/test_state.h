#ifndef _TEST_STATE_H
#define _TEST_STATE_H

#include <state.h>
#include <cppunit/extensions/HelperMacros.h>

class StateTest : public CppUnit::TestFixture{
	 CPPUNIT_TEST_SUITE(StateTest);
	 CPPUNIT_TEST(testStringValue);
	 CPPUNIT_TEST(testStringValue2);
	 CPPUNIT_TEST(testIntValue);
	 CPPUNIT_TEST(testRawValue);
	 CPPUNIT_TEST_SUITE_END();
	 private:

		  State* s;

	 public:

		  void setUp();
		  void tearDown();
		  void testStringValue();
		  void testStringValue2();
		  void testIntValue();
		  void testRawValue();
};

#endif
