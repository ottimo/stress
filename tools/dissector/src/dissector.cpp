/*
 * dissector.cpp
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

#include <dissector.h>
#include <iostream>
#include <stdio.h>

/** Constructor.
 *
 * \param in the string containg the path to the input file
 * \param out the string containg the path to the output file (the output directory in "multiple file" mode)
 * \param layer a number representing the the operative layer, that determinates how much of the packet is printed
 * \param m a boolean value: if true one file is created for each packet in the capture file, if false a single file is created
 * \param v a boolean value: if true the output is verbose
 */


Dissector::Dissector(string in, string out, int layer, bool m, bool v) : pcapFile(in),
outputDirectory(out),
captureFile(NULL),
captureModel(layer, v),
verbose(v),
multipleFiles(m),
initialized(false) {
}

/** Destructor.
 *
 */

Dissector::~Dissector() {
    if (captureFile)
        //   delete captureFile; //FIXME su linux mi fa segfault
        epan_cleanup();
}

/** This methods initializes the structures used by the wireshark library and open the capture file.
 *!
 */

bool Dissector::init() {

    /*
     * Get credential information for later use.
     */
    get_credential_info();

    /* Register all dissectors*/

    epan_init(register_all_protocols,
            register_all_protocol_handoffs, NULL, NULL,
            failure_message, open_failure_message, read_failure_message,
            write_failure_message);

    captureFile = new CaptureFile(pcapFile);
    bool r = captureFile->open();

    if (!r) printError(captureFile->getErrorMessage());
    return r;
}

/** This methods load the capture file and initialized the class members with the data in the packets.
 *
 */

int Dissector::loadCapFile() {

    int err;
    gchar* err_info;
    gint64 data_offset = 0;

    frame_data* fdata;
    int c = 1;
    if (captureFile != NULL) {
        fdata = captureFile->readPacket(&err, &err_info, &data_offset);
        while (fdata) {

            if (c == 10 || true) processPacket(fdata, c);


            if (fdata->pfd)
                g_slist_free(fdata->pfd);


            c++;
            fdata = captureFile->readPacket(&err, &err_info, &data_offset);
        }



        if (err != 0) {
            /* Print a message noting that the read failed somewhere along the line. */
            switch (err) {

                case WTAP_ERR_UNSUPPORTED_ENCAP:
                {
                    ostringstream s;
                    s << "File " << pcapFile << " has a packet with a network type that is not supported. (" << err_info << ")";
                    g_free(err_info);
                    printError(s.str());
                    break;
                }
                case WTAP_ERR_CANT_READ:
                {
                    ostringstream s;
                    s << "An attempt to read from file " << pcapFile << " failed for some unknown reason.";
                    printError(s.str());
                    break;
                }
                case WTAP_ERR_SHORT_READ:
                {
                    ostringstream s;
                    s << "File " << pcapFile << " appears to have been cut short in the middle of a packet.";
                    printError(s.str());
                    break;
                }
                case WTAP_ERR_BAD_RECORD:
                {
                    ostringstream s;
                    s << "File " << pcapFile << " appears to be damaged or corrupt.";
                    printError(s.str());
                    g_free(err_info);
                    break;
                }
                default:
                {
                    ostringstream s;
                    s << "An error occurred while reading" << " file " << pcapFile << ": " << wtap_strerror(err);
                    printError(s.str());

                    break;
                }
            }

        }

        captureFile->close();
        return err;
    } else return 1;
}

/** This method perform the actual dissection on each single packet in the capture file and creates an object of class Packet for
 * each one of them.
 *
 * \sa Packet
 * \param fdata the pointer to a frame_data structure containing the data contained in one packet, read from the capture file
 * \param number the sequential number of the packet
 */

bool Dissector::processPacket(frame_data *fdata, int number) {


    epan_dissect_t *edt;

	 // Wireshark function initializes struct epan_dissect_t 
    edt = epan_dissect_new(TRUE, TRUE);

    epan_dissect_run(edt, captureFile->getPseudoHeader(), captureFile->getPd(), fdata, NULL);

    captureModel.createPacket(edt, number);

    return 0;
}

/** This method must be called in order to start the dissection process. It reads the capture file, performs dissection on each packet and save the results on file.
 *
 */

bool Dissector::runDissection() {
    if (captureFile && loadCapFile() == 0) {
        captureModel.savePacketsAsAbnf(outputDirectory, multipleFiles, verbose);
        return true;
    } else return false;
}

/** This is a static method used to output errors.
 *
 * \param message a string containing the message to be displayed
 */


void Dissector::printError(string message) {
    cout << "ERROR " << message << endl;
}

/** This is a static method used by the wireshark dissector to report general errors.
 *
 * \param msg_format a char array containing the message with embedded format tags
 * \param va_list the list of arguments in the message
 */

void Dissector::failure_message(const char *msg_format, va_list ap) {
    //    ostringstream message;
    //    message << "dftest: " << msg_format;
    //    printError(message.str());
    vfprintf(stderr, msg_format, ap); //FIXME va messo in una stringa!
    fprintf(stderr, "\n");
}

/** This is a static method used by the wireshark dissector to report open/create errors.
 *
 * \param filename a char array containing the message
 * \param err the error code
 * \param for_writing a boolean value needed by the wireshark library  in the method signature but not used
 */
void Dissector::open_failure_message(const char *filename, int err, gboolean for_writing) {
    ostringstream message;
    message << "dftest: " << file_open_error_message(err, for_writing) << " " << filename;
    printError(message.str());
}

/** This is a static method used by the wireshark dissector to report read errors.
 *
 * \param filename a char array containing the message
 * \param err the error code
 */
void Dissector::read_failure_message(const char *filename, int err) {
    ostringstream message;
    message << "dftest: An error occurred while reading from the file " << filename,
            printError(message.str());

}

/** This is a static method used by the wireshark dissector to report write errors.
 *
 * \param filename a char array containing the message
 * \param err the error code
 */
void Dissector::write_failure_message(const char *filename, int err) {
    ostringstream message;
    message << "dftest: An error occurred while writing to the file " << filename,
            printError(message.str());
}

