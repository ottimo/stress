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

#include "abstract-injector-performer.h"
#include <leaf.h>
#include <string-leaf.h>
#include <bin-leaf.h>
#include <hex-leaf.h>
#include <dec-leaf.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

AbstractInjectorPerformer::AbstractInjectorPerformer(){
	anomalyCounter = 0;
    orCounter = 0;
	srand(time(NULL));
};

Composite* AbstractInjectorPerformer::createFaults(Leaf* orig){
	if(trim(orig).size() == 0)
		return orig;
    CompositeFactory* cfact = new CompositeFactory();
    std::ostringstream ooss(std::ostringstream::out);
    ooss << "or" << orCounter;

    Composite* orc = cfact->getOrNode(ooss.str());
    orCounter++;

    (*orc) << (orig->clone());
	
	addFaults(orig, orc, cfact);

	delete cfact;
	return orc;
};

Composite* AbstractInjectorPerformer::inject(Composite* comp) {

    Composite* c;

    if (dynamic_cast<StringLeaf*> (comp))
        c = inject(dynamic_cast<StringLeaf*> (comp));
    else if (dynamic_cast<HexLeaf*> (comp))
        c = inject(dynamic_cast<HexLeaf*> (comp));
    else if (dynamic_cast<DecLeaf*> (comp))
        c = inject(dynamic_cast<DecLeaf*> (comp));
    else if (dynamic_cast<BinLeaf*> (comp))
        c = inject(dynamic_cast<BinLeaf*> (comp));
    else
        c = comp->clone();

    return c;
}

Composite* AbstractInjectorPerformer::inject(Leaf* leaf){
	return (Composite*) leaf->clone();
};
Composite* AbstractInjectorPerformer::inject(HexLeaf* leaf){
	return (Composite*) leaf->clone();
};
Composite* AbstractInjectorPerformer::inject(StringLeaf* leaf){
	return (Composite*) leaf->clone();
};
Composite* AbstractInjectorPerformer::inject(DecLeaf* leaf){
	return (Composite*) leaf->clone();
};
Composite* AbstractInjectorPerformer::inject(BinLeaf* leaf){
	return (Composite*) leaf->clone();
};

std::string AbstractInjectorPerformer::generateAnomalyName(Composite* orig) {

    std::ostringstream ostr(std::ostringstream::out);

    std::string origName = trim(orig);

    ostr << "anomaly";
    ostr << anomalyCounter << "";
    //ostr << origName;

    anomalyCounter++;
    return ostr.str();
}

std::string AbstractInjectorPerformer::trim(Composite* orig) {
    std::string origName = orig->getName();
    origName = origName.erase(0, (origName.find_first_of("\"") + 1));
    origName = origName.erase(origName.find_last_of("\\"));
    return origName;
}

std::string AbstractInjectorPerformer::addQuote(std::string s) {
    std::ostringstream ooss(std::ostringstream::out);
    ooss << "\\\"" << s << "\\\"";
    return ooss.str();
}