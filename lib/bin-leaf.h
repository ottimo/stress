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

#ifndef _BIN_LEAF_H_
#define _BIN_LEAF_H_

#include <leaf.h>

class BinLeaf: public Leaf 
{
	public:
		BinLeaf();
		BinLeaf(std::string s);
		BinLeaf(const BinLeaf& c);

		virtual Composite* clone(){return new BinLeaf(*this);};

		virtual State* runTest();
	protected:

	private:

};

#endif // _BIN_LEAF_H_
