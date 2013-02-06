#ifndef _SOCKET_H
#define _SOCKET_H

#include <vector>
#include <stdint.h>
#include <sys/select.h>

class Socket {
	public:
		Socket();
		virtual int send(std::vector<uint8_t>&)=0;
		virtual int read(std::vector<uint8_t>&)=0;
		virtual void reconnect()=0;
		virtual void resetTimeout();
		virtual bool check()=0;
	protected:

		virtual timeval* getTimeout();
	private:

	public:

	protected:

	private:
		timeval timeout;
};

#endif
