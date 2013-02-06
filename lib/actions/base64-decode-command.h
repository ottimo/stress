/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * trunk
 * Copyright (C) Matteo Rosi 2010 <matteo.rosi@gmail.com>
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

#ifndef _BASE64_DECODE_COMMAND_H_
#define _BASE64_DECODE_COMMAND_H_

#include <action.h>
#include <command.h>

class Base64DecodeCommand: public Action, Command{
	public:
		Base64DecodeCommand();
		~Base64DecodeCommand();
		Action* clone();
		virtual State* runAction(Composite*);
	protected:

	private:
		std::string base64_decode(std::string const& s);
		static inline bool is_base64(unsigned char c) {
			return (isalnum(c) || (c == '+') || (c == '/'));
		}
};

static Base64DecodeCommand* base64_decode_command = new Base64DecodeCommand();

#endif // _SKEL_COMMAND_H_
