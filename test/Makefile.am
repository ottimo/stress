# Rules for the test code (use `make check` to execute)
TESTS = TestStress
check_PROGRAMS = $(TESTS)
TestStress_SOURCES = ../lib/state.h ../lib/state.cc test_state.h test_state.cc main.cc
TestStress_CXXFLAGS = $(CPPUNIT_CFLAGS) -I../lib
TestStress_LDFLAGS = $(CPPUNIT_LIBS) -ldl
