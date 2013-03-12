//
//  CoreCreativeClientAppAppDelegate.m
//  CoreCreativeClientApp
//
//  Created by Ashish Lal on 10/09/10.
//  Copyright NetTech India 2010. All rights reserved.
//

#import "CoreCreativeClientAppAppDelegate.h"


@implementation CoreCreativeClientAppAppDelegate

@synthesize window;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

#ifdef CUSTOM_IPHONE_TABBAR_IMAGE
    // Override point for customization after application launch.
	UIImage *buttonImage = [UIImage imageNamed:@"BroadcastService.png"];
	
	UIButton* button = [UIButton buttonWithType:UIButtonTypeCustom];
	
	NSLog(@"Button size: %f, %f", buttonImage.size.width, buttonImage.size.height);
	
	button.frame = CGRectMake(0.0, 0.0, buttonImage.size.width, buttonImage.size.height);
	[button setBackgroundImage:buttonImage forState:UIControlStateNormal];
	
	CGFloat heightDifference = buttonImage.size.height - self.tabBarController.tabBar.frame.size.height;
	
	NSLog(@"self.tabBarController.tabBar.frame.size.height: %f", self.tabBarController.tabBar.frame.size.height);
	NSLog(@"heightDifference: %f", heightDifference);
	NSLog(@"%Tabbar: %f, %f", tabBarController.tabBar.center.x, tabBarController.tabBar.center.y);
	if (heightDifference < 0)
		button.center = tabBarController.tabBar.center;
	else {
		CGPoint center = self.tabBarController.tabBar.center;
		center.y = center.y - heightDifference/2.0;
		button.center = tabBarController.tabBar.center;
	}
	
	NSLog(@"%Button: %f, %f", button.center.y, button.center.x);
	
	[tabBarController.view addSubview:button];
	tabBarController.hidesBottomBarWhenPushed=YES;
#endif	
    // Add the tab bar controller's view to the window and display.
	
    //[window addSubview:tabBarController.view];
    [window makeKeyAndVisible];

    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}



#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    //[tabBarController release];
    [window release];
    [super dealloc];
}

@end

