#include "CCUDT.h"
#include <iostream>

#include <stdio.h> 
#include <stdlib.h> 

#ifdef WIN32
#include <Queue>
#include <errors.h>
#include <sys\types.h>
#include <WS2tcpip.h>
#endif

#include <errno.h> 

#ifdef LINUX
#include <queue>
#include <unistd.h>
#include <sys/types.h>
#endif

#ifdef OSX
#include <Queue>
#include <arpa/inet.h>
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace UDT;
extern std::queue<unsigned int> q;
extern pthread_mutex_t request_mutex ;

extern UDTTxArg_t *txData[MAX_QUEUE_SIZE];

/* global condition variable for our program. assignment initializes it. */ 
extern pthread_cond_t got_request;
extern int window_width; // The width of our window
extern int window_height; // The height of our window

extern unsigned int prodIndex;
extern unsigned int consIndex;
extern unsigned int connected;
extern char *servicePort;
extern unsigned int clientReqRecd;

namespace UDT {

/*********************************************************************************************************
 **		
 ** Function: 	CreateUDT
 ** Returns:	ICCUDT *
 ** Parameters:	None
 ** Notes: 	Return an object to UDT interface
 *********************************************************************************************************/
	ICCUDT *CreateUDT()
	{
		return new CCUDT(); // return a new object
	}

/*********************************************************************************************************
 **		
 ** Function: 	CCUDT constructor
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Starts the UDT library
 *********************************************************************************************************/
	CCUDT::CCUDT() 
	{
		UDT::startup(); // start the UDT library

	}
	
/*********************************************************************************************************
 **		
 ** Function: 	CCUDT destructor
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Releases the UDT library
 *********************************************************************************************************/
	CCUDT::~CCUDT()
	{
		UDT::cleanup(); // release the UDT library

	}

/*********************************************************************************************************
 **		
 ** Function: 	StartUDTClientRequestReceiver
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Repeatedly call RxRequest to receive requests from client
 *********************************************************************************************************/
	void CCUDT::StartUDTClientRequestReceiver()
	{
		while(1) {
			RxRequest();
		}
		return;
	}

/*********************************************************************************************************
 **		
 ** Function: 	RxRequest
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Receive requests from client to send next image. If the request matches a signature
 ** 		set a flag to 1.
 *********************************************************************************************************/
	void CCUDT::RxRequest()
	{
		int size = 8;
		char *data = new char[size];
		int rs=0, rsize=0;
		while (true)
		{
			while (rsize < size) {
				if (UDT::ERROR == (rs = UDT::recv(sender, data + rsize, size - rsize, 0)))
				{
					std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;	
					break;
				}
				DEBUG_LOG(MODULE_UDT, DBG, "server: ");
				DEBUG_LOG(MODULE_UDT, DBG, rs);
				DEBUG_LOG(MODULE_UDT, DBG, "bytes received");
				rsize += rs;
			}
			DEBUG_LOG(MODULE_UDT, DBG, "server: ");
			DEBUG_LOG(MODULE_UDT, DBG, rs);
			DEBUG_LOG(MODULE_UDT, DBG, "bytes received");

			if (rsize == size)	
				break;
		}
		if((*data == (char)(SIG1)) && (*(data+1) == (char)(SIG2)) && (*(data+2) == (char)(SIG3)) && (*(data+3) == (char)(SIG1))) {
			if(clientReqRecd) {
#ifdef MYDEBUG
				std::cout << "Request pending" << std::endl;
#endif
			}
			clientReqRecd=1;
		}
		delete [] data;

		return;
	}

/*********************************************************************************************************
 **		
 ** Function: 	StartTx
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	1. Check if a buffer is fresh and unlocked
 ** 		2. Lock the buffer for transmission to client.
 ** 		3. Transmit to client.
 ** 		4. Unlock the buffer after transmission.
 ** 		5. Set the clientReqRecd flag to 0
 *********************************************************************************************************/
	void CCUDT::StartTx()
	{
		static int n=0;
		static unsigned int bufferIndex = 0;
		bool noTx=true;
		bool freshBufferFound=false;
		unsigned int freshBufferIndex=0;
#ifdef BUFFER_NO_LOOP
		if((txData[consIndex]->flag & BUFFER_FRESH) && ((txData[consIndex]->flag & BUFFER_LOCKED) ==0)) {
			bufferIndex = consIndex;
		}
		else if((txData[(consIndex + 1)%MAX_QUEUE_SIZE]->flag & BUFFER_FRESH) && 
			((txData[(consIndex + 1)%MAX_QUEUE_SIZE]->flag & BUFFER_LOCKED) ==0)) {
			consIndex = (consIndex+1)%MAX_QUEUE_SIZE;
			bufferIndex = consIndex;
		}
#else
		for(int i=0; i < MAX_QUEUE_SIZE; i++, bufferIndex=(bufferIndex+1)%MAX_QUEUE_SIZE) {

			if(txData[bufferIndex]->flag & BUFFER_LOCKED)  continue;
			if((txData[bufferIndex]->flag & BUFFER_FRESH) == 0)  continue;
			freshBufferFound=true;
			freshBufferIndex=bufferIndex;
			txData[bufferIndex]->flag |= BUFFER_LOCKED;
			break;
		}
#endif
		if(!freshBufferFound) return;
		
		ASSERT(freshBufferIndex == bufferIndex);
		ASSERT(txData[bufferIndex]->flag & BUFFER_FRESH);
		ASSERT(txData[bufferIndex]->length);
		unsigned char data1[4] = {
			(unsigned char)(txData[bufferIndex]->length & 0xff),
			(unsigned char)((txData[bufferIndex]->length & 0xff00) >> 8),
			(unsigned char)((txData[bufferIndex]->length & 0xff0000) >> 16),
			(unsigned char)((txData[bufferIndex]->length & 0xff000000) >> 24)
		};

		char *data2 = (char *)(txData[bufferIndex]->buffer);
		int size = 0;
		consIndex = (consIndex+1)%MAX_QUEUE_SIZE;
		if(n < 5) {
			DEBUG_LOG(MODULE_UDT, DBG, "txLength: ");
			DEBUG_LOG(MODULE_UDT, DBG, txData[bufferIndex]->length);
			DEBUG_LOG(MODULE_UDT, DBG, "bytes transmitted");
			
			n++;
		}
		
		if((txData[bufferIndex]->length) && (data1 != NULL) && (data2 != NULL)) { 		

#if 1
			//#ifdef STREAM
			size = 4;
			int ssize = 0;
			for (int i = 0; i < size; i ++) {
				int ss=0;
				while (ssize < size) {
					if (UDT::ERROR == (ss = UDT::send(sender, (const char *)(data1 + ssize), size - ssize, 0)))
					{
						break;
					}
					ssize += ss;
				}
				if (ssize < size)	
					break;
			}

			size = (txData[bufferIndex]->length);
			ssize = 0;
			for (int i = 0; i < size; i ++) {
				int ss=0;
				while (ssize < size) {
					if (UDT::ERROR == (ss = UDT::send(sender, data2 + ssize, size - ssize, 0)))
					{
						break;
					}
					ssize += ss;
				}
				if (ssize < size)	
					break;
			}
			
#else //datagram
			int ss=0;
			if (UDT::ERROR == (ss = UDT::sendmsg(sender, data, size)))
			{
			}
#endif
		}
		noTx=false;
		clientReqRecd=0;
		txData[bufferIndex]->flag=0;

		if(noTx) {
			std::cout << "No Tx" << std::endl;
		} 
	}

/*********************************************************************************************************
 **		
 ** Function: 	StartTestTx
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	pthread_mutex based function used to test performance using various UDT transmission options.
 ** 		Legacy code.
 *********************************************************************************************************/
	void CCUDT::StartTestTx()
	{
#if 0
		while (1) {
			int rc = pthread_mutex_lock(&request_mutex);
			if (rc) { /* an error has occurred */
				std::cout << "pthread_mutex_lock" << std::endl; 
				pthread_exit(NULL);
			}
			if(q.size() == 2) {
				std::cout << "Server: " << q.front() << std::endl;
				q.pop();
			}
			rc = pthread_mutex_unlock(&request_mutex);
		}
#else
		int size = 100000;
		char* data = new char[size];
		int ssize = 0;
		for (int i = 0; i < 1000000; i ++) {


			int ss=0;

			while (ssize < size) {

				if (UDT::ERROR == (ss = UDT::send(sender, data + ssize, size - ssize, 0)))
				{
					std::cout << "send:" << UDT::getlasterror().getErrorMessage() << std::endl;		
					break;
				}
				ssize += ss;
			}
			if (ssize < size)	
				break;
		}


		std::cout << "Server " << ssize << " bytes sent" << std::endl;
		int size1 = 1024;
		char *data1 = new char[size1];
		int rs=0, rsize=0;
		while (true)
		{
			while (rsize < size1) {
				if (UDT::ERROR == (rs = UDT::recv(sender, data1 + rsize, size - rsize, 0)))
				{
					std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
					break;
				}
				std::cout << "server: " << rs << " bytes received" << std::endl;
				rsize += rs;
			}

			std::cout << "Server " << rs << " bytes received" << std::endl;
			if (rsize == size1)
				break;
		}

		std::cout << "Closing Server Socket" << std::endl;
		//UDT::close(serv);
		delete [] data;
#endif
	}

	void CCUDT::StartRx() 
	{
	}

/*********************************************************************************************************
 **		
 ** Function: 	StartUDTServer
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	1. Start UDT Server. Creates a UDT socket, binds and listens for an incmoing connection.
 ** 		2. Terminates after accepting incoming connection from client.
 *********************************************************************************************************/
	void CCUDT::StartUDTServer()
	{
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		//hints.ai_socktype = SOCK_DGRAM;
		//const char *servicePort = "5555";


		if (0 != getaddrinfo(NULL, servicePort, &hints, &res))
		{
			std::cout << "illegal port number or port is busy.\n" << std::endl;
			return;
		}


		serv = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (UDT::ERROR == UDT::bind(serv, res->ai_addr , res->ai_addrlen))
		{
			std::cout << "bind: " << UDT::getlasterror().getErrorMessage() << std::endl;
			return;
		}

		//bool block = true;
		//UDT::setsockopt(serv, 0, UDT_SNDSYN, &block, sizeof(bool));

		std::cout << "UDT server is ready at port: " << servicePort << std::endl;
		freeaddrinfo(res);
		if (UDT::ERROR == UDT::listen(serv, 10))
		{
			std::cout << "listen: " << UDT::getlasterror().getErrorMessage() << std::endl;
			return;
		}


		sockaddr_storage clientaddr;
		int addrlen = sizeof(clientaddr);

		if (UDT::INVALID_SOCK == (sender = UDT::accept(serv, (sockaddr*)&clientaddr, &addrlen)))
		{
			std::cout << "accept: " << UDT::getlasterror().getErrorMessage() << std::endl;
			return;
		}


		std::cout << "accepted connection.." << std::endl;
		getnameinfo((sockaddr *)&clientaddr, addrlen, clienthost, sizeof(clienthost), clientservice, 
			sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
		std::cout << "new connection: " << clienthost << ":" << clientservice << std::endl;

		connected=1;


#ifdef TEST
		int size = 100000;
		char* data = new char[size];
		int ssize = 0;
		for (int i = 0; i < 1000000; i ++) {
			int ss=0;
			while (ssize < size) {
				if (UDT::ERROR == (ss = UDT::send(sender, data + ssize, size - ssize, 0)))
				{
					std::cout << "send:" << UDT::getlasterror().getErrorMessage() << std::endl;
					break;
				}
				ssize += ss;
			}
			if (ssize < size)
				break;
		}


		std::cout << "Server " << ssize << " bytes sent" << std::endl;
		int size1 = 1024;
		char *data1 = new char[size1];
		int rs=0, rsize=0;
		while (true)
		{
			while (rsize < size1) {
				if (UDT::ERROR == (rs = UDT::recv(sender, data1 + rsize, size - rsize, 0)))
				{
					std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
					break;
				}
				std::cout << "server: " << rs << " bytes received" << std::endl;
				rsize += rs;
			}
			std::cout << "Server " << rs << " bytes received" << std::endl;
			if (rsize == size1)
				break;
		}

		std::cout << "Closing Server Socket" << std::endl;
		UDT::close(serv);
		
		delete [] data;

#endif
	}

/*********************************************************************************************************
 **		
 ** Function: 	StartUDTClient
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	1. UDT client: connects to a UDT server. Legacy code used to test UDT.
 *********************************************************************************************************/
	void CCUDT::StartUDTClient(char *server_node, char *server_service)
	{
		memset(&hints_client, 0, sizeof(struct addrinfo));
		hints_client.ai_flags = AI_PASSIVE;
		hints_client.ai_family = AF_INET;
		hints_client.ai_socktype = SOCK_STREAM;
		//hints.ai_socktype = SOCK_DGRAM;
		if (0 != getaddrinfo(NULL, server_service, &hints_client, &local))
		{
			std::cout << "incorrect network address.\n" << std::endl;
			return;
		}
		
		client = UDT::socket(local->ai_family, local->ai_socktype, local->ai_protocol);

		// Windows UDP issue
		// For better performance, modify HKLM\System\CurrentControlSet\Services\Afd\Parameters\FastSendDatagramThreshold

#ifdef _WIN32
		UDT::setsockopt(client, 0, UDT_MSS, new int(1052), sizeof(int));
#endif

		// for rendezvous connection, enable the code below
		/*
		UDT::setsockopt(client, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool));
		if (UDT::ERROR == UDT::bind(client, local->ai_addr, local->ai_addrlen)) {
		cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
		return 0;
		}
		*/


		freeaddrinfo(local);

		// connect to the server, implict bind
		if (UDT::ERROR == UDT::connect(client, peer->ai_addr, peer->ai_addrlen))
		{
			std::cout << "connect: " << UDT::getlasterror().getErrorMessage() << std::endl;
			return;
		}

		std::cout << "Client connected to server " << std::endl;
		char* data;
		int size = 100000;
		data = new char[size];
		int rs=0;
		int rsize = 0;
		while (true)
		{
			while (rsize < size)
			{
				if (UDT::ERROR == (rs = UDT::recv(client, data + rsize, size - rsize, 0)))
				{
					std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
					break;
				}
				//std::cout << "Client " << rs << " bytes received" << std::endl;
				rsize += rs;
			}
			std::cout << "Client " << rsize << " bytes received so far" << std::endl;
			if (rsize == size)
				break;
		}
		freeaddrinfo(peer);

		char* byebye;
		int size1 = 1024;
		byebye = new char[size1];
		memset((void *)byebye, (int)(0xab), 1024);
		int ssize = 0, ss=0;
		while (ssize < size1)
		{
			if (UDT::ERROR == (ss = UDT::send(client, byebye + ssize, size1 - ssize, 0)))
			{
				std::cout << "send:" << UDT::getlasterror().getErrorMessage() << std::endl;
				break;
			}
			ssize += ss;
		}


		std::cout << "Sent bye bye to server" << ssize << " bytes " << std::endl;
		delete [] data;
		std::cout << "Closing Client Socket " << std::endl;
		UDT::close(client);
	}
}

