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

#ifndef _REPEAT_COMMAND_H_
#define _REPEAT_COMMAND_H_

#include <action.h>
#include <command.h>

class RepeatCommand: public Action, Command{
	public:
		RepeatCommand();
		~RepeatCommand();
		Action* clone();
		virtual State* runAction(Composite*);
	protected:

	private:

};

static RepeatCommand* repeat_command = new RepeatCommand();

#endif // _REPEAT_COMMAND_H_
