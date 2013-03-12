//
//  CCUDTClientController.m
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 12/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import "CCClientController.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <Queue>
#import "UIImageExtensions.h"
#include "ConstantsAndMacros.h"
#include "GameTime.h"
#import "CCConcurrentOperation.h"
//#include "JpegUtils.h"

#define USE_JPEG_IMAGE
#define SERVICE_NAME	@"Crack Creative Messaging Service"
using namespace std;

@implementation CCClientController

@synthesize connectButton;
@synthesize serverIP;
@synthesize fps;
@synthesize viewTop;
@synthesize viewLeft;
@synthesize serverPort;
@synthesize udtServerIP;
@synthesize udtServerPort;
@synthesize imageView;
@synthesize imageView2;
@synthesize overlayViewTop;
@synthesize overlayViewBottom;
@synthesize overlayViewLeft;
@synthesize overlayViewRight;
@synthesize fhandle;
@synthesize operationQueue;
//@synthesize q;
@synthesize glView;
@synthesize fps_smoothed;
@synthesize services;
@synthesize browser;


std::queue<unsigned int> q;
//@synthesize data=_data;
#pragma pack(4)
char *data[MAX_QUEUE_SZ+1];
TIMEVALUE startTime;
double maxFPS=0.0f;
unsigned int totFrames=0.0f;
bool connected=false;
unsigned int HEIGHT=0;
unsigned int WIDTH=0;
unsigned int MAX_W=0;
unsigned int MAX_H=0;

#define FULLSCREEN_MODE

// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		
		
    }
	//udtServerIP = @"";
	//udtServerPort = @"";
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	
    [super viewDidLoad];
	
	serverIP.delegate=self;
	serverPort.delegate=self;
	fhandle = nil;
	
	// Set up services array to insert the bonjour service. 
	// Easier to have any array as it nicely fits with the tableview in any gui enhacement for mulitple services.
	services = [[NSMutableArray array] retain];
	
	// Set up the browser and search for the Bonjour Service.
	browser = [[NSNetServiceBrowser alloc] init];
	[browser setDelegate:self];
	[browser searchForServicesOfType:@"_CrackCreativeMessagingService._udp." inDomain:@""];
	[serviceNameField setText:@"Searching..."];
#if 1
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        
        HEIGHT = IPAD_IMAGEVIEW_HEIGHT;
        WIDTH = IPAD_IMAGEVIEW_WIDTH;
        MAX_W = MAX_IPAD_WIDTH;
        MAX_H = MAX_IPAD_HEIGHT;
                
	}
	else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        HEIGHT = IPHONE_IMAGEVIEW_HEIGHT;
        WIDTH = IPHONE_IMAGEVIEW_WIDTH;
        MAX_W = MAX_IPHONE_WIDTH;
        MAX_H = MAX_IPHONE_HEIGHT;
    }
#else
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
         if((orientation == UIDeviceOrientationPortrait) || (orientation == UIDeviceOrientationPortraitUpsideDown)) {
        
            HEIGHT = IPAD_IMAGEVIEW_HEIGHT;
            WIDTH = IPAD_IMAGEVIEW_WIDTH;
            MAX_W = MAX_IPAD_WIDTH;
            MAX_H = MAX_IPAD_HEIGHT;
        }
        else if((orientation == UIDeviceOrientationLandscapeLeft) || (orientation == UIDeviceOrientationLandscapeRight)) {
        
            HEIGHT = IPAD_IMAGEVIEW_WIDTH;
            WIDTH = IPAD_IMAGEVIEW_HEIGHT;
            MAX_W = MAX_IPAD_HEIGHT;
            MAX_H = MAX_IPAD_WIDTH;
        
        }
    
    }
    else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
        if((orientation == UIDeviceOrientationPortrait) || (orientation == UIDeviceOrientationPortraitUpsideDown)) {
        
            HEIGHT = IPHONE_IMAGEVIEW_HEIGHT;
            WIDTH = IPHONE_IMAGEVIEW_WIDTH;
            MAX_W = MAX_IPHONE_WIDTH;
            MAX_H = MAX_IPHONE_HEIGHT;
        }
        else if((orientation == UIDeviceOrientationLandscapeLeft) || (orientation == UIDeviceOrientationLandscapeRight)) {
        
            HEIGHT = IPHONE_IMAGEVIEW_WIDTH;
            WIDTH = IPHONE_IMAGEVIEW_HEIGHT;
            MAX_W = MAX_IPHONE_HEIGHT;
            MAX_H = MAX_IPHONE_WIDTH;
        
        }
    }
#endif
	
	overlayViewTop = nil;
#ifdef USE_JPEG_IMAGE
	UIImage *img;
	
	viewLeft=0;
	viewTop=140;
	
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		viewLeft = 10;
		img = [UIImage imageNamed:@"QuestionMark-iPad.jpg"];
	}
	else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
		viewLeft = 32;
		img = [UIImage imageNamed:@"QuestionMark.jpg"];
	}
#ifndef FULLSCREEN_MODE
	imageView = [[UIImageView alloc] initWithFrame:CGRectMake(viewLeft,viewTop,WIDTH,HEIGHT)];

	imageView.image = img;
	[self.view addSubview:imageView];
	
	overlayViewLeft = [[UIView alloc] initWithFrame:CGRectMake(0,viewTop,viewLeft,HEIGHT)];
	overlayViewLeft.backgroundColor = [UIColor whiteColor];
	[self.view addSubview:overlayViewLeft];
	
	overlayViewRight = [[UIView alloc] initWithFrame:CGRectMake((viewLeft+WIDTH),viewTop,MAX_W-(viewLeft+WIDTH),HEIGHT)];
	overlayViewRight.backgroundColor = [UIColor whiteColor];
	[self.view addSubview:overlayViewRight];
	
	overlayViewBottom = [[UIView alloc] initWithFrame:CGRectMake(0,(viewTop+HEIGHT),MAX_W,MAX_H-(viewTop+HEIGHT))];
	overlayViewBottom.backgroundColor = [UIColor whiteColor];
	[self.view addSubview:overlayViewBottom];
#else
	imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0,viewTop,MAX_W,MAX_H)];
	
	CGSize sz;
	sz.width=(float)(imageView.frame.size.width);
	sz.height=(float)(imageView.frame.size.height);
	//imageView.image = [img imageByScalingProportionallyToSize:sz];;
	[self.view addSubview:imageView];
	
	overlayViewLeft = [[UIView alloc] initWithFrame:CGRectMake(0,viewTop,viewLeft,MAX_H)];
	overlayViewLeft.backgroundColor = [UIColor clearColor];
	[self.view addSubview:overlayViewLeft];
	
#endif
    int adjustedSZ = 4 *  WIDTH * HEIGHT;
	for(int i=0; i < MAX_QUEUE_SZ+1; i++) {
		//data[i] = new char[adjustedSZ];
		data[i] = (char *)malloc(adjustedSZ);
		memset(data[i], 0, adjustedSZ);
	}
	std::cout << "sz = " << adjustedSZ << endl;
	operationQueue = [[NSOperationQueue alloc] init];
	[operationQueue setMaxConcurrentOperationCount:2];
    
	// add gesture recognizers to the overlay view left
    UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)];
    UITapGestureRecognizer *doubleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDoubleTap:)];
    UITapGestureRecognizer *twoFingerTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTwoFingerTap:)];
    
    [doubleTap setNumberOfTapsRequired:2];
    [twoFingerTap setNumberOfTouchesRequired:2];
    
    [overlayViewLeft addGestureRecognizer:singleTap];
    [overlayViewLeft addGestureRecognizer:doubleTap];
    [overlayViewLeft addGestureRecognizer:twoFingerTap];
    
    [singleTap release];
    [doubleTap release];
    [twoFingerTap release];
	
#else
	glView = [[CCGLView alloc] initWithFrame:CGRectMake(32,140,WIDTH,HEIGHT)];
	//glView = [[CCGLView alloc] initWithFrame:CGRectMake(0,0,self.frame.size.width,self.frame.size.height)];
	[glView setController:self];
	[self.view addSubview:glView];
	
#endif
}

- (void)dealloc
{
	
	if(imageView) [imageView release];
	if(overlayViewLeft) [overlayViewLeft removeFromSuperview];
	if(overlayViewRight) [overlayViewRight removeFromSuperview];
	if(overlayViewTop) [overlayViewTop removeFromSuperview];
	if(overlayViewBottom) [overlayViewBottom removeFromSuperview];
	
	if(glView) [glView removeFromSuperview];
	for(int i=0; i < MAX_QUEUE_SZ; i++) {
		if(data[i]) 
			delete data[i];
	}
	[super dealloc];
}

- (BOOL)shouldAutorotateToInterfaceOrientation: (UIInterfaceOrientation)interfaceOrientation
{
    if(fhandle == 0) return YES;
#ifndef FULLSCREEN_MODE
	if(overlayViewTop) {
		[self SetImageViewDimensions:true];
	}
	else {
		[self SetImageViewDimensions:false];
	}
#else
	if(imageView.frame.size.height == MAX_H) {
		[self SetImageViewDimensions:true];
	}
	else {
		[self SetImageViewDimensions:false];
	}
#endif

    return YES;
}


#pragma mark NSNetServiceBrowser delegates
// Called when the netService browser finds the service we are looking for
// Add the service to the services array and update the serviceNameField
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didFindService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
    
	NSLog( @"Service Discovered %@", [aNetService name] );
	if([[aNetService name] isEqualToString:SERVICE_NAME]==TRUE) 
	{
		[services addObject:aNetService];
		[serviceNameField setText:[[services objectAtIndex:0] name]];
		[aNetService setDelegate:self];
		[aNetService resolveWithTimeout:0];
	}
	
}

// Called when the netService browser finds the service is no longer active
// Remove the service to the services array and udate the serviceNameField
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
    [services removeObject:aNetService];
	[serviceNameField setText:@"Searching..."];
	
}

- (void)netServiceWillResolve:(NSNetService *)sender
{
    //NSLog( @"Attempting to resolve address for %@", [sender name] );
	return;
}

- (void)netService:(NSNetService *)sender didNotResolve:(NSDictionary *)errorDict
{
    // NSLog(@"Could not resolve: %@", errorDict);
	return;
}

- (void)netServiceDidResolveAddress:(NSNetService *)sender
{	
	NSLog( @"Resolving address for %@", [sender name] );
	NSString  *name = nil;
    NSData *address = nil;
    struct sockaddr_in *socketAddress = nil;
    int port=0;
	static bool addrResolved = false;
    
    //for(int i=0;i < [[sender addresses] count]; i++ )
	for(int i=0;(i < 1) && ([[sender addresses] count]) && (!addrResolved); i++ )
    {
        name = [sender name];
        address = [[sender addresses] objectAtIndex: i];
        socketAddress = (struct sockaddr_in *) [address bytes];
        udtServerIP = [NSString stringWithFormat: @"%s", inet_ntoa(socketAddress->sin_addr)];
        port = socketAddress->sin_port;
		udtServerPort = [NSString stringWithFormat:@"%d", port];
		NSLog( @"Successfully resolved address for %@-->%@:%@\n", [sender name], udtServerIP, udtServerPort );
		
		serverIP.text = udtServerIP;
		serverPort.text = udtServerPort;
		if (([serverIP.text length] <= 0) || ([serverPort.text length] <= 0)) {
			NSLog( @"Invalid server address/port\n");
			return;
		}
		addrResolved = true;
		if(fhandle != 0) return;
		
		NSInvocationOperation *udtOperation = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(ConnectAndUDTTransfer) object:nil];
		[operationQueue addOperation:udtOperation];
		[udtOperation release];
		
		startTime = GetTimeNow();
	}
}

#pragma mark -
#pragma mark Text Editing

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField {
	
	if ((textField == serverIP) && ([serverIP.text length] > 0)) {
		udtServerIP = serverIP.text;
	}
	else if ((textField == serverPort) && ([serverPort.text length] > 0)) {
		udtServerPort = serverPort.text;
	}
	
	return YES;
}

// return when 'Return' is pressed
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return NO;
}



#pragma mark TapDetectingImageViewDelegate methods

- (void)handleSingleTap:(UIGestureRecognizer *)gestureRecognizer {
	if(fhandle == 0) return;
#ifndef FULLSCREEN_MODE
	if(overlayViewTop) {
		[self SetImageViewDimensions:false];
	}
	else {
		[self SetImageViewDimensions:true];
	}
#else
	if(imageView.frame.size.height == MAX_H) {
		[self SetImageViewDimensions:false];
	}
	else {
		[self SetImageViewDimensions:true];
	}
#endif
}

- (void)handleDoubleTap:(UIGestureRecognizer *)gestureRecognizer {
    // double tap does nothing for now
}

- (void)handleTwoFingerTap:(UIGestureRecognizer *)gestureRecognizer {
	// two fingertap does nothing for now
}


#pragma mark -
#pragma mark Utilities

-(void) SetImageViewDimensions:(BOOL)isMax
{

	int statusBarHt=20;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
		
		
        
        if((orientation == UIDeviceOrientationPortrait) || (orientation == UIDeviceOrientationPortraitUpsideDown)) {
            
            HEIGHT = IPAD_IMAGEVIEW_HEIGHT;
            WIDTH = IPAD_IMAGEVIEW_WIDTH;
            MAX_W = MAX_IPAD_WIDTH;
            MAX_H = MAX_IPAD_HEIGHT;
        }
        else if((orientation == UIDeviceOrientationLandscapeLeft) || (orientation == UIDeviceOrientationLandscapeRight)) {
            
            HEIGHT = IPAD_IMAGEVIEW_WIDTH;
            WIDTH = IPAD_IMAGEVIEW_HEIGHT;
            MAX_W = MAX_IPAD_HEIGHT;
            MAX_H = MAX_IPAD_WIDTH;
            
        }

	}
	else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
        
        if((orientation == UIDeviceOrientationPortrait) || (orientation == UIDeviceOrientationPortraitUpsideDown)) {
            
            HEIGHT = IPHONE_IMAGEVIEW_HEIGHT;
            WIDTH = IPHONE_IMAGEVIEW_WIDTH;
            MAX_W = MAX_IPHONE_WIDTH;
            MAX_H = MAX_IPHONE_HEIGHT;
        }
        else if((orientation == UIDeviceOrientationLandscapeLeft) || (orientation == UIDeviceOrientationLandscapeRight)) {
            
            HEIGHT = IPHONE_IMAGEVIEW_WIDTH;
            WIDTH = IPHONE_IMAGEVIEW_HEIGHT;
            MAX_W = MAX_IPHONE_HEIGHT;
            MAX_H = MAX_IPHONE_WIDTH;
            
        }
	}
#ifndef FULLSCREEN_MODE
	if(isMax) {
		/* move image view to the center */
		viewTop=112+statusBarHt;
		imageView.backgroundColor = [UIColor blackColor];
		
		if(overlayViewTop) return;
		
		overlayViewTop = [[UIView alloc] initWithFrame:CGRectMake(0,0,MAX_W,viewTop)];
		overlayViewTop.backgroundColor = [UIColor blackColor];
		[self.view addSubview:overlayViewTop];
	}
	else {
		viewTop = 140;
		if(overlayViewTop) [overlayViewTop removeFromSuperview];
		overlayViewTop = nil;
	}
	imageView.frame = CGRectMake( viewLeft, viewTop, WIDTH, HEIGHT);
	overlayViewLeft.frame = CGRectMake(0,viewTop,viewLeft,HEIGHT);
	overlayViewRight.frame = CGRectMake((viewLeft+WIDTH),viewTop,MAX_W-(viewLeft+WIDTH),HEIGHT);
	overlayViewBottom.frame = CGRectMake(0, viewTop+HEIGHT, MAX_W, MAX_H-(HEIGHT+viewTop));
	overlayViewBottom.backgroundColor = [UIColor blackColor];
#else
	viewTop = 140;
	statusBarHt=0;
    imageView.backgroundColor = [UIColor blackColor];
	if(isMax) {
		imageView.frame = CGRectMake( 0, 0, MAX_W, MAX_H);
	}
	else {
		imageView.frame = CGRectMake( 0, viewTop, MAX_W, MAX_H-viewTop);
	}
    
#endif
}

- (void) CalcFPS
{
	TIMEVALUE now = GetTimeNow();
	static double minFPS=0.0f;
	
#ifdef OLD_CODE
	TIMEVALUE TimeDiffSinceLastCheck = SubtractTime(now , startTime );
	double diff_time;
	diff_time = TimeDiffSinceLastCheck.tv_sec;
	diff_time += (double) (TimeDiffSinceLastCheck.tv_usec * 1e-6);

	// every 1 second display the fps
	if(diff_time >= 1.0f)
	{
		NSString *myString;
		
		double d_fps = (double)(totFrames)/diff_time;
		if(d_fps > maxFPS) maxFPS = d_fps;
		
		totFrames=0;
		startTime = now;
		if(minFPS == 0.0) 
			minFPS=d_fps;
		else {
			if(d_fps < minFPS) minFPS = d_fps;
		}
		myString = [NSString stringWithFormat:@"%f (%f) (%f)",d_fps, minFPS, maxFPS];
		//std::cout << "c: " << d_fps << " m: " << maxFPS << std::endl;
		[self performSelectorOnMainThread:@selector(updateUIWithMessage:) withObject:myString waitUntilDone:NO];
		
	}
#else
	static TIMEVALUE last_frame_time;
	static double tot_time_so_far=0.0f;
	TIMEVALUE TimeDiffSinceLastFrame;
	
	if(1 == totFrames) {
		/* 1st frame */
		TimeDiffSinceLastFrame = SubtractTime(now , now );
	}
	else {
		TimeDiffSinceLastFrame = SubtractTime(now , last_frame_time );
	}
	tot_time_so_far += TimeDiffSinceLastFrame.tv_sec;
	tot_time_so_far += (double) (TimeDiffSinceLastFrame.tv_usec * 1e-6);
	last_frame_time = now;
	
	if(tot_time_so_far >= 1.0f)
	{
		NSString *myString;
		
		double d_fps = ((double)(totFrames))/tot_time_so_far;
		if(d_fps > maxFPS) maxFPS = d_fps;
		
		totFrames=0;
		startTime = now;
		tot_time_so_far = 0.0f;
		if(minFPS == 0.0) 
			minFPS=d_fps;
		else {
			if(d_fps < minFPS) minFPS = d_fps;
		}
		myString = [NSString stringWithFormat:@"%f (%f) (%f)",d_fps, minFPS, maxFPS];
		//std::cout << "c: " << d_fps << " m: " << maxFPS << std::endl;
		[self performSelectorOnMainThread:@selector(updateUIWithMessage:) withObject:myString waitUntilDone:NO];
		
	}
	
	
#endif
}

-(void) CalcFPSSmoothed
{

	if(totFrames != 0) {
		static unsigned int numFramesRecd=0;
		static double min_smoothed=0.0;
		static double max_smoothed=0.0;
		double smoothing_factor=3.5;
		//double smoothing_factor=1.0;
		NSString *myString;
		TIMEVALUE now = GetTimeNow();
		TIMEVALUE TimeDiffSinceLastCheck = SubtractTime(now , startTime );
		double diff_time;
		diff_time = TimeDiffSinceLastCheck.tv_sec;
		diff_time += (double) TimeDiffSinceLastCheck.tv_usec * 1e-6;
		numFramesRecd += totFrames;
		double d_fps = (double)(totFrames)/diff_time;
		if(d_fps > maxFPS) maxFPS = d_fps;
		// Explanation for smoothed frame rate
		// 1. The iPhone scehduler is based on AMU's Mach3 sceduler.
		// 2. I haven't used a queueing model for UDT traffic
		//Since we drop frames
		fps_smoothed += (d_fps * 0.9);
		
		if((numFramesRecd % 100) ==0) {
			double fps_average_smoothed = (fps_smoothed /numFramesRecd);
			if(min_smoothed == 0.0) 
				min_smoothed=fps_average_smoothed;
			else {
				if(fps_average_smoothed < min_smoothed) min_smoothed = fps_average_smoothed;
			}
			if(max_smoothed == 0.0) 
				max_smoothed=fps_average_smoothed;
			else {
				if(fps_average_smoothed > max_smoothed) max_smoothed = fps_average_smoothed;
			}
			myString = [NSString stringWithFormat:@"%f (%f) (%f)",fps_average_smoothed/smoothing_factor, 
						min_smoothed/smoothing_factor, max_smoothed/smoothing_factor];
			//std::cout << "c: " << fps_average_smoothed << " m: " << min_smoothed << " m1: " << max_smoothed << " n: " << numFramesRecd << " dt: " << diff_time << std::endl;
			[self performSelectorOnMainThread:@selector(updateUIWithMessage:) withObject:myString waitUntilDone:NO];
		}
		totFrames=0;
		startTime = now;
		
	}

}


#pragma mark -
#pragma mark Threads
- (void) ImageFromGLData
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[self retain];
	
	[glView drawView];
		
	[pool release];
	[self release];
	
	return;
	
	
}


- (void) ImageFromJPEGData:(UIImage *)image
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[self retain];
	{
		int i = q.size();
		if(i > 0) {
			q.pop();
		}
	}
	
	totFrames++;
#ifndef FULLSCREEN_MODE
	overlayViewLeft.backgroundColor = [UIColor blackColor];
	overlayViewRight.backgroundColor = [UIColor blackColor];
	overlayViewBottom.backgroundColor = [UIColor blackColor];
#endif
	imageView.image = image;
	[self CalcFPS];
	[pool release];
	[self release];
	
	return;
}


- (void)RxThread {
	
	if(fhandle == 0) return;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[self retain];
	
	int size = 4 * WIDTH * HEIGHT;
		
	int i=0;
	char *mydata =data[MAX_QUEUE_SZ];
	int rsize = 0;
	//while (q.size() < MAX_QUEUE_SZ)
	{
		//@synchronized(self)
		{
			i = q.size();
		}
		
		if(i <  MAX_QUEUE_SZ) {
			
			mydata = data[i];
		}
	
		char* sendNextFrameReq;
		int size1 = 8;
		sendNextFrameReq = new char[size1];
		memset((void *)sendNextFrameReq, 0, size1);
		*(sendNextFrameReq) = SIG1;
		*(sendNextFrameReq+1) = SIG2;
		*(sendNextFrameReq+2) = SIG3;
		*(sendNextFrameReq+3) = SIG1;
		
		int ssize = 0, ss=0;
		while (ssize < size1)
		{
			if (UDT::ERROR == (ss = [OBJC_UDT send:fhandle buffer:sendNextFrameReq+ssize  length:size1 - ssize flags:0]))
			{
				std::cout << "send:" << UDT::getlasterror().getErrorMessage() << std::endl;
				break;
			}
			ssize += ss;
		}
		
		delete [] sendNextFrameReq;
		
		//STREAM
#ifndef UDT_DGRAM
		int rs=0;
		size=4;
		unsigned char dataLength[4];
		while (true)
		{
			while (rsize < size)
			{
				if (UDT::ERROR == (rs = [OBJC_UDT recv:fhandle buffer:(char *)(dataLength + rsize)  length:size - rsize flags:0]))
				{
					std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
					break;
				}
				
				rsize += rs;
			}
			
			if (rsize == size)
				break;
		}
		unsigned int buflen = ((unsigned int)(dataLength[0]) | 
							   (unsigned int)(dataLength[1] << 8) | 
							   (unsigned int)(dataLength[2] << 16) | 
							   (unsigned int)(dataLength[3] << 24)
							   );
		size=buflen;
		rs=0;
		rsize=0;
		while (true)
		{
			while (rsize < size)
				{
					if (UDT::ERROR == (rs = [OBJC_UDT recv:fhandle buffer:mydata + rsize  length:size - rsize flags:0]))
					{
						std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
						break;
					}
					
					rsize += rs;
				}
				
				if (rsize == size)
					break;
		}
#else
		if (UDT::ERROR == (rsize = [OBJC_UDT recvmsg:fhandle buffer:mydata length:size]))
		{
			[self CalcFPS];			
			//free(mydata);
			[self performSelectorOnMainThread:@selector(RxThread) withObject:nil waitUntilDone:NO];
			
			[pool release];
			[self release];
			return;
			
		}
		
#endif
#ifndef USE_JPEG_IMAGE
		unsigned int width, height;
		if(JpegDecompress((unsigned char *)mydata, size, (unsigned char *)(data[i]), MY_GL_RGB, &width, &height))
		{
			{
				if(i <  MAX_QUEUE_SZ) {
					//std::cout << "Rx push in q: " << i << std::endl;
					//std::cout << "Rx push in q: " << i << " w: " << width << " h: " << height << std::endl;
					q.push(i);
				}
				
			}
			[glView drawView];
		}
		// we do everything on main thread
		[self CalcFPS];
#else
#ifndef FULLSCREEN_MODE
		unsigned char bReshaped = mydata[size-1];
		UIImage *theImage;
		UIImage *m_Image= [UIImage imageWithData:[NSData dataWithBytes:mydata length:size-1]];
		CGSize sz;
		sz.width=(float)WIDTH;
		sz.height=(float)HEIGHT;
		
		//bReshaped=0;
		if(bReshaped) {
			theImage = [m_Image imageByScalingProportionallyToSize:sz];
			//theImage = m_Image;
		}
		else {
			theImage = m_Image;
		}
		
#else
		unsigned char bReshaped = mydata[size-1];
		UIImage *theImage;
		UIImage *m_Image= [UIImage imageWithData:[NSData dataWithBytes:mydata length:size-1]];
		CGSize sz;
		sz.width=(float)(imageView.frame.size.width);
		sz.height=(float)(imageView.frame.size.height);
		// bReshaped not used set it to 0 to prevent warning
		bReshaped=0;
		theImage = [m_Image imageByScalingProportionallyToSize:sz];
					
#endif
		if(i <  MAX_QUEUE_SZ) {
			q.push(i);
		}
		[self performSelectorOnMainThread:@selector(ImageFromJPEGData:) withObject:theImage waitUntilDone:YES];
#endif		
	}
	
#ifdef USE_JPEG_IMAGE	
	[self performSelectorInBackground:@selector(RxThread) withObject:nil];
#else
	[self performSelectorOnMainThread:@selector(RxThread) withObject:nil waitUntilDone:NO];
#endif
	[pool release];
	[self release];
}

#pragma mark -
#pragma mark Server connect Action delegate

-(IBAction) connectToServer:(id) sender
{
	if (([serverIP.text length] <= 0) || ([serverPort.text length] <= 0)) {
		return;
	}
	
	//already connected */
	if(fhandle != 0) return;
	 
	NSInvocationOperation *udtOperation = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(ConnectAndUDTTransfer) object:nil];
    [operationQueue addOperation:udtOperation];
    [udtOperation release];
	
	startTime = GetTimeNow();
}

-(void) ConnectAndUDTTransfer 
{
	addrinfo hints_client;
	memset(&hints_client, 0, sizeof(struct addrinfo));
	
	
	hints_client.ai_flags = AI_PASSIVE;
	hints_client.ai_family = AF_INET;
	hints_client.ai_socktype = SOCK_STREAM;
	//hints_client.ai_socktype = SOCK_DGRAM;
    
    std::cout << "Inside ConnectAndUDTTransfer.. " << std::endl;
    
	addrinfo *local;
	if (0 != getaddrinfo((const char *)[serverIP.text UTF8String], (const char *)[serverPort.text UTF8String], &hints_client, &local))
	{
		std::cout << "incorrect network address.\n" << std::endl;
		return;
	}
	
	fhandle = [OBJC_UDT socket:local->ai_family type:local->ai_socktype protocol:0];
	//addrinfo hints;
	addrinfo *peer=local;
	
	std::cout << "connecting addr:" << inet_ntoa(((struct sockaddr_in *)(peer->ai_addr))->sin_addr) << " len: " << peer->ai_addrlen << std::endl;
	if (UDT::ERROR == [OBJC_UDT connect:fhandle address:(sockaddr*) peer->ai_addr length: peer->ai_addrlen])
	{
		std::cout << "connect: " << UDT::getlasterror().getErrorMessage() << std::endl;
		fhandle=0;
		return;
	}
	
	//bool block=false;
	//if (UDT::ERROR == [OBJC_UDT setsockopt:fhandle level:0 optname: UDT_RCVSYN optval:(const void*)&block length:sizeof(bool)])
	//{
	//	std::cout << "setsockopt: " << UDT::getlasterror().getErrorMessage() << std::endl;
	//	fhandle=0;
	//	return;
	//}
	
	std::cout << "Client connected to server " << std::endl;
	connected=true;
	freeaddrinfo(local);
//#ifndef FULLSCREEN_MODE
	// once connection is established, resize image view to max
	[self SetImageViewDimensions:true];
//#endif
	
#ifdef USE_JPEG_IMAGE	
	[self performSelectorInBackground:@selector(RxThread) withObject:nil];
#else
	[self performSelectorOnMainThread:@selector(RxThread) withObject:nil waitUntilDone:NO];
#endif	
}


-(IBAction) connectToServerTest:(id) sender
{
	if (([serverIP.text length] <= 0) || ([serverPort.text length] <= 0)) {
		return;
	}
	
	addrinfo hints_client;
	memset(&hints_client, 0, sizeof(struct addrinfo));
	
	
	hints_client.ai_flags = AI_PASSIVE;
	hints_client.ai_family = AF_INET;
	hints_client.ai_socktype = SOCK_STREAM;
	addrinfo *local;
	if (0 != getaddrinfo((const char *)[serverIP.text UTF8String], (const char *)[serverPort.text UTF8String], &hints_client, &local))
	{
		std::cout << "incorrect network address.\n" << std::endl;
		return;
	}
	
	fhandle = [OBJC_UDT socket:local->ai_family type:local->ai_socktype protocol:0];

	addrinfo *peer=local;
	
	std::cout << "connecting addr:" << inet_ntoa(((struct sockaddr_in *)(peer->ai_addr))->sin_addr) << " len: " << peer->ai_addrlen << std::endl;
	if (UDT::ERROR == [OBJC_UDT connect:fhandle address:(sockaddr*) peer->ai_addr length: peer->ai_addrlen])
	{
		std::cout << "connect: " << UDT::getlasterror().getErrorMessage() << std::endl;
		
		return;
	}
		
	std::cout << "Client connected to server " << std::endl;
	freeaddrinfo(local);

	char* data;
	int size = 100000;
	data = new char[size];
	int rs=0;
    int rsize = 0;
	while (true)
	{
		
		while (rsize < size)
		{
			if (UDT::ERROR == (rs = [OBJC_UDT recv:fhandle buffer:data + rsize  length:size - rsize flags:0]))
			{
				std::cout << "recv:" << UDT::getlasterror().getErrorMessage() << std::endl;
				break;
			}
			
			rsize += rs;
		}
		std::cout << "Client " << rsize << " bytes received so far" << std::endl;
		if (rsize == size)
			break;
	}
	
	char* byebye;
	int size1 = 1024;
	byebye = new char[size1];
	memset((void *)byebye, (int)(0xab), 1024);
	
	int ssize = 0, ss=0;
	while (ssize < size1)
	{
		if (UDT::ERROR == (ss = [OBJC_UDT send:fhandle buffer:byebye+ssize  length:size1 - ssize flags:0]))
		{
			
			std::cout << "send:" << UDT::getlasterror().getErrorMessage() << std::endl;
			
			break;
			
		}
		
		ssize += ss;
		
	}
	
	std::cout << "Sent bye bye to server" << ssize << " bytes " << std::endl;
	
	delete [] data;
	
	std::cout << "Closing Client Socket " << std::endl;
	
	[OBJC_UDT close:fhandle];
}

-(void)updateUIWithMessage:(NSString*)message
{
	fps.text = message;
}


@end
