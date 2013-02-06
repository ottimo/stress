/* 
 * File:   AnomalyInjector.h
 * Author: Lu
 *
 * Created on 26 aprile 2010, 14.45
 */

#ifndef _ANOMALYINJECTOR_H
#define	_ANOMALYINJECTOR_H

#include <string>
#include <abstract-injector-performer.h>
#include <sstream>
#include <vector>

class AnomalyInjector {
public:
    AnomalyInjector();
    AnomalyInjector(const AnomalyInjector& orig);
    virtual ~AnomalyInjector();

    bool inject(Composite* root);
    bool write(Composite* root);
    void configure(int p);
    bool isActive(){return active;};
    Composite* findSendComposite(Composite* , std::vector<Composite*>&);
    
private:
    std::string outFileName;
    AbstractInjectorPerformer* injector;
    bool writeInjectedTreeToABNF(Composite*, std::ostringstream&);
    std::string trim(Composite* orig);
    std::string addQuote(std::string s);
    std::string getNodeName4File(Composite* orig);
    bool configure(bool strL = true, std::string fname = "/tmp/self.bnf", bool hexL = false, bool decL = false, bool binL = false);
    bool active;
};

#endif	/* _ANOMALYINJECTOR_H */

