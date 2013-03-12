//
//  BonjourServerController.h
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

@interface BonjourServerMainWindowController : NSWindowController<NSNetServiceDelegate, NSTextFieldDelegate> {
	
	IBOutlet NSButton		*broadcastButton;
	IBOutlet NSButton		*stopButton;
	IBOutlet NSTextField	*appDataFromClientField;
	IBOutlet NSTextField	*appDataToClientField;
	IBOutlet NSTableView	*servicesTable;
	
	NSNetService	*netService;
    NSFileHandle	*listeningSocket;
	NSMutableArray	*services;
	bool			serviceStarted;
}

-(IBAction) broadcastAction:(id) sender;
-(IBAction) stopAction:(id) sender;
-(IBAction) refreshAction:(id) sender;


@property(nonatomic,retain) NSMutableArray	*services;


@end
