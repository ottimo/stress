#ifndef _REPEAT_COMPOSITE_H
#define _REPEAT_COMPOSITE_H

#include <composite.h>

class RepeatComposite : public Composite{
 
public:
	
	RepeatComposite();
	RepeatComposite(std::string s);
	RepeatComposite(const RepeatComposite& c);

	Composite* clone();	
	virtual Action* getAction();
private:

};


#endif
