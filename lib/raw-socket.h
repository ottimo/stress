#ifndef _RAW_SOCKET_H
#define _RAW_SOCKET_H

#include <socket.h>

class RawSocket : public Socket {
	public:
		RawSocket();
                ~RawSocket();
		virtual int send(std::vector<uint8_t>&);
		virtual int read(std::vector<uint8_t>&);
		virtual void reconnect();
		virtual bool check();
	protected:

	private:

	public:

	protected:

	private:
            int sock;
};

#endif
