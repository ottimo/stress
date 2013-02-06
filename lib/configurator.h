#ifndef _CONFIGURATOR_H
#define _CONFIGURATOR_H
#include <getopt.h>
#include <string>
#include <memory>
#include <ts-generator.h>

static struct option long_options[] = {
	{"abnf", 1, 0, 'a'},
	{"output", 1, 0, 'o'},
	{"type", 1, 0, 'O'},
	{"mac", 0, 0, 'm'},
	{"udp", 0, 0, 'u'},
	{"timeout", 1, 0, 't'},
	{"source", 1, 0, 's'},
	{"destination", 1, 0, 'd'},
	{"port", 1, 0, 'p'},
	{"help", 0, 0, 'h'},
    {"inject", 1, 0, 'j'},
	{"monitor", 0, 0, 'M'},
	{"delay", 1, 0, 'D'},
	{"iface", 1, 0, 'i'},
	{"listen", 0, 0, 'l'},
	{"generator", 1, 0, 'g'},
	{0, 0, 0, 0}
};

class Configurator{
	public:
		static Configurator* getInstance();
		void parseCmdLine(int, char**);

		const std::string& 	getAbnfFile();
		const std::string& 	getSourceIp(){return ip_src;};
		const std::string& 	getDestinationIp(){return ip_dst;};
		const std::string& 	getTypeOutput(){return type_output;};
		const int& 		getPort(){return dport;};
		const int&              getAutoInject(){return auto_inject;};
		const bool&	isUdp(){return udp; };
		const bool& 	isTcp() {return tcp; };
		const std::string	getOutputFile(){return output_file;};
		const int&		getDelay(){ return delay;};
		const int& 		getMonitor(){ return monitor; };
		const int&		getTimeout(){ return timeout; };
		const std::string&	getIface(){return iface;};
		const bool& getListen(){return listen; };
		TSGenerator* getTSGenerator(){return ts_generator;};
		
	protected:
		Configurator();
	private:
		void usage();
	public:

	protected:

	private:
		static std::auto_ptr<Configurator> _instance;
		std::string abnf_file;
		std::string output_file;
		bool mac;
		bool ip;
		bool udp;
		bool tcp;
		std::string ip_src;
		std::string ip_dst;
		std::string type_output;
		int dport;
      	int auto_inject;
		int monitor;
		int delay;
		int timeout;
		std::string iface;
		bool listen;
		TSGenerator* ts_generator;
};

#endif
