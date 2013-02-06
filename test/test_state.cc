#include <test_state.h>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
 
CPPUNIT_TEST_SUITE_REGISTRATION( StateTest );

void StateTest::setUp(){
	 s = new State();
};
void StateTest::tearDown(){
	 if(s)
		delete s;
	 s = NULL;
};

void StateTest::testStringValue(){
	std::string s1 = std::string("ffff02");
	int int_result = atoi(s1.c_str());
	uint8_t buf[1024];
	
	memset(buf,0,1024);
	s->setStringValue(s1);
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0x02;


	CPPUNIT_ASSERT(s1 == s->getStringValue());
	CPPUNIT_ASSERT_EQUAL(int_result , s->getIntegerValue());
	CPPUNIT_ASSERT(buf[0] == s->getRawValue()[0]);
	CPPUNIT_ASSERT(buf[1] == s->getRawValue()[1]);
	CPPUNIT_ASSERT(buf[2] == s->getRawValue()[2]);

};
void StateTest::testStringValue2(){
	std::string s1 = std::string("02");
	int int_result = atoi(s1.c_str());
	uint8_t buf[1024];
	
	memset(buf,0,1024);
	s->setStringValue(s1);
	buf[0] = 0x02;


	CPPUNIT_ASSERT(s1 == s->getStringValue());
	CPPUNIT_ASSERT_EQUAL(int_result , s->getIntegerValue());
	CPPUNIT_ASSERT_EQUAL(2 , s->getIntegerValue());
	CPPUNIT_ASSERT(buf[0] == s->getRawValue()[0]);
};

void StateTest::testIntValue(){
	int i = 34;
	s->setIntegerValue(i);

	const uint8_t* buf = s->getRawValue();
	CPPUNIT_ASSERT_EQUAL(i ,s->getIntegerValue());
	CPPUNIT_ASSERT(s->getStringValue() == "34");
	CPPUNIT_ASSERT(buf[0] == 34);
	CPPUNIT_ASSERT(buf[1] == 0);
	CPPUNIT_ASSERT(buf[2] == 0);
	CPPUNIT_ASSERT(buf[3] == 0);
};


void StateTest::testRawValue(){
	uint8_t buf1[4];
	memset(buf1,0,4);
	buf1[0] = 0xff;
	buf1[1] = 0x00;
	buf1[2] = 0x02;
	buf1[3] = 0x00;
	s->setRawValue(buf1, 4);

	const uint8_t* buf = s->getRawValue();

	int* ip = (int*)buf;

	CPPUNIT_ASSERT(buf[0] == 0xff);
	CPPUNIT_ASSERT(buf[1] == 0x00);
	CPPUNIT_ASSERT(buf[2] == 0x02);
	CPPUNIT_ASSERT(buf[3] == 0x00);

	std::cout << *ip << std::endl;
	CPPUNIT_ASSERT_EQUAL(*ip ,s->getIntegerValue());

	std::string s1 = s->getStringValue();
	const unsigned char* cp = (const unsigned char*) s1.c_str();
	printf("%.2x ",cp[0]);
	printf("%.2x ",cp[1]);
	printf("%.2x ",cp[2]);
	printf("%.2x ",cp[3]);
	
	
	CPPUNIT_ASSERT(cp[0] == 0xff);
	CPPUNIT_ASSERT(cp[1] == 0x00);
	CPPUNIT_ASSERT(cp[2] == 0x02);
	CPPUNIT_ASSERT(cp[3] == 0x00);
};
