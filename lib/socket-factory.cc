#include <socket-factory.h>
#include <udp-socket.h>
#include <tcp-socket.h>
#include <tcp-server-socket.h>
#include <raw-socket.h>
#include <configurator.h>

std::auto_ptr<SocketFactory> SocketFactory::_instance;

SocketFactory* SocketFactory::getInstance(){
	if(_instance.get() == 0)
		_instance = std::auto_ptr<SocketFactory>(new SocketFactory());
	return _instance.get();
};

SocketFactory::SocketFactory() : udp(0), udp_socket(0),
											tcp(0), tcp_socket(0),
											raw(0), raw_socket(0) {
	
};

SocketFactory::~SocketFactory(){
	//TODO cancellazione di tutti gli object rilasciati
	if(udp_socket)
		delete udp_socket;
	if(tcp_socket)
		delete tcp_socket;
	if(raw_socket)
		delete raw_socket;
};

Socket* SocketFactory::getSocket (){
	//TODO return a vector with all allocated sockets
	if(udp){
		return getUdpSocket();
	}else if(tcp){
		return getTcpSocket();
	}else if(raw){
		return getRawSocket();
	}else
		return NULL;
};

void SocketFactory::releaseSocket (){
	if(Configurator::getInstance ()->isUdp()){
		return releaseUdpSocket ();
	}else if(Configurator::getInstance ()->isTcp()){
		return releaseTcpSocket ();
	}
};

Socket* SocketFactory::getSocket(UdpSendAction* action){
	return getUdpSocket();
};

Socket* SocketFactory::getSocket(UdpReadAction* action){
	return getUdpSocket();
};

Socket* SocketFactory::getUdpSocket(){
	if(udp == 0)
		udp_socket = newUdpSocket();
	udp++;
	return udp_socket;
};

void SocketFactory::releaseSocket(UdpSendAction* action){
	releaseUdpSocket();
};

void SocketFactory::releaseSocket(UdpReadAction* action){
	releaseUdpSocket();
};


void SocketFactory::releaseUdpSocket(){
	udp--;
	if(udp <= 0 && udp_socket){
		udp = 0;
		delete udp_socket;
	}
};

Socket* SocketFactory::getSocket(TcpSendAction* action){
	return getTcpSocket();
};

Socket* SocketFactory::getSocket(TcpReadAction* action){
	return getTcpSocket();
};

Socket* SocketFactory::getTcpSocket(){
	if(tcp == 0)
		tcp_socket = newTcpSocket();
	tcp++;
	return tcp_socket;
};

void SocketFactory::releaseSocket(TcpSendAction* action){
	releaseTcpSocket();
};

void SocketFactory::releaseSocket(TcpReadAction* action){
	releaseTcpSocket();
};

void SocketFactory::releaseTcpSocket(){
	tcp--;
	if(tcp <= 0 && tcp_socket){
		tcp = 0;
		delete tcp_socket;
	}
};

Socket* SocketFactory::getSocket(RawSendAction* action){
	return getRawSocket();
};

Socket* SocketFactory::getSocket(RawReadAction* action){
	return getRawSocket();
};

Socket* SocketFactory::getRawSocket(){
	if(raw == 0)
		raw_socket = newRawSocket();
	raw++;
	return raw_socket;
};

void SocketFactory::releaseSocket(RawSendAction* action){
	releaseRawSocket();
};

void SocketFactory::releaseSocket(RawReadAction* action){
	releaseRawSocket();
};

void SocketFactory::releaseSocket(Socket* socket){
	if(socket == tcp_socket)
		releaseTcpSocket();
	else if(socket == udp_socket)
		releaseUdpSocket();
	else if(socket == raw_socket)
		releaseRawSocket();
};


void SocketFactory::releaseRawSocket(){
	raw--;
	if(raw <= 0 && raw_socket){
		raw = 0;
		delete raw_socket;
	}
};

void SocketFactory::reconnectSocket(){
	if(tcp)
		tcp_socket->reconnect();
	if(udp)
		udp_socket->reconnect();
	if(raw)
		raw_socket->reconnect();
};

void SocketFactory::releaseAllSocket (){
	if(udp_socket){
		delete udp_socket;
		udp_socket = 0;
	}
	if(tcp_socket){
		delete tcp_socket;
		tcp_socket = 0;
	}
	if(raw_socket){
		delete raw_socket;
		raw_socket = 0;
	}
};

Socket* SocketFactory::newTcpSocket(){
	bool server = Configurator::getInstance()->getListen();
	if(server){
		return new TcpServerSocket();
	}else{
		return new TcpSocket();
	}
};

Socket* SocketFactory::newUdpSocket(){
	return new UdpSocket();
};

Socket* SocketFactory::newRawSocket(){
	return new RawSocket();
};
