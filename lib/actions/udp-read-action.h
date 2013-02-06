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

#ifndef _UDP_READ_ACTION_H_
#define _UDP_READ_ACTION_H_

#include <read-action.h>
#include <state.h>
#include <socket.h>
#include <command.h>

class UdpReadAction: public ReadAction,Command {
	public:
		UdpReadAction();
		UdpReadAction(Socket*);
		~UdpReadAction();
		Action* clone();
	protected:

	private:
		
};

static UdpReadAction* udp_read_action = new UdpReadAction();

#endif // _UDP_READ_ACTION_H_
