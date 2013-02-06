/*
 * util.h
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


#ifndef _UTILH_
#define _UTILH_

#include <string>
#include <glib.h>

//!  Class Util

/*!
This class contains static methods used to transform strings in order to be inserted
 in abnf files.
 */

class Util {
public:

    static std::string trimPlus(std::string);
    static std::string ethToHex(std::string eth);
    static std::string dataToHex(std::string data);
    static std::string asciiToHex(const gchar* ascii);
    static std::string getABNFString(const gchar* ascii);
};





#endif
