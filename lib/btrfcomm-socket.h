#ifndef _BTRFCOMM_SOCKET_H
#define _BTRFCOMM_SOCKET_H

#include <socket.h>
extern "C" {
	#include <sys/socket.h>
	#include <bluetooth/bluetooth.h>
	#include <bluetooth/rfcomm.h>
}

class BTrfcommSocket : public Socket {
	public:
		BTrfcommSocket();
		~BTrfcommSocket();
		virtual int send(std::vector<uint8_t>&);
		virtual int read(std::vector<uint8_t>&);
		virtual void reconnect();
		virtual bool check();
	protected:

	private:

	public:

	protected:

	private:

	int socket, status;
	struct sockaddr_rc addr; //kernel assigned
/*		boost::asio::io_service io_service;
		boost::asio::ip::address to_address;
		boost::asio::ip::tcp::endpoint receiver_endpoint;
		boost::asio::ip::tcp::endpoint sender_endpoint;
		boost::asio::ip::tcp::socket socket;*/
};

#endif
