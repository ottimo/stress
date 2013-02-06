#include<iostream>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<linux/if_ether.h>
#include<netpacket/packet.h>
#include<netinet/in.h>
#include<netinet/ether.h>
#include<net/if.h>
#include<net/ethernet.h>

#include <raw-socket.h>
#include <configurator.h>

int read_wrapper(int i, void* cpp, int l){
	return read(i, cpp, l);
};

RawSocket::RawSocket():sock(0){

	//vars definitions
	struct sockaddr_ll saddr;
	std::string iface;
	struct ifreq ifr;
	int n=0;

	//vars initialization
	iface = Configurator::getInstance()->getIface();
	memset(&saddr, 0, sizeof(saddr));
	memset(&ifr, 0, sizeof(ifr));
	sock = socket(PF_PACKET, SOCK_RAW,htons(ETH_P_ALL));
	if(sock < 0)
		std::cout << "socket(): "<< strerror(errno) << std::endl;

	//main body

	strncpy(ifr.ifr_name, iface.c_str(), iface.size());
	n = ioctl(sock, SIOCGIFINDEX, &ifr);
	if(n<0)
		std::cout << "ioctl(SIOCGIFINDEX): "<< strerror(errno) << std::endl;


	saddr.sll_family = AF_PACKET;
	saddr.sll_protocol = htons(ETH_P_ALL);
	saddr.sll_ifindex=ifr.ifr_ifindex;

	n = bind(sock, (struct sockaddr *) &saddr, sizeof(saddr));
	if(n<0)
		std::cout << "bind(): "<< strerror(errno) << std::endl;


	//n = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, iface.c_str(), iface.size());
	//if(n<0)
	//	cout << "setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE): "<< strerror(errno) << endl;

};

RawSocket::~RawSocket(){
  	unsigned char data[] = 	"\x80\x00\x00\x00"
						"\xff\xff\xff\xff\xff\xff"
						"\x00\x1e\x58\xa0\xe9\x89"
						"\x00\x1e\x58\xa0\xe9\x89"
						"\x05\xd1"
						"\x98\xc1\xfc\x7c\x02\x00\x00\x00"
						"\x64\x00\x31\x00\x00\x05\x50\x55"
						"\x50\x50\x41\x01\x04\x82\x84\x8b"
						"\x96\x03\x01\x01\x05\x03\x00\x02";

	//vars terminating
	close(sock);

};

int RawSocket::send(std::vector<uint8_t>& data){
    unsigned char u8aRadiotap[] = {
        0x00, 0x00, // <-- radiotap version
        0x0c, 0x00, // <- radiotap header length
        0x04, 0x80, 0x00, 0x00, // <-- bitmap
        0x00, // <-- rate
        0x00, // <-- padding for natural alignment
        0x18, 0x00, // <-- TX flags
    };


	unsigned char tmpbuf[4096];
	memset(tmpbuf, 0, 4096);
	memcpy(tmpbuf, u8aRadiotap, sizeof(u8aRadiotap));
	//memcpy(tmpbuf+sizeof(u8aRadiotap), data, sizeof(data));
        std::vector<uint8_t>::iterator it;
        int index = sizeof(u8aRadiotap);
        for(it = data.begin(); it!=data.end(); it++){
            tmpbuf[index] = *it;
            index++;
        }

	int n = write(sock, tmpbuf, sizeof(u8aRadiotap)+data.size());
	if(n<0)
		std::cout << "write(): "<< strerror(errno) << std::endl;

};

int RawSocket::read(std::vector<uint8_t>& data_to_read){
	unsigned char data[4096];
	memset(data,0,4096);
	data_to_read.clear();
	size_t len = -1;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	timeval* tv = getTimeout();
	int n = select(sock+1, &fds, 0, 0, tv );

	if(n>0){
		len = read_wrapper(sock, data, 4096);
		if(len<0)
			std::cout << "read(): "<< strerror(errno) << std::endl;
		uint16_t hdrlen = data[2];
		std::cout << "Header length: " <<hdrlen<< std::endl;
		for(int i=hdrlen; i<len; i++)
			data_to_read.push_back(data[i]);
		return len;
	}else
		return -1;
	return -1;
};

void RawSocket::reconnect(){};

bool RawSocket::check(){
	return true;
};
