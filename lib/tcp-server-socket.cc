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

#include "tcp-server-socket.h"
#include <iostream>
#include <configurator.h>

TcpServerSocket::TcpServerSocket() :	
	io_service(0), 
	to_address(), 
	receiver_endpoint(), 
	sender_endpoint(), 
	socket(io_service) {
		std::cout << "TcpServerSocket" << std::endl;
	try{
		resetTimeout ();
		to_address = boost::asio::ip::address::from_string(Configurator::getInstance()->getDestinationIp());

		//receiver_endpoint.address(boost::asio::ip::tcp::v4());
		receiver_endpoint.port(Configurator::getInstance()->getPort());

		//boost::asio::ip::tcp::acceptor acceptor(io_service);
		//acceptor = boost::asio::ip::tcp::acceptor(io_service, receiver_endpoint);
		//acceptor.open(receiver_endpoint.protocol());
		//acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		//acceptor.bind(receiver_endpoint);
		//acceptor.listen(0);
		//acceptor.accept(socket);
		//acceptor.close();

		//socket.open(boost::asio::ip::tcp::v4());
		/*try{
			boost::asio::socket_base::linger option(true, 2);
			socket.set_option(option);
		} catch (std::exception& e) {
			std::cerr << "TcpSocket::TcpSocket(): Cannot set Timeout because: " << e.what() << std::endl;
		}

		socket.connect(receiver_endpoint); */
	} catch (std::exception& e) {
		std::cerr << "TcpServerSocket::TcpServerSocket(): " << e.what() << std::endl;
		exit(-1);
	}					
};

TcpServerSocket::~TcpServerSocket(){
	socket.close();
	io_service.reset();
	delete &socket;
	delete &io_service;
};

void TcpServerSocket::reconnect(){
	try{
		if (check()){
			socket.close();
			io_service.reset();
		}
		boost::asio::ip::tcp::acceptor acceptor(io_service, receiver_endpoint);

		//boost::asio::socket_base::non_blocking_io command(false);
		//socket.io_control(command);
		
		//socket.open(boost::asio::ip::tcp::v4());
		//socket.connect(receiver_endpoint);
		//acceptor.open(receiver_endpoint.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		//acceptor.bind(receiver_endpoint);
		//acceptor.listen(0);
		acceptor.accept(socket);
		acceptor.close();
	} catch      (std::exception& e) {
		std::cerr<< "TcpServerSocket::reconnect(): " << e.what() << std::endl;
	}
};

int TcpServerSocket::send(std::vector<uint8_t>& data_to_send){
	int n= -1;
	try{
		n=socket.send(boost::asio::buffer(data_to_send));
	}catch(boost::system::system_error& se){
		std::cerr <<"TcpSocket::send(): "<< se.what() << std::endl;
	}
	return n;
};

int TcpServerSocket::read(std::vector<uint8_t>& data_to_read){
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
		 std::cerr << "TcpServerSocket::read(): " << se.what() << std::endl;
	  }
	  for(int i=0; i<2050; i++)
		 data_to_read.push_back(data[i]);
	  return len;
	}else
	  return -1;
};

bool TcpServerSocket::check(){
	return socket.is_open();
};