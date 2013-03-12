#include "Interfaces.h"

#ifdef WIN32
#include <GL/glew.h> // Include the GLEW header file
#include <GL/freeglut/glut.h> // Include the GLUT header file
#include <GL/freeglut/freeglut_ext.h> // Include the GLUT header file
#include <pthread.h>
#endif

#ifdef LINUX
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/gl.h>

#include <GL/glut.h>
#include <GL/glext.h>
#endif

#ifdef OSX

#define _GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>

#include <GL/glut.h>
#include <GL/glext.h>
#endif

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

#ifdef NOT_USED
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
#endif

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
extern unsigned char *pixelData;

void keyOperations (void);
void renderCubeScene (unsigned int);
void Flip(void *b, unsigned int w, unsigned int h);
void readFromFB (unsigned int );
void blitFB (unsigned int ); 
void display (void);
void reshape (int width, int height) ;
void keyPressed (unsigned char key, int x, int y) ;
void keyUp (unsigned char key, int x, int y) ;
void close () ;
void setup ();
                
int myargc;
char *myargv[32];

//#pragma comment(lib, "glew32.lib")
bool resized_win_flag=false;
bool* keyStates = new bool[256]; // Create an array of boolean values of length 256 (0-255)
bool    renderInBackground=FALSE; //donot show window but run in background
bool    multicolor=FALSE;
unsigned int fbo; // The frame buffer object
unsigned int fbo_render[2]; // The texture and depth object to write our frame buffer object to

unsigned int fboNext; // The frame buffer object
unsigned int fboNext_render[2]; // The texture and depth object to write our frame buffer object to


#define MYCOLOR 0
#define MYDEPTH 1
 
float rotation_degree = 0.0f; 
float rotation_degree2 = 0.0f;

int TEX_WIDTH        = 256;
int TEX_HEIGHT       = 256;
GLfloat tex_param_s  = 1.0f;
GLfloat tex_param_t  = 1.0f;
GLfloat aspect_ratio = 1.0f;
GLuint cube_list;
GLuint texture;                 // we need one texture later22

GLdouble *tex_mat = 0;

/*********************************************************************************************************
**             
** Function:   StartOpenGLRenderer
** Returns:    void
** Parameters: void *threadArg (renderInBackground, multicolor)
** Notes: This function sets up the frame buffers, depth and texture buffers for spinning cube.
*********************************************************************************************************/
void StartOpenGLRenderer(void *threadArg)
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
** Function:   initFrameBufferDepthBuffer
** Returns:    void
** Parameters: void 
** Notes: This function initializes depth buffer
*********************************************************************************************************/
void initFrameBufferDepthBuffer(void) 
{

        int WIDTH=MAX_WINDOW_WIDTH;
        int HEIGHT=MAX_WINDOW_HEIGHT;
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_render[MYDEPTH]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

}

/*********************************************************************************************************
**             
** Function:   initFrameBufferTexture
** Returns:    void
** Parameters: void 
** Notes: This allocates storage for 2 render buffers (color and depth) and inits color buffer.
*********************************************************************************************************/
void initFrameBufferTexture(void) 
{
        int WIDTH=MAX_WINDOW_WIDTH;
        int HEIGHT=MAX_WINDOW_HEIGHT;
        glGenRenderbuffersEXT(2, fbo_render);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_render[MYCOLOR]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, WIDTH, HEIGHT);
}

/*********************************************************************************************************
**             
** Function:   initFrameBuffer
** Returns:    void
** Parameters: void 
** Notes: Allocates storage for a frame buffer, color and depth buffers
*********************************************************************************************************/
void initFrameBuffer(void) 
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
** Function:   initFrameBufferDepthBufferB
** Returns:    void
** Parameters: void 
** Notes: Initializes the depth buffer B
*********************************************************************************************************/
void initFrameBufferDepthBufferB(void) 
{
        int WIDTH=MAX_WINDOW_WIDTH;
        int HEIGHT=MAX_WINDOW_HEIGHT;
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fboNext_render[MYDEPTH]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

}

/*********************************************************************************************************
**             
** Function:   initFrameBufferTextureB
** Returns:    void
** Parameters: void 
** Notes: Initializes the texture buffer B
*********************************************************************************************************/
void initFrameBufferTextureB(void) 
{
        int WIDTH=MAX_WINDOW_WIDTH;
        int HEIGHT=MAX_WINDOW_HEIGHT;
        glGenRenderbuffersEXT(2, fboNext_render);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fboNext_render[MYCOLOR]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, WIDTH, HEIGHT);
}

/*********************************************************************************************************
**             
** Function:   initFrameBufferB
** Returns:    void
** Parameters: void 
** Notes: Allocates frame buffer B, initializes it.
*********************************************************************************************************/
void initFrameBufferB(void) 
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
** Function:   initA
** Returns:    void
** Parameters: void 
** Notes: Initializes Frame Texture and depth render buffers, then allocates and inits the frame buffer
*********************************************************************************************************/
void initA(void) 
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
** Function:   initB
** Returns:    void
** Parameters: void 
** Notes: Initializes Frame Texture and depth render buffers, then allocates and inits the frame buffer
*********************************************************************************************************/
void initB(void) 
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
** Function:   init
** Returns:    void
** Parameters: void 
** Notes: Initializes 2 frame buffers used
*********************************************************************************************************/
void init(void) 
{
        //glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture
        glEnable(GL_DEPTH_TEST); // Enable depth testing
        initA();
        initB();
}

/*********************************************************************************************************
**             
** Function:   keyOperations
** Returns:    void
** Parameters: void 
** Notes: 
*********************************************************************************************************/
void keyOperations (void) 
{
        if (keyStates['a']) { // If the a key has been pressed
                // Perform 'a' key operations
        }
}

/*********************************************************************************************************
**             
** Function:   renderCubeScene
** Returns:    void
** Parameters: unsigned int currFBO 
** Notes: Binds the given frame buffer object for rendering, clears depth and color buffer,
** renders a spinning cube
*********************************************************************************************************/
void renderCubeScene (unsigned int currFBO) 
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


/*********************************************************************************************************
**             
** Function:   Flip
** Returns:    buffer
** Parameters: void *b, unsigned int w, unsigned int h
** Notes: Flips a buffer b of width w and height h along y-axis, 8 bytes a time.
**
*********************************************************************************************************/

void Flip(void *b, unsigned int w, unsigned int h)
{
#if 0
    __asm {
         pushq   rbp
        movq    rsp, rbp
        //subq    32, rsp

	mov r8, rdi //rdi b
	mov r9, rsi //W
	mov r10,rdx //H
	mov r11, 0 // h <- 0
	mov r12, 0 // w<- 0
	outloop:
	//obuf:
		mov r13, r9 // W
		mov r13, 4 //W * 4
		imul r13, r11 // W * 4 *h
		add r13, r8 // obuf = b + (h * W * 4)
	//inbuf:
		mov r14, r9 // W
		imul r14, 4 // W* 4
		mov r15, r10
		dec r15
		sub r15, r11 // r15 = (H-1-h) 
		imul r15, r14 // r15 = (H-1-h) *(W * 4)
		add r15, r8 // inbuf = r15 = b + (H-1-h) * (W * 4)
	inloop:
		mov rdi, [r15]
		mov rsi, [r13]
		mov [r15], rsi // swap bytes
		mov [r13], rdi // swap bytes
		add r15, 8
		add r13, 8
		inc r12
                inc r12
		cmp r12, r9 // r12 = 0 to W-1
		jb  inloop
                mov r12, 0
		inc r11 // inc h
		cmp r11, r10 // r11 = 0 to H-1
		jb outloop
    }
#else
    __asm__("mov %r8, %1\n\t"
            "mov %r9, %2\n\t" //W
            "mov %r10,%3\n\t" //H
            "mov %r11, 0\n\t" // h <- 0
            "mov %r12, 0\n\t" // w<- 0
            "outloop:"
       // obuf:
	    "mov %r13, %r9\n" // W
	    "mov %r13, 4\n" //W * 4
	    "imul %r13, %r11\n" // W * 4 *h
	    "add %r13, %r8\n" // obuf = b + (h * W * 4)
	//inbuf:
	    "mov %r14, %r9\n" // W
	    "imul %r14, 4\n" // W* 4
	    "mov %r15, %r10\n"
	    "dec %r15\n"
	    "sub %r15, %r11\n" // r15 = (H-1-h) 
	    "imul %r15, %r14\n" // r15 = (H-1-h) *(W * 4)
	    "add %r15, %r8\n" // inbuf = r15 = b + (H-1-h) * (W * 4)
	   "inloop:"
	    "mov %rdi, (%r15)\n\t"
	    "mov %rsi, (%r13)\n\t"
	    "mov (%r15), %rsi\n\t" // swap bytes
	    "mov (%r13), %rdi\n\t" // swap bytes
	    "add %r15, 8\n\t"
	    "add %r13, 8\n\t"
	    "inc %r12\n\t"
            "inc %r12\n\t"
	    "cmp %r12, %r9\n\t" // r12 = 0 to W-1
	    "jb  inloop;\n\t"
            "mov %r12, 0n\t"
	    "inc %r11\n\t" // inc h
	    "cmp %r11, %r10\n\t" // r11 = 0 to H-1
	    "jb outloop;\n\t"
            : "=r" (b)			/* output operands */
	    : "r" (b), "r" (w), "r" (h) /* input operands */
	    : "0");				/* clobbered operands */
 
#endif
}

#ifdef TEST
int modifyFramebufferContents(unsigned char *fb, int width, int height)
{
    int i, sum;
    glReadPixels(1, 1, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fb);
    for (i = 0, sum = 0; i < 4 * width * height; i++)
        sum += fb[i];

    printf("sum over fb via C: %d\n", sum);

sum =0;
__asm {
    mov r8, fb //fb
    mov r12, 0
    mov r13, 0
    mov ecx, 0
    mov r12, width
    imul r12, height
    imul r12, 4
    label:
        add ecx, [r8]
        inc r13
        cmp r13, r12
        jb label
    mov sum, ecx
}
    


    printf("sum over fb via inline asm: %d\n", sum);

    return (sum);
}
#endif

/*********************************************************************************************************
**             
** Function:   readFromFB
** Returns:    void
** Parameters: unsigned int currFBO 
** Notes:      1. binds the FBO to a READ_FRAMEBUFFER_EXT
**             2. does nothing if no UDT connection
**             3. Looks for a free buffer, locks it for writing
**             4. Reads the raw pixel data
**             5. Sets up JPEG compression arguments.
**             6. Calls function to compress JPEG pixel data
**             7. Unbinds the FBO
*********************************************************************************************************/
void readFromFB (unsigned int currFBO) 
{
        unsigned char *data=NULL;
        static unsigned int bufferIndex=0;
        bool unlockedBufferFound=false;
        unsigned int unlockedBufferIndex=0;
        if(!connected) return;
        if(resized_win_flag) return;
        if(!clientReqRecd) return;
#if 0
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
        if(clientReqRecd)
        //if(1) 
        {
#ifdef ORIGINAL_CODE                    
                data =(unsigned char *)malloc(4 * window_height * window_width);
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

#if 0
                
                data = (unsigned char *)malloc(4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT);
                
                memset(data, 0, (4 * MAX_WINDOW_WIDTH * MAX_WINDOW_HEIGHT));
#endif
                data = pixelData;
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                JpegThreadArg arg;
                unsigned char bReshaped=0;
                
                arg.window_width = buf_win_width1;
                arg.window_height = buf_win_height1;
                
                if((buf_win_height1 != WINDOW_HEIGHT) || (buf_win_width1 != WINDOW_WIDTH)) {
                    
#ifndef EXPERIMENTAL                  
                    glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    //glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, txData[bufferIndex]->buffer);
#else
                    glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, txData[bufferIndex]->buffer);
                    //we want closest to WINDOW_WIDTHxWINDOW_HEIGHT
                    unsigned int tot_cols = WINDOW_WIDTH;
                    unsigned int tot_rows = WINDOW_HEIGHT;
                    
                    if(buf_win_height1 > buf_win_width1) {
                        tot_cols = (unsigned int)(((float)tot_cols + (float)((float)buf_win_height1/(float)buf_win_width1)));
                    }
                    else if(buf_win_width1 > buf_win_height1) {
                        tot_rows = (unsigned int)(((float)tot_rows + (float)((float)buf_win_width1/(float)buf_win_height1)));
                    }
                    
                    gluScaleImage(GL_RGBA, buf_win_width1, buf_win_height1,GL_UNSIGNED_BYTE, txData[bufferIndex]->buffer, tot_cols,tot_rows, GL_UNSIGNED_BYTE, data);
                    arg.window_width = tot_cols;
                    arg.window_height = tot_rows;
#endif
                    bReshaped=1;
                    
                }
                else {
                    glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, data);
                   //glReadPixels(0, 0, buf_win_width1, buf_win_height1, GL_RGBA, GL_UNSIGNED_BYTE, txData[bufferIndex]->buffer);
                }
                
                GLenum error1 = glGetError(); 
                if(error1)
                        std::cout << "glReadPixels falied: " << gluErrorString(error1) << std::endl;
                        
                //memcpy(data, txData[bufferIndex]->buffer, 4 * buf_win_width1 * buf_win_height1);
                //Flip(data, buf_win_width1, buf_win_height1);
                //modifyFramebufferContents(data, buf_win_width1, buf_win_height1);
                
                txData[bufferIndex]->flag |= BUFFER_LOCKED;
                arg.inBuffer = (void *)data;
                
                arg.outBuffer = (void *)(txData[bufferIndex]->buffer);
                arg.outLen = 4 * buf_win_width1 * buf_win_height1;
                //arg.window_width = buf_win_width1;
                //arg.window_height = buf_win_height1;
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
                //free(data);
                
        }
        //pthread_mutex_unlock(&request_mutex);
        glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, 0 ); // unbind our fbo

}

/*********************************************************************************************************
**             
** Function:   blitFB
** Returns:    void
** Parameters: unsigned int currFBO 
** Notes:      1. binds the FBO to a READ_FRAMEBUFFER_EXT
**             2. unbinds from draw buffer, clears depth and color buffers
**             3. Draws the cube.
**             4. Swaps buffers if rendering in foreground.
*********************************************************************************************************/
void blitFB (unsigned int currFBO) 
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
** Function:   display
** Returns:    void
** Parameters: None
** Notes:      1. Renders cube scene using 2 buffers
*********************************************************************************************************/
void display (void) 
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
** Function:   reshape
** Returns:    void
** Parameters: None
** Notes:      Called when window is resized.
*********************************************************************************************************/
void reshape (int width, int height) 
{
        //std::cout << "Inside reshape.." << width << " x " << height << std::endl;
        glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
        glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
        glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
        
#if 1
        unsigned int tot_cols = width;
        unsigned int tot_rows = height;
        if(height > width) {
            tot_cols = (unsigned int)(((float)tot_cols + (float)((float)height/(float)width)));
        }
        else if(width > height) {
            tot_rows = (unsigned int)(((float)tot_rows + (float)((float)width/(float)height)));
        }
         gluPerspective(45, (GLfloat)tot_cols / (GLfloat)tot_rows, 1.0, 100.0);
#else
        gluPerspective(45, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
 #endif      
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
** Function:   keyPressed
** Returns:    void
** Parameters: unsigned char key, int x, int y
** Notes:      called when a key is pressed. Press 'b' to swtich to background rendering.
*********************************************************************************************************/
void keyPressed (unsigned char key, int x, int y) 
{
        keyStates[key] = true; // Set the state of the current key to pressed
        if(key == 'b') renderInBackground=TRUE;
        if(key == 'f') renderInBackground=FALSE;
}

void keyUp (unsigned char key, int x, int y) 
{
        keyStates[key] = false; // Set the state of the current key to not pressed
}

void close () 
{
        leaveProgram(0);
}
/*********************************************************************************************************
**             
** Function:   setup
** Returns:    void
** Parameters: None
** Notes:      1. Initialize GLUT, set up basic display buffer, init and create window
**             2. initialize frame buffer objects, color and depth buffers
**             3. Set the idle and reshape functions.
**             4. Call glutMainLoop in rendering in foreground
**             5. Call reshape fucntion continuously if rendering in background.
*********************************************************************************************************/
void setup () 
{
        glutInit(&myargc, myargv); // Initialize GLUT
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); // Set up a basic display buffer (only single buffered for now)
        glutInitWindowSize (window_width, window_height); // Set the width and height of the window
        resized_window_width=window_width;
        resized_window_height=window_height;
        glutInitWindowPosition (100, 100); // Set the position of the window

        glutCreateWindow("Crack Creative Server");

        if (window_width > 256)
            TEX_WIDTH = 512;
        else if (window_width > 512)
            TEX_WIDTH = 1024;
        else if (window_width > 1024)
            TEX_WIDTH = 2048;
        
        if (window_height > 256)
            TEX_HEIGHT = 512;
        else if (window_height > 512)
            TEX_HEIGHT = 1024;
        else if (window_height > 1024)
            TEX_HEIGHT = 2048;
        
        tex_param_s = static_cast<GLfloat>(window_width)  / static_cast<GLfloat>(TEX_WIDTH);
        tex_param_t = static_cast<GLfloat>(window_height) / static_cast<GLfloat>(TEX_HEIGHT);
        
        /* create texture transform matrix (avi file must not be 2^n by 2^m but a texture has to be) */
        tex_mat = new GLdouble[16];
        for (int i=0; i<16; ++i) tex_mat[i] = 0.0;
        tex_mat[0]  = tex_param_s;
        tex_mat[5]  = tex_param_t;
        tex_mat[10] = tex_mat[15] = 1.0;
    
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

