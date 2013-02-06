#ifndef _TCP_SOCKET_H
#define _TCP_SOCKET_H

#include <socket.h>
#include <boost/asio.hpp>

class TcpSocket : public Socket {
	public:
		TcpSocket();
		~TcpSocket();
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
		boost::asio::ip::address to_address;
		boost::asio::ip::tcp::endpoint receiver_endpoint;
		boost::asio::ip::tcp::endpoint sender_endpoint;
		boost::asio::ip::tcp::socket socket;
};

#endif
