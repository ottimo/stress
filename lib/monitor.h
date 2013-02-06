#ifndef _MONITOR_H
#define _MONITOR_H

#include <boost/asio.hpp>
#include <state.h>
#include <abstract-monitor.h>

class Monitor : public AbstractMonitor{
	private:	

		
	protected:

	public:

		Monitor();
		~Monitor();

//		int read();
//		int send();
		State* getReport();
		void start_test();
		void finish_test();
                void packet_read();
	private:
		boost::asio::io_service 				io_service;
		boost::asio::ip::address 			to_address;
		boost::asio::ip::tcp::endpoint 	receiver_endpoint;
		boost::asio::ip::tcp::endpoint 	sender_endpoint;
		boost::asio::ip::tcp::socket 		socket;
};

#endif
