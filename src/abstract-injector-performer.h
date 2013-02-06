/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
stress is free software: you can redistribute it and/or modify it
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

#ifndef _ABSTRACT_INJECTOR_PERFORMER_H_
#define _ABSTRACT_INJECTOR_PERFORMER_H_

#include "composite.h"
#include "composite-factory.h"

class AbstractInjectorPerformer
{
	public:
		AbstractInjectorPerformer();
	    virtual Composite* inject(Composite* leaf);
		
	protected:

		virtual Composite* inject(Leaf* leaf);
		virtual Composite* inject(StringLeaf* leaf);
		virtual Composite* inject(HexLeaf* leaf);
		virtual Composite* inject(DecLeaf* leaf);
	    virtual Composite* inject(BinLeaf* leaf);
		
		virtual void addFaults(Leaf* orig, Composite* or_node, CompositeFactory* cfact)=0;

		virtual Composite* createFaults(Leaf* orig);
		
	    virtual std::string generateAnomalyName(Composite* orig);
	    virtual std::string trim (Composite* orig);
	    virtual std::string addQuote(std::string s);

		int anomalyCounter;
		int orCounter;
	private:

};

#endif // _ABSTRACT_INJECTOR_PERFORMER_H_
