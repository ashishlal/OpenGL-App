//
//  BonjourServerAppDelegate.h
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface BonjourServerAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
