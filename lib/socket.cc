#include <socket.h>
#include <configurator.h>

Socket::Socket(){
	resetTimeout();
};

timeval* Socket::getTimeout(){
	return &timeout;
};

void Socket::resetTimeout(){
	int usec = Configurator::getInstance()->getTimeout();	
	timeout.tv_sec = usec / 1000;
	timeout.tv_usec = 1000 * (usec % 1000);
};
