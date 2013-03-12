
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

namespace OpenGL {

	class CCOpenGL: public ICCOpenGL {
	public:
		CCOpenGL() {}
		~CCOpenGL() {}

		void StartOpenGLRenderer(void *arg);
		void ToggleRendererMode();
		void StartOpenGLImageReader();

	private:
		void initFrameBufferDepthBuffer(void);
		void initFrameBufferTexture(void);
		void initFrameBuffer(void);
		void initA(void);
		void initFrameBufferDepthBufferB(void);
		void initFrameBufferTextureB(void);
		void initFrameBufferB(void);
		void initB(void);
		void init(void);
		static void keyOperations (void);
		static void renderCubeScene (unsigned int);
                static void Flip8(unsigned char *b, unsigned int w, unsigned int h);
		static void readFromFB (unsigned int );
		static void blitFB (unsigned int ); 
		static void display (void);
		static void reshape (int width, int height) ;
		static void keyPressed (unsigned char key, int x, int y) ;
		static void keyUp (unsigned char key, int x, int y) ;
		static void close () ;
		void setup ();
		int myargc;
		char *myargv[32];


	};

}
