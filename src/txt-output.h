/* 
 * File:   TXTOutput.h
 * Author: Eve
 *
 * Created on 9 febbraio 2010, 12.48
 */

#ifndef _TXTOUTPUT_H
#define _TXTOUTPUT_H

#include <output.h>
#include <sstream>

class TXTOutput : public AbstractOutput {
public:
    TXTOutput();
    TXTOutput(const TXTOutput& orig);
    virtual ~TXTOutput();
    virtual void printTree(State*, uint64_t, int);
protected:

private:
    void runTreeState(std::ostringstream*, State*);
};

#endif	/* _TXTOUTPUT_H */

