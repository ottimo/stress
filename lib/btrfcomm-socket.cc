#include <tcp-socket.h>
#include <configurator.h>
#include <iostream>
#include <sys/select.h>
#include <btrfcomm-socket.h>
extern "C" {
	#include <unistd.h>
	#include <sys/socket.h>
	#include <bluetooth/bluetooth.h>
	#include <bluetooth/rfcomm.h>
	#include <stdio.h>

	int create_btrfcomm_socket(int *sock,struct sockaddr_rc *addr)
/*Returns the status of the socket.*/
	{
		addr->rc_family = AF_BLUETOOTH;
		(*sock) = socket(AF_BLUETOOTH, SOCK_STREAM,BTPROTO_RFCOMM);
    //fcntl(*sock, F_SETFL, O_NONBLOCK); //set sock to non-blocking mode
		int ret = connect(*sock, (struct sockaddr*) addr, sizeof(*addr));
		//printf("RET: %d\n",ret);	
		return ret;
	}

	int init_btrfcomm_socket(int *sock,const char* dest,struct sockaddr_rc *addr,uint8_t port)
/*dest is the BD_ADDR of the destination bt device. It return the status of the socket.*/
	{
		str2ba(dest,&(addr->rc_bdaddr)); //Possible segfault!
		addr->rc_channel = port;
		addr->rc_family = AF_BLUETOOTH;
		//(*sock) = socket(AF_BLUETOOTH, SOCK_STREAM,BTPROTO_RFCOMM);
		//return connect(*sock, (struct sockaddr*) addr, sizeof(*addr));
		return create_btrfcomm_socket(sock,addr);
	}
	int write2socket(int socket,char* data,int msg_length)
	/*returns the status of the socket.*/
	{
		return  write(socket,data,msg_length);
	}
	int read_socket(int socket,unsigned char* data,int msg_length,timeval *tv)
	/*returns the bytes read from the socket.*/
	{
		fd_set fds;

		FD_ZERO(&fds);
		FD_SET(socket, &fds);
	
		if(select(socket+1,&fds,NULL,NULL,tv))		
			return  read(socket,data,msg_length);
		else
			return -1;
	}
	void close_a_socket(int sock)
	{ 
		close(sock); 
	}

}

BTrfcommSocket::BTrfcommSocket() : Socket()	
{
	//std::cout << "Creating a BT socket\n";
	const char *str_addr = Configurator::getInstance()->getDestinationIp().c_str();
	uint8_t port= (uint8_t) Configurator::getInstance()->getPort();
	status = init_btrfcomm_socket(&socket,str_addr,&addr,port);
 //TODO check status error
};

BTrfcommSocket::~BTrfcommSocket(){
	//std::cout << "Closing a BT socket\n";
	close(socket);
};

int BTrfcommSocket::send(std::vector<uint8_t>& data_to_send){
	//std::cout << "Sending bt data\n";
	int msg_length = data_to_send.size();
	this->status = write2socket(socket,(char*)&(data_to_send[0]),msg_length);
	//TODO check status error
};

int BTrfcommSocket::read(std::vector<uint8_t>& data_to_read){
	//std::cout << "Ready to read!\n";
	//TODO increment the byte readable.
	unsigned char data[2050];

//	if (check() == false )
//		return -1;

	int length = read_socket(socket,data,2049,getTimeout());


	for(int i=0; i<length; i++)
	 data_to_read.push_back(data[i]);

	return length;
/*	memset(data,0,2049);
	data_to_read.clear();
	size_t len = -1;
	if(!socket.is_open())
		return -1;
	
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket.native(), &fds);
	timeval* tv = getTimeout();

	int n = select(socket.native()+1, &fds, 0, 0, tv );
	if(n>0){
	  try{
		 len = socket.receive(boost::asio::buffer(data, 2048));
	  } catch (boost::system::system_error& se) {
		 std::cerr << "TcpSocket::read(): " << se.what() << std::endl;
	  }
	  for(int i=0; i<2050; i++)
		 data_to_read.push_back(data[i]);
	  return len;
	}else
	  return -1;
*/
};

bool BTrfcommSocket::check()
/*Returns true if the socket is open and false if the socket is closed.*/
{
	//return socket.is_open();
	//return socket ? true : false;
	int error = 0,retval=0;
	socklen_t len = sizeof (error);
	retval = getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &len );

	//std::cout << "Checking a BT socket\n";
	//retval ? std::cout << "Socket closed\n" : std::cout<<"Socket open\n";;
	return (retval==0)  ? true : false;
};

void BTrfcommSocket::reconnect(){
//	std::cout << "Reconnecting\n";
/*
	if(check())
		close_a_socket(socket);
 	status = create_btrfcomm_socket(&socket,&addr); 
*/
	if(check())
		do
		{
			close_a_socket(socket);
			sleep(1); //needed because device is busy
			status = create_btrfcomm_socket(&socket,&addr); 		
		} while(status < 0)
 //TODO check status error
};
