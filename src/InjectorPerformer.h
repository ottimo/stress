/* 
 * File:   InjectorPerformer.h
 * Author: Lu
 *
 * Created on 26 aprile 2010, 16.54
 */

#ifndef _INJECTORPERFORMER_H
#define	_INJECTORPERFORMER_H

#include "composite.h"
#include "composite-factory.h"
#include "abstract-injector-performer.h"


class InjectorPerformer : public AbstractInjectorPerformer{

public:
    InjectorPerformer();

    virtual ~InjectorPerformer();

    virtual Composite* inject(StringLeaf* leaf); // Mandatory

protected:
	virtual void addFaults(Leaf* orig, Composite* or_node, CompositeFactory* cfact); //Mandatory    
	
    Composite* stringInjectionNONASCII(StringLeaf* orig, CompositeFactory* cf);
    Composite* stringInjectionMIDDLETERM(StringLeaf* orig, CompositeFactory* cf, std::string term);
    Composite* stringInjectionVERYLONG(StringLeaf* orig, CompositeFactory* cf, int l);
    Composite* stringInjectionRANDOMFUZZ(StringLeaf* orig, CompositeFactory* cf);
    Composite* stringInjectionNULLSTRING(StringLeaf* orig, CompositeFactory* cf);
};

#endif	/* _INJECTORPERFORMER_H */

