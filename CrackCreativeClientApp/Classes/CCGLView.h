//
//  CCGLView.h
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 18/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@class CCUDTClientController;
@interface CCGLView : UIView
{
@private
	// The pixel dimensions of the backbuffer
	GLint backingWidth;
	GLint backingHeight;
	CCUDTClientController *controller;
	BOOL controllerSetup;
	BOOL draw;
	
	EAGLContext* m_context;
    
	GLuint m_frameBuffer;
    GLuint m_colorRenderbuffer;
    GLuint m_depthRenderbuffer;
	GLubyte *m_pixelData;
	BOOL animating;
	BOOL displayLinkSupported;
	NSInteger animationFrameInterval;
	// Use of the CADisplayLink class is the preferred method for controlling your animation timing.
	// CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
	// The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
	// isn't available.
	id displayLink;
    NSTimer *animationTimer;	
	
	//GLuint m_tex[1];
}

@property(nonatomic, retain) CCUDTClientController *controller;
@property(nonatomic, assign) BOOL draw;

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property(nonatomic, retain) EAGLContext* m_context;
@property(nonatomic, assign) GLuint m_frameBuffer;
@property(nonatomic, assign) GLuint m_colorRenderbuffer;
@property(nonatomic, assign) GLuint m_depthRenderbuffer;
@property(nonatomic, assign) GLubyte *m_pixelData;

- (void)setupView;
- (void) drawView;
- (void)gluPerspective:(double)fovy aspect:(double)aspect zNear:(double)zNear zFar:(double)zFar;
- (void)startAnimation;
- (void)stopAnimation;
- (void) setAnimationFrameInterval:(NSInteger)frameInterval;
- (CCUDTClientController *)controller;
- (void)setController:(CCUDTClientController *)d;
- (void)setData:(unsigned char *)buf;
@end