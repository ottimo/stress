#include <udp-socket.h>
#include <configurator.h>
#include <sys/select.h>

UdpSocket::UdpSocket() : 	Socket(), io_service(), receiver_endpoint(), 
									sender_endpoint(), socket(io_service) {
	try{
		boost::asio::ip::address to_address;
		to_address = boost::asio::ip::address::from_string(Configurator::getInstance()->getDestinationIp());

		receiver_endpoint.address(to_address);
		receiver_endpoint.port(Configurator::getInstance()->getPort());
	
		socket.open(boost::asio::ip::udp::v4());
		int sock = socket.native();
		timeval tv;
		memset( &tv, 0, sizeof(tv) );
		tv.tv_sec = 1;
		tv.tv_usec = 1000;
		setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv) );
		setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv) );
	} catch (std::exception& e) {
		std::cerr << "UdpSocket::UdpSocket(): "<< e.what() << std::endl;
		exit(-1);
	}
};

int UdpSocket::send(std::vector<uint8_t>& data_to_send){
	int n=-1;
	n=socket.send_to(boost::asio::buffer(data_to_send), receiver_endpoint);
	return n;
};

int UdpSocket::read(std::vector<uint8_t>& data_to_read){
	uint8_t data[2049];
	memset(data,0,2049);
	data_to_read.clear();
	size_t len = -1;
	boost::system::error_code error;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket.native(), &fds);
	timeval* tv = getTimeout();
	int n = select(socket.native()+1, &fds, 0, 0, tv );

	if(n>0){
		try{
			len = socket.receive_from(
			    boost::asio::buffer(data, 2048), sender_endpoint, 0, error);
		} catch (boost::system::system_error& se) {
			std::cerr << se.what();
		}
		for(int i=0; i<2050; i++)
			data_to_read.push_back(data[i]);
		return len;
	}else
		return -1;
};

void UdpSocket::reconnect(){
	srand(time(NULL));
	sender_endpoint = socket.local_endpoint();
	sender_endpoint.port(rand()%60000+1024);
	try{
		socket.close();
	} catch      (std::exception& e) {
		std::cerr << "UdpSocket::reconnect(): socket.close(): " << e.what() << std::endl;
	}
	try{
		socket.open(boost::asio::ip::udp::v4());
	} catch      (std::exception& e) {
		std::cerr << "UdpSocket::reconnect(): socket.open(): " << e.what() << std::endl;
	}
	try{
		socket.bind(sender_endpoint);
	} catch      (std::exception& e) {
		std::cerr << "UdpSocket::reconnect(): socket.bind(): " << e.what() << std::endl;
	}
};

bool UdpSocket::check(){
	struct msghdr msg;
	int sock = socket.native();
	int n=-1;
	n=write(sock, NULL, 0);
	recvmsg(sock, &msg, 0);
	return true;
};
