//
//  CoreCreativeClientAppAppDelegate.h
//  CoreCreativeClientApp
//
//  Created by Ashish Lal on 10/09/10.
//  Copyright NetTech India 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
@class CCClientController;
@interface CoreCreativeClientAppAppDelegate : NSObject <UIApplicationDelegate, UITabBarControllerDelegate> {
    UIWindow *window;
    
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end
