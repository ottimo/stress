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

#include "md5-command.h"
#include <value-hex-state.h>
#include <openssl/md5.h>
#include <composite.h>

MD5Command::MD5Command() : Command("<%MD5%>"){
	my_action = std::string("MD5Command");
};

MD5Command::~MD5Command(){};

Action* MD5Command::clone(){ 
	return new MD5Command(); 
};

State* MD5Command::runAction(Composite* c){
	// this method implements action
	std::vector<State*> result = runSonsAction(c);
	std::string input;
	std::vector<State*>::iterator it;
	unsigned char digest[MD5_DIGEST_LENGTH];

	for(it=result.begin(); it!=result.end(); it++)
		input += (*it)->getStringToSend();
	MD5((const unsigned char*)input.c_str(), input.size(), digest);
	ValueHexState* res = new ValueHexState(c->getId());
	*res << result;
	res->setRawValue(digest, MD5_DIGEST_LENGTH);
	return res;
};
