#include "Interfaces.h"
#ifndef _WIN32
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <pthread.h>
#endif
#include <stdint.h>
#include "udt.h"


enum {
	CONNECTION_MODE_CLIENT_SERVER=1,
	CONNECTION_MODE_RENDEZVOUS
};

namespace UDT {

	class CCUDT: public ICCUDT {
	public:
		CCUDT();
		~CCUDT();
		// for each connection
		virtual void  StartUDTServer();
		virtual void StartUDTClientRequestReceiver();
		virtual void StartUDTClient(char *server_node, char *server_service);
		virtual void  StartTx();
		virtual void StartTestTx();
		virtual void  StartRx();
		virtual void RxRequest();
	private:
		unsigned int serverPort;
		addrinfo hints;
		addrinfo* res;
		addrinfo hints_client;
		addrinfo* res_client;
		addrinfo *local;
		addrinfo *peer;
		UDTSOCKET serv;
		UDTSOCKET client;
		UDTSOCKET recver;
		UDTSOCKET sender;
		int connectionMode;
		char clienthost[NI_MAXHOST];
		char clientservice[NI_MAXSERV];
	};
}