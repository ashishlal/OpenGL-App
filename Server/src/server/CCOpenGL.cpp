#include "CCOpenGL.h"
#include <iostream>
#ifndef LINUX
#include <Queue>
#else
#include <queue>
#include <cstring>
#endif
#include <stdlib.h>
extern "C" {
#define XMD_H
#include <jpeglib.h>
}

#if defined(OSX) || defined(LINUX)
#include <ImageMagick/Magick++.h>
#include <ImageMagick/Magick++/Blob.h>
#include <ImageMagick/Magick++/Image.h>
#elif defined(WIN32)
#include <Magick++.h>
#include <Magick++/Blob.h>
#include <Magick++/Image.h>
#endif
using namespace Magick;

#if defined(LINUX) || defined(OSX)
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#endif

extern std::queue<unsigned int> q;
extern UDTTxArg_t *txData[MAX_QUEUE_SIZE]; 
extern void leaveProgram(int);

extern pthread_mutex_t request_mutex ;
extern int window_width; // The width of our window
extern int window_height; // The height of our window
extern int resized_window_width;
extern int resized_window_height;
extern unsigned int prodIndex;
extern unsigned int consIndex;
extern void *UDTTxThread(void *arg);
extern void *JpegCompressPixelData(void *arg);
extern unsigned int connected;
extern unsigned int clientReqRecd;

namespace OpenGL {

	//#pragma comment(lib, "glew32.lib")
	bool resized_win_flag=false;
	bool* keyStates = new bool[256]; // Create an array of boolean values of length 256 (0-255)
	bool	renderInBackground=FALSE; //donot show window but run in background
	bool	multicolor=FALSE;
	unsigned int fbo; // The frame buffer object
	unsigned int fbo_render[2]; // The texture and depth object to write our frame buffer object to

	unsigned int fboNext; // The frame buffer object
	unsigned int fboNext_render[2]; // The texture and depth object to write our frame buffer object to


#define MYCOLOR 0
#define MYDEPTH 1

	float rotation_degree = 0.0f; 
	float rotation_degree2 = 0.0f;


/*********************************************************************************************************
 **		
 ** Function: 	CreateOpenGL
 ** Returns:	ICCOpenGL *, pointer to OpenGL interface
 ** Parameters:	None
 ** Notes: This function returns a pointer to OpenGL interface
 *********************************************************************************************************/
	ICCOpenGL *CreateOpenGL() 
	{
		return new CCOpenGL();
	}

/*********************************************************************************************************
 **		
 ** Function: 	StartOpenGLRenderer
 ** Returns:	void
 ** Parameters:	void *threadArg (renderInBackground, multicolor)
 ** Notes: This function sets up the frame buffers, depth and texture buffers for spinning cube.
 *********************************************************************************************************/
	void CCOpenGL::StartOpenGLRenderer(void *threadArg)
	{
		ThreadArg *arg = (ThreadArg *)threadArg;
		renderInBackground=arg->r;
		multicolor = arg->m;
		myargc=arg->argc;
		for(int i=0; i < myargc; i++)
			myargv[i]=arg->argv[i];
		setup();
	}
	
/*********************************************************************************************************
 **		
 ** Function: 	initFrameBufferDepthBuffer
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: This function initializes depth buffer
 *********************************************************************************************************/
	void CCOpenGL::initFrameBufferDepthBuffer(void) 
	{

		int WIDTH=window_width * 8;
		int HEIGHT=window_height * 8;
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_render[MYDEPTH]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

	}

/*********************************************************************************************************
 **		
 ** Function: 	initFrameBufferTexture
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: This allocates storage for 2 render buffers (color and depth) and inits color buffer.
 *********************************************************************************************************/
	void CCOpenGL::initFrameBufferTexture(void) 
	{
		int WIDTH=window_width * 8;
		int HEIGHT=window_height * 8;
		glGenRenderbuffersEXT(2, fbo_render);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_render[MYCOLOR]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, WIDTH, HEIGHT);
	}

/*********************************************************************************************************
 **		
 ** Function: 	initFrameBuffer
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Allocates storage for a frame buffer, color and depth buffers
 *********************************************************************************************************/
	void CCOpenGL::initFrameBuffer(void) 
	{

		glGenFramebuffersEXT(1, &fbo);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
		GLenum error = glGetError();
		if(error) std::cout << "FBO init failed1 " <<  gluErrorString(error) << std::endl;

		glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, fbo_render[MYCOLOR]); 
		glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_render[MYDEPTH], 0);
		error = glGetError();
		if(error) std::cout << "FBO init failed2 " <<  gluErrorString(error) << std::endl;

		glEnable(GL_DEPTH_TEST);
		error = glGetError();
		if(error) std::cout << "FBO init failed 3" <<  gluErrorString(error) << std::endl;

		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	}

/*********************************************************************************************************
 **		
 ** Function: 	initFrameBufferDepthBufferB
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Initializes the depth buffer B
 *********************************************************************************************************/
	void CCOpenGL::initFrameBufferDepthBufferB(void) 
	{
		int WIDTH=window_width * 8;
		int HEIGHT=window_height * 8;
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fboNext_render[MYDEPTH]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

	}

/*********************************************************************************************************
 **		
 ** Function: 	initFrameBufferTextureB
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Initializes the texture buffer B
 *********************************************************************************************************/
	void CCOpenGL::initFrameBufferTextureB(void) 
	{
		int WIDTH=window_width * 8;
		int HEIGHT=window_height * 8;
		glGenRenderbuffersEXT(2, fboNext_render);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fboNext_render[MYCOLOR]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, WIDTH, HEIGHT);
	}

/*********************************************************************************************************
 **		
 ** Function: 	initFrameBufferB
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Allocates frame buffer B, initializes it.
 *********************************************************************************************************/
	void CCOpenGL::initFrameBufferB(void) 
	{
		glGenFramebuffersEXT(1, &fboNext);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fboNext);

		glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, fboNext_render[MYCOLOR]); 
		glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fboNext_render[MYDEPTH], 0);
		glEnable(GL_DEPTH_TEST);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	}

/*********************************************************************************************************
 **		
 ** Function: 	initA
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Initializes Frame Texture and depth render buffers, then allocates and inits the frame buffer
 *********************************************************************************************************/
	void CCOpenGL::initA(void) 
	{
		// Initialize the render buffers
		initFrameBufferTexture();
		GLenum error = glGetError(); 
		if(error) std::cout << "Texture Render Buffer init failed " <<  gluErrorString(error) << std::endl;

		initFrameBufferDepthBuffer();
		error = glGetError(); 
		if(error) std::cout << "Depth Render Buffer init failed " <<  gluErrorString(error) << std::endl;

		// Initialize the FBO
		initFrameBuffer();
		error = glGetError(); 
		if(error) std::cout << "FBO init failed " <<  gluErrorString(error) << std::endl;

	}

/*********************************************************************************************************
 **		
 ** Function: 	initB
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Initializes Frame Texture and depth render buffers, then allocates and inits the frame buffer
 *********************************************************************************************************/
	void CCOpenGL::initB(void) 
	{
		// Initialize the render buffers
		initFrameBufferTextureB();
		GLenum error = glGetError(); 
		if(error) std::cout << "Texture Render Buffer init failed " <<  gluErrorString(error) << std::endl;

		initFrameBufferDepthBufferB();
		error = glGetError(); 
		if(error) std::cout << "Depth Render Buffer init failed " <<  gluErrorString(error) << std::endl;

		// Initialize the FBO
		initFrameBufferB();
		error = glGetError(); 
		if(error) std::cout << "FBO init failed " <<  gluErrorString(error) << std::endl;

	}

/*********************************************************************************************************
 **		
 ** Function: 	init
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: Initializes 2 frame buffers used
 *********************************************************************************************************/
	void CCOpenGL::init(void) 
	{
		//glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture
		glEnable(GL_DEPTH_TEST); // Enable depth testing
		initA();
		initB();
	}

/*********************************************************************************************************
 **		
 ** Function: 	keyOperations
 ** Returns:	void
 ** Parameters:	void 
 ** Notes: 
 *********************************************************************************************************/
	void CCOpenGL::keyOperations (void) 
	{
		if (keyStates['a']) { // If the a key has been pressed
			// Perform 'a' key operations
		}
	}

/*********************************************************************************************************
 **		
 ** Function: 	renderCubeScene
 ** Returns:	void
 ** Parameters:	unsigned int currFBO 
 ** Notes: Binds the given frame buffer object for rendering, clears depth and color buffer,
 ** renders a spinning cube
 *********************************************************************************************************/
	void CCOpenGL::renderCubeScene (unsigned int currFBO) 
	{

		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, currFBO); // Bind our frame buffer for rendering
		glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states
		glViewport(0, 0, window_width, window_height); // Set the size of the frame buffer view port
		//glViewport(0, 0, resized_window_width, resized_window_height); // Set the size of the frame buffer view port
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers

		glLoadIdentity(); // Reset the modelview matrix

		glTranslatef(0.0f, 0.0f, -5.0f); // Translate back 5 units
		
		glRotatef(rotation_degree, 1.0f, 1.0f, 0.0f); // Rotate according to our rotation_degree value
		glRotatef(rotation_degree2, 0.0f, 0.0f, 1.0f);
		
		
		//glutSolidCube(2.0f); // Render a Cube
		glBegin(GL_QUADS);
		glColor4f(0.0f,1.0f,0.0f, 1.0f);            // Set The Color To Green
		glVertex3f( 1.0f, 1.0f, 1.0f);            // Bottom Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);            // Bottom Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);            // Top Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f,-1.0f);            // Top Right Of The Quad (Top)

		glColor3f(1.0f,0.5f,0.0f);            // Set The Color To Orange
		glVertex3f( 1.0f,-1.0f,-1.0f);            // Bottom Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);            // Bottom Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);            // Top Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f, 1.0f);            // Top Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);            // Set The Color To Red
		glVertex3f( 1.0f,-1.0f, 1.0f);            // Bottom Right Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);            // Bottom Left Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);            // Top Left Of The Quad (Front)
		glVertex3f( 1.0f, 1.0f, 1.0f);            // Top Right Of The Quad (Front)

		glColor3f(1.0f,1.0f,0.0f);            // Set The Color To Yellow
		glVertex3f( 1.0f, 1.0f,-1.0f);            // Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);            // Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);            // Bottom Right Of The Quad (Back)
		glVertex3f( 1.0f,-1.0f,-1.0f);            // Bottom Left Of The Quad (Back)

		glColor3f(0.0f,0.0f,1.0f);            // Set The Color To Blue
		glVertex3f(-1.0f,-1.0f, 1.0f);            // Bottom Right Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);            // Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);            // Top Left Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f, 1.0f);            // Top Right Of The Quad (Left)

		glColor3f(1.0f,0.0f,1.0f);            // Set The Color To Violet
		glVertex3f( 1.0f,-1.0f,-1.0f);            // Bottom Right Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);            // Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);            // Top Left Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f,-1.0f);            // Top Right Of The Quad (Right)
		glEnd();
		glPopAttrib(); // Restore our glEnable and glViewport states

		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0); // Unbind our fbo

		rotation_degree += 0.5f;
		if (rotation_degree > 360.0f)
			rotation_degree = 0.0f;
		rotation_degree2 += 0.125f;
		if (rotation_degree2 > 360.0f)
			rotation_degree2 = 0.0f;
	}

void CCOpenGL::Flip8(unsigned char *b, unsigned int w, unsigned int h)
{
    __asm {
	mov r8, rdi //b
	mov r9, rsi //w
	mov r10,rdx //h
	mov r11, 0 // h <- 0
	mov r12, 0 // w<- 0
	outloop:
	//obuf:
		mov r13, r9 // w
		mov r13, 4 //w * 4
		imul r13, r11 // w * 4 *h
		add r13, r8 // obuf = b + (h * w * 4)
	//inbuf:
		mov rbx, r9 // w
		imul rbx, 4
		mov r15, r10
		dec r15
		sub r15, r11 // r15 = (h-1) 
		imul r15, rbx // r15 = (h-1) *w
		add r15, r8 // r15 = b + (h-1) * w
	inloop:
		mov edi. [r15]
		mov esi, [r13]
		mov [r15], esi // swap 4 bytes
		mov [r13], edi // swap 4 bytes
		inc r15, 4
		inc r13, 4
		inc r12
		cmp r12, r9 // r12 = o to w-1
		jb  inloop
		inc r11 // inc h
		cmp r11, r10
		jb outloop
    }
    

}

/*********************************************************************************************************
 **		
 ** Function: 	Flip4
 ** Returns:	buffer
 ** Parameters:	void *b, unsigned int w, unsigned int h
 ** Notes: Flips a buffer b of width w and height h along y-axis, 4 bytes a time.
 **
 *********************************************************************************************************/

void Flip4(unsigned char *b, unsigned int w, unsigned int h)
{
    __asm {
        mov r8, rdi //b
        mov r9, rsi //w
        mov r10,rdx //h
        mov r11, 0
        mov r14, r9
        imul r14, r10
        mov r15, r14
        shr r15, 1
        mov r12, 0
        dec r10
        label:
            mov r13, r11
            imul r13, 4
            cmp r13, r9
            jae label1
            mov r14 rdx
            dec r14
            sub r14, r10
            imul r14, r9
            add r14, r13
            mov r13, r14
            
            mov r14, r10 // load h-1 in r14
            imul r14, r9 //  (h-1) * w
            mov rbx, r8 // loadbuffer address in rbx
            add rbx, r14 // b + [(h-1) * w]
            mov r12, r11
            imul r12, 4
            add rbx, r12 // next 4 bytes
            mov eax, [rbx] // read 4 bytes
            
            mov rcx, r8 // load buffer address in rcx
            add rcx, r13 //nth iteration
            mov edi, [rcx] // read nth 4 bytes
            
            mov [rcx], eax // swap 8 bytes, 
            mov [rbx], edi // swap 8 bytes
            
            inc r11 // increment, next iteration
            mov r12, r11
            imul r12, 4
            mov r14, r9
            cmp r12, r14
            jb label
        label1:
            mov r11 0
            dec r10
            mov r15, rdx
            shr r15, 1
            cmp r10, r15
            jae label
    }
}

/*********************************************************************************************************
 **		
 ** Function: 	readFromFB
 ** Returns:	void
 ** Parameters:	unsigned int currFBO 
 ** Notes: 	1. binds the FBO to a READ_FRAMEBUFFER_EXT
 ** 		2. does nothing if no UDT connection
 ** 		3. Looks for a free buffer, locks it for writing
 ** 		4. Reads the raw pixel data
 ** 		5. Sets up JPEG compression arguments.
 ** 		6. Calls function to compress JPEG pixel data
 ** 		7. Unbinds the FBO
 *********************************************************************************************************/
	void CCOpenGL::readFromFB (unsigned int currFBO) 
	{
		unsigned char *data=NULL;
		static unsigned int bufferIndex=0;
		bool unlockedBufferFound=false;
		unsigned int unlockedBufferIndex=0;
		//if(!connected) return;
		if(resized_win_flag) return;
		//if(!clientReqRecd) return;
#if 1
		static int x=0;
		if(x >= 2) return;
		//if(resized_win_flag)
		x++;
#endif
		glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, currFBO ); // bind our fbo

		for(int i=0; (i < MAX_QUEUE_SIZE) && (!unlockedBufferFound); i++, bufferIndex = (bufferIndex+i)%MAX_QUEUE_SIZE) {
			if(txData[bufferIndex]->flag & BUFFER_LOCKED) continue;
			unlockedBufferFound=true;
			unlockedBufferIndex = bufferIndex;
			break;
		}
		if(!unlockedBufferFound) {
			std::cout << "No buffer found.." << std::endl;
			return;
		}
		/* std::cout << "Buffer found.." << std::endl; */
		ASSERT(unlockedBufferIndex == bufferIndex);
		//if(clientReqRecd)
		if(1) 
		{
#ifdef ORIGINAL_CODE			
			data = (unsigned char *)malloc(4 * window_height * window_width);
			memset(data, 0, (4 * window_width * window_height));

			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, window_width, window_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			
			JpegThreadArg arg;
			txData[bufferIndex]->flag |= BUFFER_LOCKED;
			arg.inBuffer = (void *)data;
			arg.outBuffer = (void *)(txData[bufferIndex]->buffer);
			arg.outLen = 4 * window_width * window_height;
			arg.window_width = window_width;
			arg.window_height = window_height;
#else
			unsigned int buf_win_width1 = resized_window_width;
			unsigned int buf_win_height1 = resized_window_height;
			unsigned int buf_win_width = window_width * 8;
			unsigned int buf_win_height = window_height * 8;
			
			data = (unsigned char *)malloc(4 * buf_win_height * buf_win_width);
			
			memset(data, 0, (4 * buf_win_width * buf_win_height));

			//glPixelStorei(GL_PACK_ALIGNMENT, 4);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			
			// copy starts at lower left corner of screen

			glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, data);

			//Flip8(data, buf_win_width1, buf_win_height1);
#if 1
	{
	int w=(int)buf_win_width1;
	int h=(int)buf_win_height1;
	
        for(int j=0; j < (int)h; j++) {
		for(int i=0; i < (int)(w/8); i+=8) {
			int k=j*8;
			unsigned char p[2048];
			char *buf = (char *)data;
			sprintf((char *)p, "%02x %02x %02x %02x %02x %02x %02x %02x",
				buf[i+k], buf[i+k+1], buf[i+k+2], buf[i+k+3], buf[i+k+4], buf[i+k+5], buf[i+k+6], buf[i+k+7]);
			std::cout << p << std::endl;
			
			//std::cout << std::hex <<  buf[i+k] << " " << buf[i+k+1] << " " << buf[i+k+2] << " " << buf[i+k+3] << " " <<  buf[i+k+4] << " " <<  buf[i+k+5] << " " << buf[i+k+6] << " " <<  buf[i+k+7]  << std::endl;
		}
	}
	std::cout << "--------1 ------end------" << std::endl;
    __asm {
        mov r8, data //b
        mov r9, buf_win_width1 //w
        mov r10,buf_win_height1 //h
       
        
        mov r11, 0
        mov r14, r9
        imul r14, r10
        mov r15, r14
        shr r15, 1
        mov r12, 0
        dec r10
        label:
            mov r13, r11
            imul r13, 8
            cmp r13, r9
            jae label1
            mov r14 rdx
            dec r14
            sub r14, r10
            imul r14, r9
            add r14, r13
            mov r13, r14
            mov r15, r8    // load buffer address in rcx
            add r15, r13   //nth iteration
            mov r12, [r15] // read nth 8 bytes
	    
            mov r14, r10   // load h-1 in r14
            imul r14, r9   //  (h-1) * w
            mov r13, r8    // loadbuffer address in rbx
            add r13, r14   // b + [(h-1) * w]
            mov r12, r11
            imul r12, 8
            add r13, r12   // next 8 bytes
            mov r14, [r13] // read 8 bytes
            
            mov [r15], r14 // swap 8 bytes, 
            mov [r13], r12 // swap 8 bytes
            
            inc r11 // increment, next iteration
            mov r12, r11
            imul r12, 8
            mov r14, r9
            cmp r12, r14
            jb label
        label1:
            mov r11 0
            dec r10
            mov r15, rdx
            shr r15, 1
            cmp r10, r15
            jae label
    }
    
    for(int j=0; j < (int)h; j++) {
		for(int i=0; i < (int)(w/8); i+=8) {
			int k=j*8;
			unsigned char p[2048];
			char *buf = (char *)data;
			sprintf((char *)p, "%02x %02x %02x %02x %02x %02x %02x %02x",
				buf[i+k], buf[i+k+1], buf[i+k+2], buf[i+k+3], buf[i+k+4], buf[i+k+5], buf[i+k+6], buf[i+k+7]);
			std::cout << p << std::endl;
			
			//std::cout << std::hex << buf[i+k] << " " << buf[i+k+1] << " " << buf[i+k+2] << " " << buf[i+k+3] << " " <<  buf[i+k+4] << " " <<  buf[i+k+5] << " " << buf[i+k+6] << " " <<  buf[i+k+7]  << std::endl;
		}
	}
	std::cout << "--------2 ------end------" << std::endl;
    }
#endif
			//Blob blob;
			//unsigned int l = (buf_win_width1 > WINDOW_WIDTH)?buf_win_width1:WINDOW_WIDTH;
			//unsigned int h = (buf_win_height1 > WINDOW_HEIGHT)?buf_win_height1:WINDOW_HEIGHT;
				
			/* void *buf = malloc(buf_win_width * buf_win_height * 4); */
			//void *buf = malloc(l * h * 4);
			JpegThreadArg arg;
			unsigned char bReshaped=0;
			txData[bufferIndex]->flag |= BUFFER_LOCKED;
			arg.inBuffer = (void *)data;
#ifdef NOT_USED
			//if((buf_win_width1 != WINDOW_WIDTH) || (buf_win_height1 != WINDOW_HEIGHT))
			if (0)
			{
				//unsigned int l = (buf_win_width > WINDOW_WIDTH)?buf_win_width:WINDOW_WIDTH;
				//unsigned int h = (buf_win_height > WINDOW_HEIGHT)?buf_win_height:WINDOW_HEIGHT;
				
				
				memcpy(buf, data, (l * h * 4));
				Blob rgbaBlob((const void *)(buf), (l * h *4));
				Image myMagickImage(buf_win_width, buf_win_height, "RGBA", CharPixel, data);
				
				//myMagickImage.magick("RGBA");
				//myMagickImage.read(rgbaBlob);
				
				myMagickImage.filterType(CubicFilter); 
				myMagickImage.resolutionUnits(PixelsPerInchResolution);
				unsigned int tot_cols = buf_win_width1;
				unsigned int tot_rows = buf_win_height1;
				//unsigned int tot_cols = WINDOW_WIDTH;
				//unsigned int tot_rows = WINDOW_HEIGHT;
				char extnt[1024];
				memset(extnt, '\0', 1024);
				if(buf_win_height1 > buf_win_width1) {
					tot_cols = (unsigned int)(((float)tot_cols + (float)((float)buf_win_height1/(float)buf_win_width1)));
				}
				else if(buf_win_width1 > buf_win_height1) {
					tot_rows = (unsigned int)(((float)tot_rows + (float)((float)buf_win_width1/(float)buf_win_height1)));
				}
				if((tot_cols > (8 * WINDOW_WIDTH)) || (tot_rows > (8 *WINDOW_HEIGHT))) {
					sprintf(extnt, "%dx%d!", tot_cols, tot_rows);
				}
				else {
					sprintf(extnt, "%dx%d!", buf_win_width1, buf_win_height1);
				}
				
				Geometry myGeomExtnt((const char *)extnt);
				myMagickImage.backgroundColor(Magick::Color(0,0,0));
				if((buf_win_width1 > ( 8 * WINDOW_WIDTH)) || (buf_win_height1 > (8 * WINDOW_HEIGHT))) {
					
					myMagickImage.resize(myGeomExtnt);
		#if 1
					std::cout << "bw: " << buf_win_width << "\n" << "bh :" << buf_win_height << "\n" << std::endl;
					std::cout << "tr: " << tot_rows << "\n" << "tc : " << tot_cols << "\n"  << std::endl;
					std::cout << "geom " << extnt << "....length-1: ..." << std::endl;
		#endif
				}
				else {
		#if 1
					std::cout << "....length-2: ..." << std::endl;
		#endif
					sprintf(extnt, "%dx%d!", WINDOW_WIDTH, WINDOW_HEIGHT);
					Geometry myGeomRsz((const char *)extnt);
		#if defined(OSX) || defined(WIN32)
					myMagickImage.extent(myGeomRsz, CenterGravity);
					//myMagickImage.resize(myGeomRsz);
		
		#elif defined(LINUX) 
					myMagickImage.extent(myGeomRsz);
		#endif
		
				}
				myMagickImage.write(&blob );
				arg.inBuffer = (void *)(blob.data());
			}
			
#endif			
			
			//if((buf_win_height1 != WINDOW_HEIGHT) || (buf_win_width1 != WINDOW_WIDTH)) {
			//	txData[bufferIndex]->buffer = (unsigned char *)realloc(txData[bufferIndex]->buffer, ((4 * buf_win_width1 * buf_win_height1)+4));
			//	memset(txData[bufferIndex]->buffer, 0, ((4 * buf_win_width1 * buf_win_height1)+4));
			//	
			//}
			
			arg.outBuffer = (void *)(txData[bufferIndex]->buffer);
			arg.outLen = 4 * buf_win_width1 * buf_win_height1;
			arg.window_width = buf_win_width1;
			arg.window_height = buf_win_height1;
			
			if((buf_win_height1 != WINDOW_HEIGHT) || (buf_win_width1 != WINDOW_WIDTH)) {
				bReshaped=1;
			}
#endif
			JpegCompressPixelData((void *)&arg);
			DEBUG_LOG(MODULE_OPENGL, DBG, "jpegLength = ");
			DEBUG_LOG(MODULE_OPENGL, DBG, arg.jpegLength);
			txData[bufferIndex]->length = (unsigned int)(arg.jpegLength);
			txData[bufferIndex]->buffer[txData[bufferIndex]->length] = bReshaped;
			txData[bufferIndex]->length++;
			txData[bufferIndex]->flag |= BUFFER_FRESH;
			unsigned int tmpIndex = bufferIndex;
			bufferIndex = (bufferIndex+1)%MAX_QUEUE_SIZE;
			txData[tmpIndex]->flag &= ~BUFFER_LOCKED;
			
			
			GLenum error = glGetError(); 
			if(error)
				std::cout << "glReadPixels falied: " << gluErrorString(error) << std::endl;
			free(data);
			//free(buf);
		}
		//pthread_mutex_unlock(&request_mutex);
		glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 ); // unbind our fbo
		//if(connected && clientReqRecd) {
		//	UDTTxThread(NULL);
		//	clientReqRecd=0;
		//}

	}

/*********************************************************************************************************
 **		
 ** Function: 	blitFB
 ** Returns:	void
 ** Parameters:	unsigned int currFBO 
 ** Notes: 	1. binds the FBO to a READ_FRAMEBUFFER_EXT
 ** 		2. unbinds from draw buffer, clears depth and color buffers
 ** 		3. Draws the cube.
 ** 		4. Swaps buffers if rendering in foreground.
 *********************************************************************************************************/
	void CCOpenGL::blitFB (unsigned int currFBO) 
	{
		glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, currFBO ); // bind our fbo
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0); 

		//glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour buffer (more buffers later on)

		glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

		glTranslatef(0.0f, 0.0f, -2.0f);

		GLenum error2 = glGetError(); 
		if(error2)
			std::cout << "Could not correctly bind buffers: " << gluErrorString(error2) << std::endl;

		glBegin(GL_QUADS);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner
		if(currFBO == fbo)
			glColor3f(0.0f,1.0f,0.0f); //green
		else if((currFBO == fboNext) && (!multicolor))
			glColor3f(0.0f,1.0f,0.0f); //green
		else if((currFBO == fboNext) && (multicolor))
			glColor3f(0.0f,0.0f,1.0f); //blue
		glEnd();

		GLenum error1 = glGetError(); 
		if(error1)
			std::cout << "1 " << gluErrorString(error1) << std::endl;

		if(!renderInBackground) {
			//glBlitFramebufferEXT( 0, 0, 255, 255, 0, 0, 255, 255,GL_COLOR_BUFFER_BIT, GL_NEAREST );
			
			glBlitFramebufferEXT( 0, 0, window_width, window_height, 0, 0, window_width, window_height,GL_COLOR_BUFFER_BIT, GL_NEAREST );
			glutSwapBuffers();
		}
		else {
			//glFlush();

		}
	}

/*********************************************************************************************************
 **		
 ** Function: 	display
 ** Returns:	void
 ** Parameters:	None
 ** Notes: 	1. Renders cube scene using 2 buffers
 *********************************************************************************************************/
	void CCOpenGL::display (void) 
	{
		renderCubeScene(fbo); // Render our cube scene into our frame buffer
		readFromFB(fbo);
		blitFB(fbo);	

		renderCubeScene(fboNext); // Render our cube scene into our frame buffer
		readFromFB(fboNext);
		blitFB(fboNext);
	}

/*********************************************************************************************************
 **		
 ** Function: 	reshape
 ** Returns:	void
 ** Parameters:	None
 ** Notes: 	Called when window is resized.
 *********************************************************************************************************/
	void CCOpenGL::reshape (int width, int height) 
	{
		//std::cout << "Inside reshape.." << width << " x " << height << std::endl;
		glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
		glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
		glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
		gluPerspective(45, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
		glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
		resized_win_flag=true;
		resized_window_width = width;
		resized_window_height=height;
		resized_win_flag=false;
		window_width = width;
		window_height=height;
	}

/*********************************************************************************************************
 **		
 ** Function: 	keyPressed
 ** Returns:	void
 ** Parameters:	unsigned char key, int x, int y
 ** Notes: 	called when a key is pressed. Press 'b' to swtich to background rendering.
 *********************************************************************************************************/
	void CCOpenGL::keyPressed (unsigned char key, int x, int y) 
	{
		keyStates[key] = true; // Set the state of the current key to pressed
		if(key == 'b') renderInBackground=TRUE;
		if(key == 'f') renderInBackground=FALSE;
	}

	void CCOpenGL::keyUp (unsigned char key, int x, int y) 
	{
		keyStates[key] = false; // Set the state of the current key to not pressed
	}

	void CCOpenGL::close () 
	{
		leaveProgram(0);
	}
/*********************************************************************************************************
 **		
 ** Function: 	setup
 ** Returns:	void
 ** Parameters:	None
 ** Notes: 	1. Initialize GLUT, set up basic display buffer, init and create window
 ** 		2. initialize frame buffer objects, color and depth buffers
 ** 		3. Set the idle and reshape functions.
 ** 		4. Call glutMainLoop in rendering in foreground
 ** 		5. Call reshape fucntion continuously if rendering in background.
 *********************************************************************************************************/
	void CCOpenGL::setup () 
	{
		glutInit(&myargc, myargv); // Initialize GLUT
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); // Set up a basic display buffer (only single buffered for now)
		glutInitWindowSize (window_width, window_height); // Set the width and height of the window
		resized_window_width=window_width;
		resized_window_height=window_height;
		glutInitWindowPosition (100, 100); // Set the position of the window

		glutCreateWindow("Crack Creative Server");

#ifndef OSX
		if(renderInBackground) {
			glutHideWindow();
		}
		if (GLEW_OK != glewInit()) {
			//std::cout << "Couldn't initialize GLEW" << endl;
			exit(0);
		}
#endif
		prodIndex=0;
		init();

		glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering
		glutIdleFunc(display); // Tell GLUT to use the method "display" for rendering

		glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping

		glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
		glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
#if defined(WIN32)
		glutCloseFunc(close);
#endif

		if(!renderInBackground) {
			glutMainLoop(); // Enter GLUT's main loop
		}
		else {
			reshape(window_width, window_height);
			while(1) {
				display();
			}
		}
	}



}
