typedef struct ThreadArg_s {
	bool r;
	bool m;
	int argc;
	char **argv;
} ThreadArg;

typedef struct JpegThreadArg_s {
	void *inBuffer;
	void *outBuffer;
	unsigned int outLen;
	int jpegLength;
	unsigned int window_width;
	unsigned int window_height;
	unsigned int out_width;
	unsigned int out_height;
} JpegThreadArg;

enum {
	BUFFER_LOCKED=1,
	BUFFER_FRESH=4 /* meaning buffer has been written into */
};
typedef struct UDTTxArg_s {
	unsigned int length;
	unsigned char *buffer;
	unsigned int flag;
	
} UDTTxArg_t;

#define MAX_QUEUE_SIZE 2
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256
#define MAX_WINDOW_WIDTH  256 * 8
#define MAX_WINDOW_HEIGHT 256 * 8
#define MAX_X 2048
#define MAX_Y 2048
#define USE_UCHAR_PIXELS 1
#define SIG1 0xab
#define SIG2 0xcd
#define SIG3 0xef

#if 0
        #define ASSERT(x)
#else
        #define ASSERT(x) \
                 if (! (x)) \
                { \
                   std::cout << "ERROR!! Assert " << #x << " failed" << std::endl; \
                   std::cout << " on line " << __LINE__  << std::endl; \
                   std::cout << " in file " << __FILE__ << std::endl; \
                }
#endif

#define MODULE_UDT 1
#define MODULE_OPENGL (1 << 1)
#define MODULE_JPEG (1 << 2)
#define MODULE_MAIN (1 << 3)
#define ALL_MODULES (MODULE_UDT | MODULE_OPENGL | MODULE_JPEG | MODULE_MAIN)
/* to see messages only for a certain module set flag as necessary */
#define MODULE_FLAG ALL_MODULES

#define INFO	1
#define WARN	(1 << 1)
#define DBG 	(1 << 2)
#define ERR	(1 << 3)

#define MSG_FLAG ERR
#define DEBUG_LOG(module_flag, msg_flag, msgstr) \
{ \
	if((module_flag) & MODULE_FLAG) { \
		if((msg_flag) & MSG_FLAG) { \
			std::cout << __FILE__<<":"<<__LINE__ << (msgstr) << std::endl; \
		} \
	} \
}

#ifdef ORG_OPENGL
class ICCOpenGL
{
public:
	virtual ~ICCOpenGL(){}; 
	virtual void StartOpenGLRenderer(void *arg) = 0; 
}; 
#endif

class ICCUDT
{
public:
	virtual ~ICCUDT(){};
	virtual void StartUDTServer()=0;
	virtual void StartUDTClientRequestReceiver()=0;
	virtual void StartUDTClient(char *server_node, char *server_service)=0;
	virtual void StartTx()=0;
};

class ICCJpeg
{
public:
	virtual ~ICCJpeg() {};
	virtual void JpegCompressPixelData(void *arg)=0;
};

#ifdef ORG_OPENGL
namespace OpenGL {ICCOpenGL *CreateOpenGL();};
#endif

namespace UDT {ICCUDT *CreateUDT();};
namespace Jpeg {ICCJpeg *CreateJpeg();};