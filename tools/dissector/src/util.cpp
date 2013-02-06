/*
 * util.cpp
 * Dissector
 *
 * Copyright (C) Evelina Agostini 2010 <evelina.agostini@gmail.com>
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

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <string.h>
#include <stdlib.h>

#include "util.h"
using namespace std;

/** This method takes a string as an argument: it trims it and removes characters that are not allowed in abnf format.
/*!
 \param p the input string
 \return the resulting string
 */
std::string Util::trimPlus(std::string p) {

    vector<char> illegalChars;

    illegalChars.push_back(' ');
    illegalChars.push_back('-');
    illegalChars.push_back('(');
    illegalChars.push_back(')');
    illegalChars.push_back('\'');
    illegalChars.push_back('\"');
    illegalChars.push_back('.');
    //TODO altri??

    vector<char>::iterator it;

    for (it = illegalChars.begin(); it != illegalChars.end(); it++) {

        char c = (*it);

        size_t it = p.find(c);

        while (it != string::npos) {

            if (it + 1 != string::npos)
                p[it + 1] = toupper(p[it + 1]);

            p.erase(it, 1);
            it = p.find(c);
        }

    }

    return p;
}

/** This method takes as an argument a string representing an ethernet address in hexadecimal
        digits, in the format dd:dd:dd:dd:dd:dd and trasform it to the format dddddddddddd,
        in order to be inserted in an abnf file.
 \param eth the input string
 \return the resulting string
 */

std::string Util::ethToHex(std::string eth) {
    if (eth.length() != 17) return "ERROR: Invalid FT_ETHER";


    ostringstream hexString;
    int i = 0;

    while (i < 16) {
        hexString << setw(2) << eth.substr(i, 2);
        i = i + 3;
    }

    return hexString.str();

}

/** This method takes as an argument a string representing an generic data in hexadecimal
 digits, in the format dd:dd:dd:dd:dd:dd... and trasform it to the format dddddddddddd..., 
 in order to be inserted in an abnf file.
 \param data the input string
 \return the resulting string
 */

std::string Util::dataToHex(std::string data) {

    ostringstream hexString;
    int i = 0;

    while (i < data.length() - 1) {
        hexString << setw(2) << data.substr(i, 2);

        i = i + 3;
    }

    return hexString.str();
}

/** This method takes as an argument an array of characters and creates a
 * string that represents the hexadecimal value for each character.
 \param ascii the input char array
 \return the resulting hex string
 */

std::string Util::asciiToHex(const gchar* ascii) {
    ostringstream hexString;

    for (int i = 0; i < strlen(ascii); i++) {
        hexString << hex << (int) ascii[i];
    }

    //  cout << hexString;

    return hexString.str();
}

/** This method takes as an argument an array of characters and check if
 * each one is in the range of values allowed in an ABNF string. It creates a
 * string made of a sequence of strings (between quotation marks) and hexadecimal
 * values (preceded by '%x'): for each sequence of allowed characters it creates a string
 * while if there are some characters which are not allowed it put them in the hexadecimal
 * format.
 \param ascii the input string
 \return the resulting string
 */

std::string Util::getABNFString(const gchar* ascii) {
	int min = 32;
    int max = 126;
    string hexstring(ascii);
    ostringstream output;

    int i = 0;

    bool inhex = false;
    bool instring = false;

    while (i < hexstring.length() - 1) {
        string str = hexstring.substr(i, 2);
        int v = strtol(str.c_str(), NULL, 16);
        if (v >= min && v <= max && v!= 34) { 
            if (!instring) output << " \""; //se non sono dentro una stringa devo aprire le virgolette
            output << (char) v;
            instring = true;
            inhex = false;
        } else {
            if (instring) output << "\" "; //se stavo scrivendo una stringa devo chiudere le virgolette
            if (!inhex) output << "%x"; //se non sto scrivendo un hex lo inizio
            output << str;
            instring = false;
            inhex = true;
        }
        i = i + 2;
    }
    if (instring) output << "\" "; //se stavo scrivendo una stringa devo chiudere le virgolette



    return output.str();


}
