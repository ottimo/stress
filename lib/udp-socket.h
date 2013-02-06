#ifndef _UDP_SOCKET_H
#define _UDP_SOCKET_H

#include <socket.h>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

class UdpSocket : public Socket {
	public:
		UdpSocket();
		virtual int send(std::vector<uint8_t>&);
		virtual int read(std::vector<uint8_t>&);
		virtual void reconnect();
		virtual bool check();
	protected:

	private:

	public:

	protected:

	private:
		boost::asio::io_service io_service;
		boost::asio::ip::udp::endpoint receiver_endpoint;
		boost::asio::ip::udp::endpoint sender_endpoint;
		boost::asio::ip::udp::socket socket;
};

#endif
