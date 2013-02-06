/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * trunk
 * Copyright (C) Matteo Rosi 2009 <matteo.rosi@gmail.com>
 * 
 * trunk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * trunk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tcp-read-action.h"
#include <socket-factory.h>
#include <composite.h>

TcpReadAction::TcpReadAction() : Command("<%TCP_read%>"){
	my_action = std::string("TcpReadAction");
	//socket = SocketFactory::getInstance()->getSocket(this);
};

TcpReadAction::TcpReadAction(Socket* s) : Command("<%TCP_read%>"){
	my_action = std::string("TcpReadAction");
	socket = s;
};


TcpReadAction::~TcpReadAction(){};

Action* TcpReadAction::clone(){ 
	return new TcpReadAction (SocketFactory::getInstance()->getSocket(this)); 
};
