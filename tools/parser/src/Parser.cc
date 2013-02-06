#include <iostream>
#include <ABNFParser.h>
#include <Parser.h>
#include <getopt.h>

using namespace std;

static void usage(){
    std::stringstream ss;
	ss << "--------------------------------------------------------------------------------" << std::endl;
	ss << "ABNF Parser - Utility to validate abnf files" << std::endl;
	ss << "" << std::endl;
	ss << "parseabnf -i FILENAME [-o OUTPUT_FILE]" << std::endl;
	ss << "" << std::endl;
	ss << "--------------------------------------------------------------------------------" << std::endl;
	ss << "" << std::endl;
	ss << "--input=FILE	-i FILE Insert abnf file name" << std::endl;
        ss << "--out=FILE	-o FILE Create a dot representation of the abnf tree (insert output file name)" << std::endl;
	ss << "" << std::endl;
	std::cout << ss.str();
}


static struct option long_options[] = {
                   {"input", 1, 0, 'i'},
                   {"output", 1, 0, 'o'},
		   {"help", 0, 0, 'h'},
                   {0, 0, 0, 0}
               };


int main (int argc, char *argv[])
{

      if (argc==1){
        usage();
        return 0;
    }


	int opt = 1;
        string input="";
        string output="";

        bool result=false;


               std::cout << endl;



	while(opt > 0){
		int option_index;
		std::stringstream ss;
		opt = getopt_long(argc, argv, "i:o:h", long_options, &option_index);

		switch(opt){
			case 'i':
				input = std::string(optarg);

				break;
                        case 'o':
				output = std::string(optarg);

				break;
        
			case 'h':
				usage();
                                return(0);
				break;
			default:
				break;
		}

	}

        if (input=="") {
         cout <<   "Run parseabnf -h for help" << endl<< endl;
              return(0);
        }
        else{
            cout << "Input file: " << input << endl;
            if (output!="")
                {
                 cout << "Output file: " << output << endl;
			ABNFParser parser(input,"",output);
			result= parser.parse();
			parser.saveTree();
		}
           else
		{
			ABNFParser parser(input);
			result=parser.parse();

		}

        }
               if (result)
                std::cout << "parsing succeded" << std::endl;
               else
                std::cout << "parsing failed" << std::endl;
	return 0;
}

