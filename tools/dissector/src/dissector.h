/*
 * dissector.h
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




#include <string>
#include "fileh.h"
#include <model.h>
#include <util.h>

extern "C" {
#include <epan/packet.h>
#include <epan/timestamp.h>
#include <epan/tvbuff.h>
#include <epan/epan.h>
#include <wsutil/privileges.h>
#include <epan/prefs.h>
#include <epan/tap.h>
#include <epan/expert.h>
#include <epan/crypt/crypt-md5.h>
#include <epan/timestamp.h>
#include <epan/plugins.h>
#include <epan/filesystem.h>
#include <epan/prefs.h>
#include <register.h>	
}

using namespace std;


//!  Class Dissector

/*!
This class represents the dissector. It contains the methods that open the pcap file and perform the actual dissection.
 * It contains also the methods that handle output.
 */


class Dissector {
public:

    Dissector(string in, string out = ".", int layer = 2, bool multipleFiles = false, bool verbose = false);
    ~Dissector();


    bool runDissection();
    bool init(); //va chiamata per inizializzare tutte le strutture di "wireshark" e aprire il file pcap

    static void printError(string message);

private:
    int loadCapFile();
    bool processPacket(frame_data *fdata, int number);


    static void failure_message(const char *msg_format, va_list ap);
    static void write_failure_message(const char *filename, int err);
    static void read_failure_message(const char *filename, int err);
    static void open_failure_message(const char *filename, int err, gboolean for_writing);

    string pcapFile;
    string outputDirectory;

    CaptureFile* captureFile;
    CaptureModel captureModel;
    bool verbose;
    bool multipleFiles;
    bool initialized;
};


