//
//  CCGLView.m
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 18/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import "CCGLView.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "CCClientController.h"
#include <iostream>
#include <Queue>
#include "ConstantsAndMacros.h"

#define GL_RENDERBUFFER 0x8d41
#define FPS (1.0f/60.0f) // 60 frames per sec
@implementation CCGLView


@synthesize draw;
@synthesize m_context;
@synthesize m_frameBuffer;
@synthesize m_colorRenderbuffer;
@synthesize m_depthRenderbuffer;
//@synthesize m_tex;
@synthesize m_pixelData;
@synthesize animating;
@dynamic animationFrameInterval;

extern char *data[MAX_QUEUE_SZ+1];
extern std::queue<unsigned int> q;
extern unsigned int totFrames;
extern bool connected;
extern unsigned int WIDTH, HEIGHT;

GLuint m_tex[1];

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
		CAEAGLLayer*  eaglLayer = (CAEAGLLayer*) self.layer;
        //eaglLayer.opaque = YES;
		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, 
										kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
#if 1		
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
        m_context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!m_context) {
            api = kEAGLRenderingAPIOpenGLES2;
            m_context = [[EAGLContext alloc] initWithAPI:api];
        }
		if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }
		
				self.multipleTouchEnabled=NO;
     
		
        if (api == kEAGLRenderingAPIOpenGLES1) {
            NSLog(@"Using OpenGL ES 1.1");
           
        } else {
            NSLog(@"Using OpenGL ES 2.0");
            
        }
#else
		EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
        m_context = [[EAGLContext alloc] initWithAPI:api];
		if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }
		
		self.multipleTouchEnabled=YES;
       		
		//NSLog(@"Using OpenGL ES 1.1");
		
        
#endif
				
		//m_applicationEngine = ObjViewer::CreateApplicationEngine(m_renderingEngine, m_resourceManager);
		
		int width=WIDTH;
		int height=HEIGHT;
#if 0
		// Create a depth buffer that has the same size as the color buffer.
		glGenRenderbuffersOES(1, &m_depthRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthRenderbuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,width, height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
									 GL_RENDERBUFFER_OES, m_depthRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
		
		glGenTextures(1, &m_tex[0]);
		glBindTexture(GL_TEXTURE_2D, m_tex[0]); 
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_tex[0], 0);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_tex[0]);
		
		glGenFramebuffersOES(1, &m_frameBuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_frameBuffer);
		//glGenRenderbuffersOES(1, &m_colorRenderbuffer);
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		
		[m_context
		 //  renderbufferStorage:GL_RENDERBUFFER
		 renderbufferStorage:GL_RENDERBUFFER_OES
		 fromDrawable:(id<EAGLDrawable>)eaglLayer];
		
		// Extract width and height from the color buffer.
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
										GL_RENDERBUFFER_WIDTH_OES, &width);
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
										GL_RENDERBUFFER_HEIGHT_OES, &height);
		
		
		
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
									 GL_RENDERBUFFER_OES, m_tex[0]);
									// GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		
		//glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
		//						 GL_RENDERBUFFER_OES, m_depthStencil);
		//glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES,
		//						 GL_RENDERBUFFER_OES, m_depthStencil);
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_tex[0]);
		GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) ;
		if(status != GL_FRAMEBUFFER_COMPLETE_OES) {
			std::cout << "failed to make complete framebuffer object %x" <<  status << std::endl;
			//return NO;
			
			
		}
		
		
#else
		// Create a depth buffer that has the same size as the color buffer.
		glGenRenderbuffersOES(1, &m_depthRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthRenderbuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,width, height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
									 GL_RENDERBUFFER_OES, m_depthRenderbuffer);
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
		
		//glGenTextures(1, &m_tex[0]);
		//glBindTexture(GL_TEXTURE_2D, m_tex[0]); 
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		//glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_tex[0], 0);
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_tex[0]);
		
		glGenFramebuffersOES(1, &m_frameBuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_frameBuffer);
		glGenRenderbuffersOES(1, &m_colorRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		
		[m_context
		 //  renderbufferStorage:GL_RENDERBUFFER
		 renderbufferStorage:GL_RENDERBUFFER_OES
		 fromDrawable:(id<EAGLDrawable>)eaglLayer];
		
		// Extract width and height from the color buffer.
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
										GL_RENDERBUFFER_WIDTH_OES, &width);
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
										GL_RENDERBUFFER_HEIGHT_OES, &height);
		
		
		
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
									// GL_RENDERBUFFER_OES, m_tex[0]);
									GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		
		//glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
		//						 GL_RENDERBUFFER_OES, m_depthStencil);
		//glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES,
		//						 GL_RENDERBUFFER_OES, m_depthStencil);
		//glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		
		GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) ;
		if(status != GL_FRAMEBUFFER_COMPLETE_OES) {
			std::cout << "failed to make complete framebuffer object %x" <<  status << std::endl;
			//return NO;
			
			
		}
		
		
#endif		
		//Enable 2D textures.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		
		m_pixelData = (GLubyte *) malloc(4 * WIDTH * HEIGHT);
		memset(m_pixelData, 0, 4 * WIDTH * HEIGHT);
		
		
		
		//glBindTexture(GL_TEXTURE_2D, 0);
       // m_applicationEngine->Initialize(width, height);
		
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		
        //[self drawView: nil];
       // m_timestamp = CACurrentMediaTime();
		animating = FALSE;
		displayLinkSupported = FALSE;
		animationFrameInterval = 1;
		displayLink = nil;
		animationTimer = nil;
		
		[self setupView];
		draw=YES;
        [self drawView];
		NSString *reqSysVer = @"3.1";
		NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
		if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
			displayLinkSupported = TRUE;
		
        //displayLink = [CADisplayLink displayLinkWithTarget:self
		//										  selector:@selector(drawView:)];
        
        //[displayLink addToRunLoop:[NSRunLoop currentRunLoop]
		 //[displayLink addToRunLoop:[NSRunLoop mainRunLoop]
		//				  forMode:NSDefaultRunLoopMode];
	
    }
    return self;
}


// Sets up an array of values to use as the sprite vertices.
const GLfloat spriteVertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	0.5f,  0.5f,
};

// Sets up an array of values for the texture coordinates.
const GLshort spriteTexcoords[] = {
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};

const GLfloat quadVertices1[] = {
	-1.0f, -1.0f, 0.0f, //bottom left
	-1.0f, 1.0f, 0.0f,  // top left
	1.0f, -1.0f, 0.0f,  // bottom right
	1.0f, 1.0f, 0.0f,   // top right
	-1.0f, -1.0f, 0.0f  // top left again
};

#if 0
const GLfloat quadVertices[] = {
	0.0f, 0.0f,  
	1.0f, 0.0f,  
	0.0f, 1.0f,   
	1.0f, 1.0f,   
};
#else
const GLfloat quadVertices[] = {
	-1.0f, -1.0f,  //bottom left
	1.0f, -1.0f,  // top left
	-1.0f, 1.0f,   // bottom right
	1.0f, 1.0f,   // top right
};
#endif
// Sets up an array of values for the texture coordinates.
const GLfloat quadTexcoords1[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

const GLfloat quadTexcoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
	
};

- (void)setupView
{
	CGImageRef dummyImage;
	CGContextRef dummyContext;
	GLubyte *dummyData;
	
	// Sets up matrices and transforms for OpenGL ES
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	[self gluPerspective:60 aspect:(GLfloat)WIDTH / (GLfloat)HEIGHT zNear:1.0 zFar:100.0];
	
	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// Creates a Core Graphics image from an image file
	dummyImage = [UIImage imageNamed:@"Image.png"].CGImage;
	// Get the width and height of the image
	int width = CGImageGetWidth(dummyImage);
	int height = CGImageGetHeight(dummyImage);
	// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
	// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.
	
	if(dummyImage) {
		// Allocated memory needed for the bitmap context
		dummyData = (GLubyte *) calloc(width * height * 4, sizeof(GLubyte));
		// Uses the bitmap creation function provided by the Core Graphics framework. 
		dummyContext = CGBitmapContextCreate(dummyData, width, height, 8, width * 4, CGImageGetColorSpace(dummyImage), kCGImageAlphaPremultipliedLast);
		// After you create the context, you can draw the dummy image to the context.
		CGContextDrawImage(dummyContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), dummyImage);
		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(dummyContext);
		
		// Use OpenGL ES to generate a name for the texture.
		glGenTextures(1, &m_tex[0]);
		// Bind the texture name. 
		glBindTexture(GL_TEXTURE_2D, m_tex[0]);
		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Specify a 2D texture image, providing the a pointer to the image data in memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummyData);
		// Release the image data
		free(dummyData);
		
		// Enable use of the texture
		glEnable(GL_TEXTURE_2D);
		// Set a blending function to use
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		// Enable blending
		glEnable(GL_BLEND);
	}
	
}

- (void)layoutSubviews
{
	[EAGLContext setCurrentContext:m_context];
}


- (void)gluPerspective:(double)fovy aspect:(double)aspect zNear:(double)zNear zFar:(double)zFar
{
	// Start in projection mode.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

- (void) drawView
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
   	if(draw) 
	{
		static unsigned char *buffer=(unsigned char *)(data[0]);
		int i=0;
		int j=0;
		BOOL newData=NO;
		int n=0;
		//while((connected) && (q.size() > 0) && (n < 1)) {
		{
		//@synchronized(self)
		{
			i = q.size();
			if(i > 0) {
				newData = YES;
				j=q.front();
				//std::cout << "pop: " << i << " idx: " << j << std::endl;
				q.pop();
			}
		}
		if(newData == YES) {
			buffer = (unsigned char *)(data[j]);
			totFrames++;
			n++;
			
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		//glClear(GL_DEPTH_BUFFER_BIT);
		glLoadIdentity(); 
		
		glTranslatef(0.0f, 0.0f, -2.0f);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_frameBuffer);
		glVertexPointer(2, GL_FLOAT, 0, quadVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, quadTexcoords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, m_tex[0]);
		int width=WIDTH; int height=HEIGHT;
		GLubyte *pixels= (GLubyte *)buffer;
		
		int level=0;
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		[m_context presentRenderbuffer:GL_RENDERBUFFER_OES];
		}
	}
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
		
	glBindTexture(GL_TEXTURE_2D, 0);
}


- (void) drawView: (CADisplayLink*) displayLink
{
	[self drawView];
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.
			
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
			animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(FPS * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];
		
           // animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];
		
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }
		
        animating = FALSE;
    }
}

- (void) setAnimationFrameInterval:(NSInteger)frameInterval
{
	// Frame interval defines how many display frames must pass between each time the
	// display link fires. The display link will only fire 30 times a second when the
	// frame internal is two on a display that refreshes 60 times a second. The default
	// frame interval setting of one will fire 60 times a second when the display refreshes
	// at 60 times a second. A frame interval setting of less than one results in undefined
	// behavior.
	if (frameInterval >= 1)
	{
		animationFrameInterval = frameInterval;
		
		if (animating)
		{
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void)dealloc
{
	if([EAGLContext currentContext] == m_context) {
		[EAGLContext setCurrentContext:nil];
	}
	
	[m_context release];
	m_context = nil;
	
	
    [super dealloc];
}
-(CCUDTClientController *)controller
{
	return controller;
}

-(void)setController:(CCUDTClientController *)d
{
	controller = d;
}

- (void)setData:(unsigned char *)buf
{
}

@end
