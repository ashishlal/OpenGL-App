//
//  CCUDTClientController.h
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 12/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "objc_udt.h"
#import <Queue>
#import "CCGLView.h"

@protocol TapDetectingImageViewDelegate <NSObject>

@optional
- (void)tapDetectingImageView:(UIView *)view gotSingleTapAtPoint:(CGPoint)tapPoint;
- (void)tapDetectingImageView:(UIView *)view gotDoubleTapAtPoint:(CGPoint)tapPoint;
- (void)tapDetectingImageView:(UIView *)view gotTwoFingerTapAtPoint:(CGPoint)tapPoint;

@end

@interface CCClientController : UIViewController<UINavigationControllerDelegate, UITextFieldDelegate, 
	NSNetServiceBrowserDelegate, NSNetServiceDelegate, TapDetectingImageViewDelegate> {

	IBOutlet UIButton		*connectButton;
	IBOutlet UITextField	*serverIP;
	IBOutlet UITextField	*fps;
	IBOutlet UITextField	*serverPort;
	IBOutlet UITextField	*serviceNameField;
	
	NSNetServiceBrowser		*browser;
	NSMutableArray			*services;
	
	double fps_smoothed;
	int viewTop;
	int viewLeft;
	NSString *udtServerIP;
	NSString *udtServerPort;
	IBOutlet UIImageView *imageView;
	IBOutlet UIImageView *imageView2;
	UIView *overlayViewTop;
	UIView *overlayViewBottom;
	UIView *overlayViewLeft;
	UIView *overlayViewRight;
		
	UDTSOCKET fhandle;
	NSOperationQueue *operationQueue;
	CCGLView *glView;
	
}

-(IBAction) connectToServer:(id) sender;
-(void) ConnectAndUDTTransfer;
-(void) ImageFromGLData;
-(void) ImageFromJPEGData:(UIImage *)image;
-(void) RxThread;
-(void) updateUIWithMessage:(NSString*)message;
-(void) CalcFPS;
-(void) CalcFPSSmoothed;
-(void) SetImageViewDimensions:(BOOL)isMax;

@property(nonatomic, retain) IBOutlet UIButton		*connectButton;
@property(nonatomic, retain) IBOutlet UITextField	*serverIP;
@property(nonatomic, retain) IBOutlet UITextField	*fps;
@property(nonatomic, retain) IBOutlet UITextField	*serverPort;
@property(nonatomic, assign) double fps_smoothed;
@property(nonatomic, assign) int viewLeft;
@property(nonatomic, assign) int viewTop;

@property(nonatomic, retain) NSString *udtServerIP;
@property(nonatomic, retain) NSString *udtServerPort;
@property(nonatomic, retain) NSMutableArray			*services;
@property(nonatomic, retain) NSNetServiceBrowser	*browser;
@property(nonatomic, retain) UIImageView *imageView;
@property(nonatomic, retain) UIImageView *imageView2;
@property(nonatomic, retain) UIView *overlayViewTop;
@property(nonatomic, retain) UIView *overlayViewBottom;
@property(nonatomic, retain) UIView *overlayViewLeft;
@property(nonatomic, retain) UIView *overlayViewRight;
@property(nonatomic, assign) UDTSOCKET fhandle;
@property(nonatomic, retain) NSOperationQueue *operationQueue;
@property(nonatomic, retain) CCGLView		*glView;

@end
