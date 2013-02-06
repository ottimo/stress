#include <monitor.h>
#include <iostream>
#include <configurator.h>
#include <vector>
#include <monitor-report.h>

Monitor::Monitor() : 		io_service(), to_address(), receiver_endpoint(), 
									sender_endpoint(), socket(io_service) 
{
	try{
		to_address = boost::asio::ip::address::from_string(Configurator::getInstance()->getDestinationIp());

		receiver_endpoint.address(to_address);
		receiver_endpoint.port(60000);

		//boost::asio::socket_base::receive_buffer_size option(8192);
		//socket.set_option(option);


		//unsigned short timeout = 2;
		//option.name(SO_RCVTIMEO);
		//option.data(&timeout);
		//option.size(sizeof(unsigned short));
		//socket.set_option(option);

		socket.open(boost::asio::ip::tcp::v4());
		/* 		try{
			boost::asio::socket_base::linger option(true, 2);
			socket.set_option(option);
	} catch (std::exception& e) {
		std::cerr << "TcpSocket::TcpSocket(): Cannot set Timeout because: " << e.what() << std::endl;
	}
	*/
		socket.connect(receiver_endpoint);

		char data[2048];
		memset(data,0,2048);							
		while( std::string(data).find("Ready") != std::string::npos ){				
			try{
				//boost::asio::read(socket,
				//							boost::asio::buffer(data,2047));
				socket.receive(boost::asio::buffer(data, 2047));
				std::cout<<"Monitor::Monitor() receive: " << data << std::endl;  
			} catch (boost::system::system_error& se) {
				std::cerr << "TcpSocket::read(): " << se.what() << std::endl;
			}
		}
	} catch (std::exception& e) {
		std::cerr << "Monitor::Monitor(): " << e.what() << std::endl;
                exit(-1);
	}
};

Monitor::~Monitor(){
	socket.close();
};

void Monitor::start_test(){
	std::string start_cmd("start\n");
	char data[2048];
	memset(data,0,2048);

	try{
		socket.send(boost::asio::buffer(start_cmd));
	}catch(boost::system::system_error& se){
		std::cerr <<"Monitor::start_test() send(): "<< se.what() << std::endl;
	}

	while( std::string(data).find("Ok") == std::string::npos ){
		memset(data,0,2048);
		std::cout << " waiting Ok ";
		try{
			socket.receive(boost::asio::buffer(data, 2047));
			std::cout<<"Monitor::start_test() receive: " << data << std::endl;  
		} catch (boost::system::system_error& se) {
			std::cerr << "Monitor::start_test() read(): " << se.what() << std::endl;
		}
	}
	std::cout << "Ok received " << std::endl;
	
};

void Monitor::finish_test(){
	std::string start_cmd("stop\n");
	char data[2048];
	memset(data,0,2048);

	try{
		socket.send(boost::asio::buffer(start_cmd));
	}catch(boost::system::system_error& se){
		std::cerr <<"Monitor::finish_test() send(): "<< se.what() << std::endl;
	}
	while( std::string(data).find("Ok") == std::string::npos ){
		memset(data,0,2048);
		try{
			socket.receive(boost::asio::buffer(data, 2047));
			std::cout<<"Monitor::finish_test() receive: " << data << std::endl;  
		} catch (boost::system::system_error& se) {
			std::cerr << "Monitor::finish_test() read(): " << se.what() << std::endl;
		}
	}
};

State* Monitor::getReport(){
	State* res = new MonitorReport(0);
	res->setName("Report");
	std::string s;	
	std::string start_cmd("getreport\n");
	char data[2048];
	memset(data,0,2048);

	try{
		socket.send(boost::asio::buffer(start_cmd));
	}catch(boost::system::system_error& se){
		std::cerr <<"Monitor::start_test() send(): "<< se.what() << std::endl;
	}
	try{
		socket.receive(boost::asio::buffer(data, 2047));
		std::cout<<"Monitor::getReport() receive: " << data << std::endl; 
	} catch (boost::system::system_error& se) {
		std::cerr << "Monitor::getReport() read(): " << se.what() << std::endl;
	}

	s = data;
	
	res->setStringValue(s);
	return res;
};

void Monitor::packet_read(){
    std::string start_cmd("read\n");
    char data[2048];
    memset(data,0,2048);

    try{
	socket.send(boost::asio::buffer(start_cmd));
    }catch(boost::system::system_error& se){
	std::cerr <<"Monitor::finish_test() send(): "<< se.what() << std::endl;
    }
//    while( std::string(data).find("Ok") == std::string::npos ){
//	memset(data,0,2048);
//	try{
//		socket.receive(boost::asio::buffer(data, 2047));
//		std::cout<<"Monitor::finish_test() receive: " << data << std::endl;
//	} catch (boost::system::system_error& se) {
//        	std::cerr << "Monitor::finish_test() read(): " << se.what() << std::endl;
//    	}
//    }
};