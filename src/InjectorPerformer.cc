/* 
 * File:   InjectorPerformer.cc
 * Author: Lu
 * 
 * Created on 26 aprile 2010, 16.54
 */

#include "InjectorPerformer.h"
#include <leaf.h>
#include <string-leaf.h>
#include <bin-leaf.h>
#include <hex-leaf.h>
#include <dec-leaf.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#define VERYLONG_ 512

InjectorPerformer::InjectorPerformer(): AbstractInjectorPerformer(){}

InjectorPerformer::~InjectorPerformer() {}

Composite* InjectorPerformer::inject(StringLeaf* leaf) {
//    if (injectStringLeaves) {
        std::cout << "Injecting StringLeaf" << leaf->getName() << std::endl;
        return createFaults(leaf);
//    } else
//        return (Composite*) leaf->clone();
}

/*
Composite* InjectorPerformer::inject(Leaf* leaf) {
    std::cout << "Injecting Leaf" << leaf->getName() << std::endl;
    return (Composite*) leaf->clone();
}

 Composite* InjectorPerformer::inject(HexLeaf* leaf) {
    std::cout << "Injecting HexLeaf" << leaf->getName() << std::endl;
    if (injectHexLeaves) {
        return (Composite*) leaf->clone(); //TODO
    } else
        return (Composite*) leaf->clone();

}

Composite* InjectorPerformer::inject(DecLeaf* leaf) {
    std::cout << "Injecting DecLeaf" << leaf->getName() << std::endl;
    if (injectDecLeaves) {
        return (Composite*) leaf->clone(); //TODO
    } else
        return (Composite*) leaf->clone();
}

Composite* InjectorPerformer::inject(BinLeaf* leaf) {
    std::cout << "Injecting BinLeaf" << leaf->getName() << std::endl;
    if (injectBinLeaves) {
        return (Composite*) leaf->clone(); //TODO
    } else
        return (Composite*) leaf->clone();
}
*/

void InjectorPerformer::addFaults(Leaf* orig, Composite* or_node, CompositeFactory* cfact) {
	if (!dynamic_cast<StringLeaf*> (orig))
		return;
	StringLeaf* sorig = dynamic_cast<StringLeaf*> (orig);
    (*or_node) << stringInjectionMIDDLETERM(sorig, cfact, std::string("%x00"));
    (*or_node) << stringInjectionMIDDLETERM(sorig, cfact, std::string("%x0a"));
    (*or_node) << stringInjectionMIDDLETERM(sorig, cfact, std::string("%x0d"));
    (*or_node) << stringInjectionVERYLONG(sorig, cfact, VERYLONG_);
    (*or_node) << stringInjectionNONASCII(sorig, cfact);
    (*or_node) << stringInjectionRANDOMFUZZ(sorig, cfact);
    (*or_node) << stringInjectionNULLSTRING(sorig, cfact);
}

Composite* InjectorPerformer::stringInjectionNONASCII(StringLeaf* orig, CompositeFactory* cf) {
    Composite * anomaly = cf->getNode(generateAnomalyName(orig));

    std::string origName = trim(orig);

    std::ostringstream ooss(std::ostringstream::out);

    ooss << "%x";
    for (int i = 0; i < origName.size(); i++)
        ooss << std::hex << ((int) (128 + rand() % 128));

    (*anomaly) << (cf->getHexNode(ooss.str()));

    return anomaly;

}

Composite* InjectorPerformer::stringInjectionMIDDLETERM(StringLeaf* orig, CompositeFactory* cf, std::string term) {

    Composite * anomaly = cf->getNode(generateAnomalyName(orig));
    std::string origName = trim(orig);
	int insertPoint = 0;
	if(origName.size()>1)
	    insertPoint = 1 + (rand() % (origName.size() - 1));
	else if(origName.size()==1)
		insertPoint = 1;
	else{ 
		(*anomaly) << (cf->getStringNode(addQuote(origName)));
		return anomaly;
	}
    std::cout << "Insert Point is " << insertPoint << " out of " << origName.size() << std::endl;

    (*anomaly) << (cf->getStringNode(addQuote(origName.substr(0, insertPoint))));
    (*anomaly) << (cf->getHexNode(term));
    (*anomaly) << (cf->getStringNode(addQuote(origName.substr(insertPoint))));

    return anomaly;
}

Composite* InjectorPerformer::stringInjectionVERYLONG(StringLeaf* orig, CompositeFactory* cf, int lenght) {
    Composite * anomaly = cf->getNode(generateAnomalyName(orig));

    std::string origName = trim(orig);

    std::ostringstream ooss(std::ostringstream::out);

    ooss << origName;

    while (ooss.str().size() < lenght)
        ooss << origName;

    (*anomaly) << (cf->getStringNode(addQuote(ooss.str())));

    return anomaly;
}

Composite* InjectorPerformer::stringInjectionRANDOMFUZZ(StringLeaf* orig, CompositeFactory* cf) {
    Composite * anomaly = cf->getNode(generateAnomalyName(orig));

    std::string origName = trim(orig);

    std::ostringstream ooss(std::ostringstream::out);

    ooss << "%x";
    for (int i = 0; i < origName.size(); i++)
        ooss << std::hex << ((int) (rand() % 128));

    (*anomaly) << (cf->getHexNode(ooss.str()));

    return anomaly;

}

Composite* InjectorPerformer::stringInjectionNULLSTRING(StringLeaf* orig, CompositeFactory* cf) {

    Composite * anomaly = cf->getNode(generateAnomalyName(orig));

    (*anomaly) << (cf->getStringNode(addQuote(std::string(""))));
    return anomaly;
}


