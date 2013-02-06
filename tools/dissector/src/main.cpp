/*
 * main.cpp
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

#include <iostream>
#include <dissector.h>
#include <string>
#include <getopt.h>


using namespace std;
static void failure_message(const char *msg_format, va_list ap);
static void open_failure_message(const char *filename, int err,
        gboolean for_writing);
static void read_failure_message(const char *filename, int err);
static void write_failure_message(const char *filename, int err);

static struct option long_options[] = {
    {"input", 1, 0, 'i'},
    {"output", 1, 0, 'o'},
    {"layer", 0, 0, 'l'},
    {"verbose", 0, 0, 'v'},
    {"help", 0, 0, 'h'},
    {0, 0, 0, 0}
};

static void usage() {
    std::stringstream ss;
    ss << "--------------------------------------------------------------------------------" << std::endl;
    ss << "Dissector - Utility to create abnf representation of network packets" << std::endl;
    ss << "" << std::endl;
    ss << "dissect -i FILENAME -o OUTPUT_FILE | -m OUTPUT_DIRECTORY [-l SELECTED_LAYER]" << std::endl;
    ss << "" << std::endl;
    ss << "--------------------------------------------------------------------------------" << std::endl;
    ss << "" << std::endl;
    ss << "--input=FILE	-i FILE				Insert .pcap file name" << std::endl;
    ss << "--out=FILE	-o FILE                         Insert output file name" << std::endl;
    ss << "--layer=LAYER  	-l LAYER     [defalut=2]        Insert selected layer number" << std::endl;
    ss << "--multiple=DIR	-m DIR       [defalut=FALSE]    Create one file for each packet, in selected directory" << std::endl;
    ss << "--verbose	-v           [defalut=FALSE]    Verbose output" << std::endl;
    ss << "--help		-h                              Display this help " << std::endl;

    ss << "" << std::endl;
    std::cout << ss.str();
}

int main(int argc, char * const argv[]) {



    if (argc == 1) {
        usage();
        return 0;
    }


    int opt = 1;
    string input = "";
    string output = "";
    string outputDir = "";
    int layer = 2;
    bool verbose = false;
    bool multipleFiles = false;
    std::cout << endl;



    while (opt > 0) {
        int option_index;
        std::stringstream ss;
        opt = getopt_long(argc, argv, "i:o:l:m:vh", long_options, &option_index);

        switch (opt) {
            case 'i':
                input = std::string(optarg);

                break;
            case 'o':
                output = std::string(optarg);

                break;
            case 'l':
                layer = atoi(optarg);
                if (layer == 0) std::cout << "Invalid layer value. Using default." << std::endl;
                else std::cout << "Selected layer: " << layer << std::endl;
                break;
            case 'm':
                multipleFiles = true;
                outputDir = std::string(optarg);

                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                usage();
                return (0);
                break;
            default:
                break;
        }

    }

    if (input == "") {
        cout << "Run dissect -h for help" << endl << endl;
        return (0);
    } else {
        std::cout << endl;
        std::cout << "File pcap: " << input << std::endl;

        if (multipleFiles && outputDir != "") {
            std::cout << "Output directory: " << outputDir << std::endl;
        } else
            if (!multipleFiles && output != "")
            std::cout << "Output file: " << output << std::endl;
        else
            std::cout << "No output file" << std::endl;

        if (multipleFiles)output = outputDir;
        Dissector dissector(input, output, layer, multipleFiles, verbose);
        bool ok = false;
        if (dissector.init()) {
            if (dissector.runDissection()) {

                ok = true;
            }
        }
        if (ok)
            std::cout << std::endl << "dissection completed" << std::endl;
        else
            std::cout << std::endl << "dissection not performed" << std::endl;

    }

}



