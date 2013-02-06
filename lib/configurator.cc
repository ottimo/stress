#include <configurator.h>
#include <sstream>
#include <iostream>
#include <cstdlib>


std::auto_ptr<Configurator> Configurator::_instance;

Configurator::Configurator() : abnf_file(""), output_file("/tmp/state"),
mac(false), ip(false), udp(false), tcp(false),
ip_src("127.0.0.1"), ip_dst("127.0.0.1"), type_output("xml"), dport(0),
auto_inject(0),delay(250),monitor(0),timeout(2000),iface("mon0"),listen(false) {
	ts_generator = new ExpTSGenerator();
};

Configurator* Configurator::getInstance() {
    if (_instance.get() == 0)
        _instance = std::auto_ptr<Configurator > (new Configurator());
    return _instance.get();
};

void Configurator::usage() {
    std::stringstream ss;
    ss << "--------------------------------------------------------------------------------" << std::endl;
    ss << "S.T.R.E.S.S. - S.T.R.E.S.S. Testing and Reverse Engineering for System Security" << std::endl;
    ss << "" << std::endl;
    ss << "stress -a FILENAME -d DESTINATION_ADDRESS -p PORT" << std::endl;
    ss << "" << std::endl;
    ss << "--------------------------------------------------------------------------------" << std::endl;
    ss << "" << std::endl;
    ss << "--abnf=FILENAME	 -a FILENAME		Insert ABNF model file" << std::endl;
    ss << "" << std::endl;
    ss << "--destination IP -d IP			Insert Destination IP Address" << std::endl;
    ss << "--source IP	 -s IP			Insert Source IP Address" << std::endl;
    ss << "--port PORT	 -p PORT			Insert Port Number" << std::endl;
    ss << "--timeout N	 -t N			Wait n msec for reading packets" << std::endl;
    ss << "--iface IFACE	 -i IFACE		Network interface used for RAW socket" << std::endl;
    ss << "--listen	 -l			Enable server mode" << std::endl;
    ss << "" << std::endl;	
    ss << "--help		 -h 			Display this help " << std::endl;
    ss << "--output FILENAME -o FILENAME 		Insert output root filename" << std::endl;
    ss << "--type		 -O OUTPUT-TYPE		xml, dot [default: xml]" << std::endl;
    ss << "" << std::endl;
 	ss << "--monitor	 -M			Activate observing modules" << std::endl;
	ss << "" << std::endl;
	ss << "--delay n	 -D n			Delay n (msec) befare AND after a test case" << std::endl;
	ss << "" << std::endl;
    ss << "--inject	 -j MODE		Activate anomalies auto-injection. 0(none),1+(string),2+(hex),3+(dec),4+(bin)" << std::endl;
	ss << "--generator	 -g GENERATOR		Select TestCase Generator:" << std::endl;
	ss << "					(default) 0: Exponential Testcase generator (Max number of faults 64)" << std::endl;
	ss << "					          1: Sequential Testcase generator (Enables only one fault each test case)" << std::endl;
    std::cout << ss.str();
    exit(0);
};

void Configurator::parseCmdLine(int argc, char* argv[]) {
    int opt = 1;
	int ts_type = 0;

    while (opt > 0) {
        int option_index;
        std::stringstream ss;
        opt = getopt_long(argc, argv, "a:mi:ut:s:d:p:ho:O:j:D:Mlg:",
                long_options, &option_index);

        switch (opt) {
            case 'a':
                abnf_file = std::string(optarg);
                std::cout << "File ABNF: " << abnf_file << std::endl;
                break;
            case 'm':
                usage();
                mac = true;
                break;
            case 'u':
                usage();
                udp = true;
                std::cout << "UDP Mode: " << udp << std::endl;
                break;
            case 't':
					 ss << optarg;
					 ss >> timeout;
                std::cout << "Timeout: " << timeout << " msec" << std::endl;
                break;
            case 's':
                ip_src = std::string(optarg);
                std::cout << "Source: " << ip_src << std::endl;
                break;
            case 'd':
                ip_dst = std::string(optarg);
                std::cout << "Destination: " << ip_dst << std::endl;
                break;
            case 'p':
				ss.clear();
                ss << optarg;
                ss >> dport;
                std::cout << "Port: " << dport << std::endl;
                break;
            case 'o':
                output_file = std::string(optarg);
                break;
            case 'O':
                type_output = std::string(optarg);
                break;
			case 'M':
				monitor = 1;
				break;
			case 'D':
				ss.clear();
				ss << optarg;
				ss >> delay;
				break;
            case 'h':
                usage();
                break;
            case 'j':
				ss.clear();
                ss << optarg;
                ss >> auto_inject;
                if (auto_inject <= 0 || auto_inject > 4)
                    std::cout << "Auto-Injection is OFF (" << auto_inject << ")" << std::endl;
                else
                    std::cout << "Auto-Injection is ON (" << auto_inject << ")" << std::endl;
                break;
            case 'i':
                iface = std::string(optarg);
                std::cout << "Interface: " << iface << std::endl;
                break;
			case 'l':
				listen = true;
                std::cout << "Server Mode enabled!" << std::endl;
				break;
			case 'g':
				ss.clear();
				ss << optarg;
				ss >> ts_type;
				if(ts_type == 1){
					delete ts_generator;
					ts_generator = new SeqTSGenerator();
				}
				break;
            default:
                break;
        }

    }
};

const std::string& Configurator::getAbnfFile(){
	if(abnf_file.compare("") == 0)
		usage();
	return abnf_file;
};
