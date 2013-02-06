#include <tcp-socket.h>
#include <configurator.h>
#include <iostream>
#include <sys/select.h>

TcpSocket::TcpSocket() :
	Socket(),	
	io_service(0), 
	to_address(), 
	receiver_endpoint(), 
	sender_endpoint(), 
	socket(io_service) {
	try{
		to_address = boost::asio::ip::address::from_string(Configurator::getInstance()->getDestinationIp());

		receiver_endpoint.address(to_address);
		receiver_endpoint.port(Configurator::getInstance()->getPort());

		//boost::asio::socket_base::receive_buffer_size option(8192);
		//socket.set_option(option);

		//unsigned short timeout = 2;
		//option.name(SO_RCVTIMEO);
		//option.data(&timeout);
		//option.size(sizeof(unsigned short));
		//socket.set_option(option);

		//socket.open(boost::asio::ip::tcp::v4());
		try{
			boost::asio::socket_base::linger option(true, 2);
			socket.set_option(option);
		} catch (std::exception& e) {
			std::cerr << "TcpSocket::TcpSocket(): Cannot set Timeout because: " << e.what() << std::endl;
		}

		//socket.connect(receiver_endpoint);
	} catch (std::exception& e) {
		std::cerr << "TcpSocket::TcpSocket(): " << e.what() << std::endl;
		exit(-1);
	}
	try{
		boost::asio::socket_base::linger option; //(true, 2);
		socket.get_option(option);
		unsigned short timeout = option.timeout();
		std::cout << "Timeout set to " << timeout << std::endl;
	} catch (std::exception& e) {
		std::cerr << "TcpSocket::TcpSocket(): Cannot get Timeout because: " << e.what() << std::endl;
	}									
};

TcpSocket::~TcpSocket(){
	socket.close();
	io_service.reset();
	delete &socket;
	delete &io_service;
};

int TcpSocket::send(std::vector<uint8_t>& data_to_send){
	int n= -1;
	try{
		n=socket.send(boost::asio::buffer(data_to_send));
	}catch(boost::system::system_error& se){
		std::cerr <<"TcpSocket::send(): "<< se.what() << std::endl;
	}
	return n;
};
int TcpSocket::read(std::vector<uint8_t>& data_to_read){
	uint8_t data[2049];
	memset(data,0,2049);
	data_to_read.clear();
	size_t len = -1;
	if(!socket.is_open())
		return -1;
	
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket.native(), &fds);
	timeval* tv = getTimeout();

	int n = select(socket.native()+1, &fds, 0, 0, tv );
	if(n>0){
	  try{
		 len = socket.receive(boost::asio::buffer(data, 2048));
	  } catch (boost::system::system_error& se) {
		 std::cerr << "TcpSocket::read(): " << se.what() << std::endl;
	  }
	  for(int i=0; i<2050; i++)
		 data_to_read.push_back(data[i]);
	  return len;
	}else
	  return -1;
};

bool TcpSocket::check(){
	return socket.is_open();
};

void TcpSocket::reconnect(){
	try{
		if(check())
			socket.close();
		socket.open(boost::asio::ip::tcp::v4());
		socket.connect(receiver_endpoint);
	} catch      (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
};
