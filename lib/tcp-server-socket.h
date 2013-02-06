/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
 * stress is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * stress is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TCP_SERVER_SOCKET_H_
#define _TCP_SERVER_SOCKET_H_

#include <tcp-socket.h>
#include <boost/asio.hpp>

class TcpServerSocket: public Socket 
{
	public:
		TcpServerSocket();
		~TcpServerSocket();
		virtual void reconnect();		
		virtual int read(std::vector<uint8_t>&);
		virtual int send(std::vector<uint8_t>&);
		virtual bool check();
	protected:

	private:
		boost::asio::io_service io_service;
		boost::asio::ip::address to_address;
		boost::asio::ip::tcp::endpoint receiver_endpoint;
		boost::asio::ip::tcp::endpoint sender_endpoint;
		boost::asio::ip::tcp::socket socket;
	//	boost::asio::ip::tcp::acceptor acceptor;
};

#endif // _TCP_SERVER_SOCKET_H_
