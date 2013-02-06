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

#include "send-packet-state.h"

SendPacketState::SendPacketState(int i) : ValueStringState(i){
	composite_id = i;
	my_name = std::string("SendPacketState");
	my_type = std::string("SendPacketState");
	state_value = OK;
};

SendPacketState::SendPacketState(int i,int n) : ValueStringState(i,n){
	composite_id = i;
	my_name = std::string("SendPacketState");
	my_type = std::string("SendPacketState");
	state_value = FAULT_VALUE;
};

