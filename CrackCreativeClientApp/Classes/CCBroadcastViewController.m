//
//  BroadcastViewController.m
//  CoreCreativeClientApp
//
//  Created by Ashish Lal on 10/09/10.
//  Copyright NetTech India 2010. All rights reserved.
//

#import "CCBroadcastViewController.h"
#import <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

extern NSString *udtServerIP;
extern NSString *udtServerPort;

@implementation BroadcastViewController

@synthesize services;
@synthesize browser;
@synthesize canSend;

// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		
		
    }
    return self;
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	
    [super viewDidLoad];
	
	// Setup the NSTextField to receive event when editing ended
	appDataToServerField.delegate=self;
	
	// Set up services array to insert the bonjour service. 
	// Easier to have any array as it nicely fits with the tableview in any gui enhacement for mulitple services.
	services = [[NSMutableArray array] retain];
	
	// Set up the browser and search for the Bonjour Service.
	browser = [[NSNetServiceBrowser alloc] init];
	[browser setDelegate:self];
	[browser searchForServicesOfType:@"_CrackCreativeMessagingService._udp." inDomain:@""];
	[serviceNameField setText:@"Searching..."];
	
}

// To Discover the service when the "Discover Service" button is pressed
// 1. Remove all objects from the services array
// 2. Setup browser again
-(IBAction) discoverServiceAction:(id)sender {
	
	[services removeAllObjects];
	
	[browser stop];
	[browser searchForServicesOfType:@"_CrackCreativeMessagingService._udp." inDomain:@""];
}

// Sync with the Bonjour server by sending a data packet.
// This is only a basic solution -
// In the actual real world scenario the solution will be more complex.
// We will have to import iPhone app's data into the server and vice-versa
-(IBAction) syncAction:(id) sender
{
	// If no service return
	if([services count] == 0) return;
	
	// We only want the first service
	NSNetService *service = [services objectAtIndex: 0];
	
	// Dont send empty strings
	if(!canSend) return;
	
	// Convert NSTextField to NSData
	NSData *appData = [appDataToServerField.text dataUsingEncoding:NSUTF8StringEncoding];
	if(service) {
		NSOutputStream *outStream;
		[service getInputStream:nil outputStream:&outStream];
		[outStream open];
		int bytes = [outStream write:[appData bytes] maxLength: [appData length]];
		[outStream close];
		
		NSLog(@"Wrote %d bytes", bytes);
	}
	
}

#pragma mark -
#pragma mark Editing

// Text field editing ended.
// We want to know if the data can be sent if user presses the 'Send' Button
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField {
	
	if ((textField == appDataFromServerField) && ([appDataFromServerField.text length] > 0)) {
		
	}
	else if ((textField == appDataToServerField) && ([appDataToServerField.text length] > 0)) {
		canSend = TRUE;
	}
	
	return YES;
}

// return when 'Return' is pressed
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return NO;
}


#pragma mark NSNetServiceBrowser delegates
// Called when the netService browser finds the service we are looking for
// Add the service to the services array and update the serviceNameField
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didFindService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
    
	[services addObject:aNetService];
	[serviceNameField setText:[[services objectAtIndex:0] name]];
	[aNetService setDelegate:self];
	[aNetService resolveWithTimeout:0];
}

// Called when the netService browser finds the service is no longer active
// Remove the service to the services array and udate the serviceNameField
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing {
    [services removeObject:aNetService];
	[serviceNameField setText:@"Searching..."];
   
}

#pragma mark Other delegates
- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
