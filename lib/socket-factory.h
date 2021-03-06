#ifndef _SOCKET_FACTORY_H
#define _SOCKET_FACTORY_H
#include <memory>
#include <udp-send-action.h>
#include <udp-read-action.h>
#include <tcp-send-action.h>
#include <tcp-read-action.h>
#include <raw-send-action.h>
#include <raw-read-action.h>
#include <btrfcomm-send-command.h> //Newly added!
#include <btrfcomm-read-command.h> //Newly added!
#include <socket.h>
#include <udp-socket.h>
#include <tcp-socket.h>
#include <raw-socket.h>

struct sock_ref{
	int num_ref;
	Socket* sock;
};

class SocketFactory {
	public:
		static SocketFactory* getInstance();
		~SocketFactory();
		
		Socket* getSocket(UdpSendAction*);
		Socket* getSocket(UdpReadAction*);
		Socket* getSocket(TcpSendAction*);
		Socket* getSocket(TcpReadAction*);
		Socket* getSocket(RawSendAction*);
		Socket* getSocket(RawReadAction*);
		Socket* getSocket(BTrfcommSend*); //Newly added!
		Socket* getSocket(BTrfcommRead*); //Newly added!
		Socket* getSocket();
		
		void releaseSocket(UdpSendAction*);
		void releaseSocket(UdpReadAction*);
		void releaseSocket(TcpSendAction*);
		void releaseSocket(TcpReadAction*);
		void releaseSocket(RawSendAction*);
		void releaseSocket(RawReadAction*);
		void releaseSocket(BTrfcommSend*); //Newly added!
		void releaseSocket(BTrfcommRead*); //Newly added!
		void releaseSocket();
		void releaseSocket(Socket*);
		void releaseAllSocket();
		
		void reconnectSocket();
	protected:
		SocketFactory();	

	private:

		Socket* 	getUdpSocket();
		void		releaseUdpSocket();
		Socket* 	getTcpSocket();
		void		releaseTcpSocket();
		Socket* 	getRawSocket();
		void		releaseRawSocket();
		Socket* 	getBTrfcommSocket();//newly added!
		void		releaseBTrfcommSocket();//newly added!
		Socket*		newTcpSocket();
		Socket*		newUdpSocket();
		Socket*		newRawSocket();
		Socket*		newBTrfcommSocket();//newly added!
	public:

	protected:

	private:
		static std::auto_ptr<SocketFactory> _instance;
		int udp;
		Socket* udp_socket;
		int tcp;
		Socket* tcp_socket;
		int raw;
		Socket* raw_socket;
		int btrfcomm;//newly added!
		Socket* btrfcomm_socket; //newly added!
};

#endif
