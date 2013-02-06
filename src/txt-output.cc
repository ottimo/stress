/* 
 * File:   TXTOutput.cc
 * Author: Eve
 * 
 * Created on 9 febbraio 2010, 12.48
 */

#include <txt-output.h>
#include <configurator.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>
#include <value-state.h>
#include <value-hex-state.h>

TXTOutput::TXTOutput() : AbstractOutput() {
}

TXTOutput::TXTOutput(const TXTOutput& orig) : AbstractOutput(orig) {
}

TXTOutput::~TXTOutput() {
}

void TXTOutput::printTree(State* s, uint64_t id, int run) {
    std::string outfilename = getFilename(id, run);
    outfilename += ".txt";
    std::ostringstream abnfTreeString;
    runTreeState(&abnfTreeString, s);

    std::ofstream myfile(outfilename.c_str());
    if (myfile.is_open()) {

        myfile << "Testcase id: " << id << " run:" << run << std::endl << std::endl;


time_t current_time;
time (&current_time);
          myfile  << ctime(&current_time) << std::endl ;

        myfile << abnfTreeString.str();
        myfile.close();
    } else
        std::cerr << std::string("Unable to open file ") << outfilename << std::endl;
};

void TXTOutput::runTreeState(std::ostringstream* oss, State* s) {
    std::vector<State*>& derived_state = s->getSons();
    std::vector<State*>::iterator sons_iterator;
  


       if (!derived_state.empty()) {
            State* firstChild = derived_state.front();


            if (ValueState * firstChildVs = dynamic_cast<ValueState*> (firstChild)) {
                std::string fcValue = firstChildVs->getValueForLog();
                if (fcValue.find("%STRESS_read%") != std::string::npos) {
                  if (ValueState * vsr = dynamic_cast<ValueState*> (s)) {
                       std::string value = vsr->getValueForLog();
                       *oss << std::endl << "Received:  " << value;
                  }
                  else {
                       *oss << std::endl << "No data received" << std::endl;
                  }
                }

                }

            }


    if (ValueState * vs = dynamic_cast<ValueState*> (s)) {
        std::string value = vs->getValueForLog();


        if (value.find("%STRESS_send%") != std::string::npos) {

            if (value.size() != 13) {
                *oss << "Sent:  " << value.substr(13, value.size()) << std::endl;
                *oss << "Packet:" << std::endl;
            }
         
        } else {
         
            if (value.find("%STRESS_read%") == std::string::npos) {
                if (vs->isFaultValue()) *oss << "*";

                *oss << value;
                if (vs->isFaultValue()) *oss << "*";
                *oss << std::endl;
            }
        }
    }

    if (!derived_state.empty()) {

        if (s->isFaultValue()) *oss << "**" << std::endl;

        for (sons_iterator = derived_state.begin(); sons_iterator != derived_state.end(); sons_iterator++) {
            runTreeState(oss, (*sons_iterator));
        }
           if (s->isFaultValue()) *oss << "**" << std::endl;;

    }
};
