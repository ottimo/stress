#ifndef _OR_COMPOSITE_H
#define _OR_COMPOSITE_H


#include <composite.h>

/* 
	TODO getAction si comporta in modo diverso

 	se la prima leaf a sinistra è una send la or si deve comportare in modo simil anomalia.
 	se la prima leaf a sinistra è una read si comporta da or logico
 	se la prima leaf a sinistra è una return allora sono un'anomalia
 */

class OrComposite : public Composite{
 
public:
	
	OrComposite();
	OrComposite(std::string s);
	OrComposite(const OrComposite& c);

	Composite* clone();	
	virtual Action* getAction();
private:

};


#endif
