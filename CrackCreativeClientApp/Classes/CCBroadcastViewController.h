//
//  FirstViewController.h
//  CoreCreativeClientApp
//
//  Created by Ashish Lal on 10/09/10.
//  Copyright NetTech India 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface BroadcastViewController : UIViewController<UITextFieldDelegate, NSNetServiceBrowserDelegate, NSNetServiceDelegate> {

	IBOutlet UIButton		*discoverButton;
	IBOutlet UIButton		*syncButton;
	IBOutlet UITextField	*serviceNameField;
	
	IBOutlet UITextField	*appDataFromServerField;
	IBOutlet UITextField	*appDataToServerField;
	
	NSNetServiceBrowser		*browser;
	NSMutableArray			*services;
	BOOL					canSend;
}

-(IBAction) discoverServiceAction:(id) sender;
-(IBAction) syncAction:(id) sender;

@property(nonatomic, retain) NSMutableArray			*services;
@property(nonatomic, retain) NSNetServiceBrowser	*browser;
@property(nonatomic, assign) BOOL	canSend;

@end
