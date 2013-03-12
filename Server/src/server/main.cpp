#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

#ifndef LINUX
#include <Queue>
#else /* OSX or WIN32 */
#include <queue>
#endif

#include <pthread.h>

#ifdef WIN32
#include <errors.h>
#endif

#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>

#ifdef LINUX 
#include <string.h>
#endif

#include <unistd.h>
#include <GL/glut.h>

#include "Interfaces.h"


#define err_abort(code,text) do { \
	fprintf (stderr, "%s at \"%s\":%d: %s\n", \
	text, __FILE__ , __LINE__ , strerror (code)); \
	abort (); \
} while (0)

#ifdef ORG_OPENGL
ICCOpenGL *openGL = NULL;
#else
extern void StartOpenGLRenderer(void *threadArg);
#endif
ICCUDT *udt = NULL;
ICCJpeg *jpeg=NULL;

int window_width = WINDOW_WIDTH; // The width of our window
int window_height = WINDOW_HEIGHT; // The height of our window
int resized_window_width;
int resized_window_height;

UDTTxArg_t *txData[MAX_QUEUE_SIZE]; 

volatile std::queue<unsigned int> q;

#ifdef MUTEX_USED
/* global mutex for our program. assignment initializes it. */ 
/* note that we use a RECURSIVE mutex, since a handler	*/ 
/* thread might try to lock it twice consecutively.	*/
pthread_mutex_t request_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t conn_request_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

/* global condition variable for our program. assignment initializes it. */ 
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;
unsigned int prodIndex=0;
unsigned int consIndex=0;
unsigned int connected=0;
char *servicePort=NULL;
unsigned int clientReqRecd=0;
unsigned char *pixelData = NULL;

void leaveProgram(int sig);

#ifdef WIN32
// helper convertor objects wchar_t to ACP and vice versa
class W2A 
{
	char  local[16];
	char* buffer;
	unsigned int n;

	void init(const wchar_t* wstr, unsigned int nu)
	{ 
		n = WideCharToMultiByte(CP_ACP,0,wstr,nu,0,0,0,0);
		buffer = (n < (16-1))? local: new char[n+1];
		WideCharToMultiByte(CP_ACP,0,wstr,nu,buffer,n,0,0);
		buffer[n] = 0;
	}
public:
	explicit W2A(const wchar_t* wstr):buffer(0),n(0)
	{ 
		if(wstr)
			init(wstr,(unsigned int)wcslen(wstr));
	}
	explicit W2A(const std::wstring& wstr):buffer(0),n(0)
	{ 
		init(wstr.c_str(),wstr.length());
	}

	~W2A() { if(buffer != local) delete[] buffer;  }
	unsigned int length() const { return n; }    
	operator const char*() { return buffer; }
};
#endif

/*********************************************************************************************************
 **		
 ** Function: 	help
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Help message for clueless users and general dudes and dudettes
 *********************************************************************************************************/
void help() {
	std::cout << "typical: [(-p|--port) ####] " << std::endl;
	std::cout << "  where:\n" << std::endl;
	std::cout << "  -p (--port) is the port number\n" << std::endl;
	std::cout << "     defaults to port 5555 if not specified.\n" << std::endl;
	std::cout << "     Port 0 is an invalid port.\n" << std::endl;
	std::cout << "  -noiwn to render to offscreen buffer no window specified\n" << std:: endl;
	std::cout << "  -multicolor to render to offscreen buffers using blue and green colors\n" << std:: endl;
}


/*********************************************************************************************************
 **		
 ** Function: 	reverseBytes
 ** Returns:	None
 ** Parameters:	None
 ** Notes: 	Reverse Bytes of a given integer
 *********************************************************************************************************/
int reverseBytes(int x) {
	x = (x & 0xffff );
#ifdef MYDEBUG
	printf("x is %d\n", x);
#endif
	int msb= ((x & 0xff00) >> 8);
	int lsb= (x & 0xff);
	x = (lsb << 8) | msb;
#ifdef MYDEBUG
	printf("x returned is %d\n", x);
#endif
	return x;
}

/*********************************************************************************************************
 **		
 ** Function: 	leaveProgram
 ** Returns:	
 ** Parameters:	int signal
 ** Notes: 	Kills BonjourServer and OpenGLBroadcastServer on SIGINT
 *********************************************************************************************************/
void leaveProgramAtExit()
{
	leaveProgram(0);
}
void leaveProgram(int signal) 
{
	std::cout << "Quitting..." << std::endl;
#if defined(OSX)
	//int r1=system("killall BonjourServer");
	int r1=system("killall dns-sd");
	int r2=system("killall OpenGLBroadcastServer");
	(r1 == r2); /* suppress compiler warning */
#elif defined(LINUX)
	std::cout << "Killing avahi-daemon" << std::endl;
	int r1=system("sudo killall avahi-daemon");
	int r2=system("killall OpenGLBroadcastServer");
	(r1 == r2); /* suppress compiler warning */
#elif defined(WIN32)
	std::cout << "Killing dns-sd.exe" << std::endl;
	system("taskkill /F /IM dns-sd.exe");
#endif
	
	return;
}

/*********************************************************************************************************
 **		
 ** Function: 	UDTClientThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Starts UDT client thread
 *********************************************************************************************************/
void *UDTClientThread(void *arg)
{
	std::cout << "Starting UDT Client thread " << std::endl;
	udt->StartUDTClient((char *)"local", (char *)"5555");

	return NULL;
}

/*********************************************************************************************************
 **		
 ** Function: 	UDTTxThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Returns NULL if not connected. Else, transmit forever.
 *********************************************************************************************************/
void *UDTTxThread(void *arg)
{
	//std::cout << "Starting UDT Tx thread " << std::endl;
	if(!connected) return NULL;
	while (true) {
	  udt->StartTx();
	}

	return NULL;
}

/*********************************************************************************************************
 **		
 ** Function: 	UDTServerhread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Starts UDT server
 *********************************************************************************************************/
void *UDTServerThread(void *arg)
{
	std::cout << "Starting UDT Server thread " << std::endl;
	udt->StartUDTServer();

	return NULL;
}

/*********************************************************************************************************
 **		
 ** Function: 	UDTClientRequestReceiverThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Starts UDT client receiver thread
 *********************************************************************************************************/
void *UDTClientRequestReceiverThread(void *arg)
{
	std::cout << "Starting UDT Request receiver thread " << std::endl;
	udt->StartUDTClientRequestReceiver();

	return NULL;
}

/*********************************************************************************************************
 **		
 ** Function: 	OpenGLRendererThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Starts OpenGL renderer thread
 *********************************************************************************************************/
void *OpenGLRendererThread(void *arg)
{
	//openGL->StartOpenGLRenderer(arg);
	StartOpenGLRenderer(arg);
	return NULL;
}


/*********************************************************************************************************
 **		
 ** Function: 	fexist
 ** Returns:	int
 ** Parameters:	char *filename
 ** Notes: 	returns 1 if file exists, 0 otherwise
 *********************************************************************************************************/
#if defined(LINUX) || defined(WIN32)
 int fexist(char *filename) 
{
	struct stat buffer;
	if(stat (filename, &buffer)) return 0;
	return 1;
}
#endif
/*********************************************************************************************************
 **		
 ** Function: 	BonjourThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Thread for starting bonjour programs on various platforms.
 *********************************************************************************************************/
int ExistsStrInFile(char *filename, const char * str) 
{
	FILE *fp=fopen(filename,"r");
	char tmp[1024]={0x0};
	int found =0;
	while(fp!=NULL && fgets(tmp, sizeof(tmp),fp)!=NULL)
	{
		if (strstr(tmp, str)) {
			found = 1;
		}
	}
	if(fp!=NULL) fclose(fp);
	return found;
}
	
/*********************************************************************************************************
 **		
 ** Function: 	BonjourThread
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Thread for starting bonjour programs on various platforms.
 *********************************************************************************************************/
void *BonjourThread(void *arg)
{
#if defined(WIN32)
	int serverPort = atoi(servicePort);
	serverPort = reverseBytes(serverPort);
	char cmd[1024];
	char dns_sd_path[1024];
	char tmp_path[1024];
#ifndef BONJOUR_BIN_DIR
#define BONJOUR_BIN_DIR "."
#endif
	memset(cmd, 0, 1024);
	memset(dns_sd_path, 0, 1024);
	memset(tmp_path, 0, 1024);
	sprintf(dns_sd_path, "%s/dns-sd.exe", BONJOUR_BIN_DIR);
	if(!fexist((char *)dns_sd_path)) {
		std::cout << cmd << "dns-sd.exe does not exist" << std::endl;
		std::cout << "Please install Bonjour service" << std::endl;
		std::cout << "Bonjour will not run" << std::endl;
		return NULL;
	}
	
	sprintf(tmp_path, "%s/tmp", BONJOUR_BIN_DIR);
    /* now find if the Bojnour service is running */
	sprintf(cmd, "sc query \"Bonjour Service\" | findstr \"RUNNING\" > %s", tmp_path); 
	system((const char *)cmd);
	
	if(!ExistsStrInFile((char *)tmp_path, (const char *)"RUNNING") ) {
		std::cout << tmp_path << " " << "Bonjour service not running" << std::endl;
		std::cout << "Please install Bonjour service" << std::endl;
		std::cout << "Bonjour will not run" << std::endl;
		sprintf(cmd, "del %s", tmp_path);
		system((const char *)cmd);
		return NULL;
	}
	sprintf(cmd, "del %s", tmp_path);
	std::cout << "Bonjour server at port: " << serverPort << std::endl;
	sprintf(cmd, "%s -R \"Crack Creative Messaging Service\" _CrackCreativeMessagingService._udp local %d", dns_sd_path, serverPort);
	system((const char *)cmd); 
	/* ShellExecute(GetDesktopWindow(), "open", cmd, NULL, NULL, SW_SHOWNORMAL); */

#elif defined(LINUX)
	int serverPort = atoi(servicePort);
	serverPort = reverseBytes(serverPort);
	char cmd[8192];
	memset(cmd, 0, 8192);
	FILE *file=NULL;
	char cmd1[1024];
	char script[1024];
	memset(cmd1, 0, 1024);
	memset(script, 0, 1024);
	
	/* check if the avahi daemon exists */
	if(!fexist((char *)"/etc/init.d/avahi-daemon")) {
		std::cout << "Please install avahi daemon" << std::endl;
		std::cout << "Bonjour will not run" << std::endl;
		return NULL;
	}

	sprintf(cmd, "%s\n","<?xml version=\"1.0\" standalone='no'?><!--*-nxml-*-->");
	strcat(cmd, "<!DOCTYPE service-group SYSTEM \"avahi-service.dtd\">\n");
	strcat(cmd, "<service-group>\n");
	strcat(cmd, "<name>Crack Creative Messaging Service</name>\n");
	strcat(cmd, "<service>\n");
	strcat(cmd, "<type>_CrackCreativeMessagingService._udp</type>\n");
	sprintf(cmd1, "<port>%d</port>\n", serverPort);
	strcat(cmd, cmd1);
	sprintf(cmd1, "%s", "</service>\n</service-group>\n");
	strcat(cmd, cmd1);
	
#ifndef BONJOUR_BIN_DIR
#define BONJOUR_BIN_DIR "."
#endif
	sprintf(script, "%s/CrackCreativeMessagingService.service", BONJOUR_BIN_DIR);

	std::cout << "script: " << script << std::endl;
	file = fopen(script,"w+"); 
	if(!file) {
		std::cout << "Could not open script file" << std::endl;
		std::cout << "Bonjour will not run" << std::endl;
		return NULL;
	}
	fprintf(file,"%s",cmd); /*writes*/
	fclose(file);
	/* check if the avahi daemon exists */
	if(!fexist((char *)"/etc/avahi/services")) {
		std::cout << "/etc/avahi/services does not exist" << std::endl;
		std::cout << "Please install avahi daemon" << std::endl;
		std::cout << "Bonjour will not run" << std::endl;
		return NULL;
	}
	sprintf(cmd1, "sudo cp %s/CrackCreativeMessagingService.service /etc/avahi/services", BONJOUR_BIN_DIR);
	int r1=system(cmd1);
	int r2=system("sudo /etc/init.d/avahi-daemon restart");
	std::cout << "Avahi result " << r1 << "," << r2 << std::endl;
#elif defined(OSX)
	int serverPort = atoi(servicePort);
	char cmd[1024];
	
	memset(cmd, 0, 1024);
#ifndef BONJOUR_BIN_DIR
#define BONJOUR_BIN_DIR "../../bin/mac"
#endif
	serverPort = reverseBytes(serverPort);
	std::cout << "Bonjour server at port: " << serverPort << std::endl;
	sprintf(cmd, "dns-sd -R \"Crack Creative Messaging Service\" _CrackCreativeMessagingService._udp local %d &", serverPort);
	//sprintf(cmd, "open %s/BonjourServer.app --args -p %d", BONJOUR_BIN_DIR, serverPort);
	system((const char *)cmd);
#endif
	std::cout << "Bonjour Thread Completed " << std::endl;
	return NULL;
}

/*********************************************************************************************************
 **		
 ** Function: 	JpegCompressPixelData
 ** Returns:	void *
 ** Parameters:	void *arg
 ** Notes: 	Compress Pixel Data in the JPEG module
 *********************************************************************************************************/
void *JpegCompressPixelData(void *arg)
{
	//std::cout << "2 " << ((JpegThreadArg *)arg)->window_width << " X " <<  ((JpegThreadArg *)arg)->window_height << std::endl;
	jpeg->JpegCompressPixelData(arg);
	return NULL;
}

#if defined(WIN32)
int WINAPI WinMain(
	HINSTANCE	hInstance,	// Instance
	HINSTANCE	hPrevInstance,	// Previous Instance
	LPSTR lpCmdLine,		// Command Line Parameters
	int nCmdShow)		// Window Show State

{

	LPWSTR *szArgList;  
	bool renderInBackground= FALSE;
	bool	multicolor=FALSE;
	int argCount = 0;  

	const char *sp = (const char *)W2A(L"5555");
	servicePort = (char *)sp;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);  

	if (szArgList == NULL)  
	{  

		MessageBox(NULL, "Unable to parse command line", "Error", MB_OK); 

	}  
	else {

		for(int i = 0; i < argCount; i++)  
		{  
#ifdef SERVER_DEBUG
			MessageBoxW(NULL, szArgList[i], L"Arglist contents", MB_OK);  
#endif
			if((!wcscmp(szArgList[i], L"-nowin")) || (!wcscmp(szArgList[i], L"-NOWIN")) ) {
				renderInBackground=TRUE;

			}
			if((!wcscmp(szArgList[i], L"-multicolor")) || (!wcscmp(szArgList[i], L"-MULTICOLOR")) ) {
				multicolor=TRUE;

			}
			if((!wcscmp(szArgList[i], L"-p")) || (!wcscmp(szArgList[i], L"-port")) ) {
				const char *sp = (const char *)W2A(szArgList[++i]);
				servicePort = (char *)sp;
			}
			if((!wcscmp(szArgList[i], L"-h")) || (!wcscmp(szArgList[i], L"-help")) ||  (!wcscmp(szArgList[i], L"--help")) ) {
				help();
				exit(1);

			}
		}  
	}

	LocalFree(szArgList);  
#elif defined(LINUX) || defined(OSX)
int main(int argc, char *argv[])
{
	bool renderInBackground= false;
	bool	multicolor=false;
	servicePort = (char *)malloc(256 * sizeof(char));
	memset(servicePort, 0, 256 * sizeof(char));
	strcpy(servicePort, "5555");
	if (argc > 1) {
		for (int count = 1; count < argc; count++) {
			printf("argv[%d] = %s\n", count, argv[count]);
			if((!strcmp(argv[count], "-nowin")) || (!strcmp(argv[count],
				"-NOWIN"))) {
					renderInBackground=true;
			}
			if((!strcmp(argv[count], "-multicolor")) ||
				(!strcmp(argv[count], "-MULTICOLOR"))) {
					multicolor=true;
			}
			if((!strcmp(argv[count], "-p")) || (!strcmp(argv[count],
				"-port"))) {
					strcpy(servicePort, argv[++count]);
			}
			if((!strcmp(argv[count], "-h")) || (!strcmp(argv[count],
				"-help")) || (!strcmp(argv[count], "--help"))) {
					help(); exit(2);
			}
		}
	}
	(void) signal(SIGHUP,leaveProgram);
	atexit(leaveProgramAtExit);
#endif
	(void) signal(SIGINT,leaveProgram);
	for(int i=0; i < MAX_QUEUE_SIZE; i++) {
		txData[i] = (UDTTxArg_t *)malloc(sizeof(UDTTxArg_t));
		txData[i]->length=0;
		txData[i]->buffer = (unsigned char *) malloc(4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT);
		memset(txData[i]->buffer, 0, 4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT );
		txData[i]->flag=0;

	}
	pixelData = (unsigned char *) malloc(4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT);
	memset(pixelData, 0, 4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT);
	
	// create and start UDT module
	udt = UDT::CreateUDT();

#ifdef ORG_OPENGL
	// create and setup OpenGL module for spinning cube
	openGL = OpenGL::CreateOpenGL();
#endif

	ThreadArg arg0;
	arg0.r = renderInBackground;
	arg0.m=multicolor;
	arg0.argc=0;
	arg0.argv=NULL;
#ifdef OSX
	arg0.argc=argc;
	arg0.argv=argv;
#endif
	// create JPEG modile
	jpeg = Jpeg::CreateJpeg();
	clientReqRecd=0;

	pthread_t id[5];

#ifdef MUTEXES_USED
	struct sched_param mainThreadSchedParam;
	
	pthread_attr_t mainThreadAttr;
	pthread_attr_init(&mainThreadAttr); 
	pthread_attr_setschedpolicy(&mainThreadAttr, SCHED_RR) ;
	pthread_attr_getschedparam(&mainThreadAttr, &mainThreadSchedParam) ;
	int sysMaxPriority = sched_get_priority_max(SCHED_RR); 
	int sysMinPriority = sched_get_priority_min(SCHED_RR); 
	int mainThrPriority = mainThreadSchedParam.sched_priority;
	/* the below is not used. Useful when using mutexes */
	std::cout << sysMaxPriority <<" "<< sysMinPriority <<" "<< mainThrPriority << std::endl; 
#endif
	// Create OpenGL Renderer Thread
	static_cast<void>(::pthread_create(&id[0], 0, OpenGLRendererThread, static_cast<void*>(&arg0)));
	// Create Bonjour Thread
	static_cast<void>(::pthread_create(&id[1], 0, BonjourThread, static_cast<void*>(&arg0)));
	
	// start UDT server
	udt->StartUDTServer();
	// create UDT Client Receiver thread
	static_cast<void>(::pthread_create(&id[2], 0, UDTClientRequestReceiverThread, static_cast<void*>(&arg0)));
	// create UDT client Tx thread
	static_cast<void>(::pthread_create(&id[3], 0, UDTTxThread, static_cast<void*>(&arg0)));
	
	// Wait indefinitely for all threads to finish.
	static_cast<void>(::pthread_join(id[0], 0));
	static_cast<void>(::pthread_join(id[2], 0));
	static_cast<void>(::pthread_join(id[1], 0));
	static_cast<void>(::pthread_join(id[3], 0));
	
	// cleanup
	for(int i=0; i < MAX_QUEUE_SIZE; i++) {

		free(txData[i]->buffer);
		free(txData[i]);
	}

	return 0;
}
