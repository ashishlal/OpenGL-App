//
//  BonjourServerController.m
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#import "BonjourServerMainWindowController.h"
#import "BonjourServerTableCell.h"

#define SERVICE_NAME	@"Crack Creative Messaging Service"
//#define SERVICE_NAME	@""
extern char servicePort[128];
@implementation BonjourServerMainWindowController

@synthesize services;
int init=0;
-(void) awakeFromNib {
	uint16_t chosenPort = 0;
	BonjourServerTableCell *cell = [[BonjourServerTableCell alloc] 
									 init:SERVICE_NAME andStatus:@"OFF"];
	self.services = [[NSMutableArray alloc] initWithObjects:cell, nil];
	
	serviceStarted=NO;
	
	if(!listeningSocket) {
        // Here, create the socket from traditional BSD socket calls,
		//and then set up an NSFileHandle with that to listen for incoming connections.
        int fdForListening;
        struct sockaddr_in serverAddress;
        socklen_t namelen = sizeof(serverAddress);
		
        // In order to use NSFileHandle's acceptConnectionInBackgroundAndNotify method, 
		//we need to create a file descriptor that is itself a socket, bind that socket, 
		//and then set it up for listening. 
		//At this point, it's ready to be handed off to acceptConnectionInBackgroundAndNotify.
        if((fdForListening = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
            memset(&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
           // serverAddress.sin_port = 5555; // allows the kernel to choose the port for us.
			serverAddress.sin_port = atoi(servicePort); 
            if(bind(fdForListening, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
                close(fdForListening);
                return;
            }
			
            // Find out what port number was chosen for us.
            if(getsockname(fdForListening, (struct sockaddr *)&serverAddress, &namelen) < 0) {
                close(fdForListening);
                return;
            }
			
            chosenPort = ntohs(serverAddress.sin_port);
            
            if(listen(fdForListening, 1) == 0) {
                listeningSocket = [[NSFileHandle alloc] initWithFileDescriptor:fdForListening closeOnDealloc:YES];
            }
        }
    }
    
    if(!netService) {
        // lazily instantiate the NSNetService object that will advertise on our behalf.
        netService = [[NSNetService alloc] initWithDomain:@"" type:@"_CrackCreativeMessagingService._udp" name:SERVICE_NAME port:chosenPort];
        [netService setDelegate:self];
    }
    
	appDataToClientField.delegate=self;
}

#pragma mark Action Handler
// Broadcast the service and add it to the services table
-(IBAction) broadcastAction:(id) sender {
	
    
       if(netService && listeningSocket) {
        if(!serviceStarted) {
            [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(connectionReceived:) name:NSFileHandleConnectionAcceptedNotification object:listeningSocket];
            [listeningSocket acceptConnectionInBackgroundAndNotify];
            [netService publish];
			serviceStarted = YES;
			BonjourServerTableCell *cell = [[BonjourServerTableCell alloc] 
											 init:SERVICE_NAME andStatus:@"Broadcasting"];
			[services replaceObjectAtIndex:0 withObject: cell];
			[servicesTable reloadData];

        } 
	}
	
}

// stop broadcasting and update the servicesTable
-(IBAction) stopAction:(id) sender {
	if(netService && listeningSocket)  {
		if(serviceStarted) {
			[netService stop];
			serviceStarted = NO;
			BonjourServerTableCell *cell = [[BonjourServerTableCell alloc] 
											init:SERVICE_NAME andStatus:@"OFF"];
			[services replaceObjectAtIndex:0 withObject: cell];
			[servicesTable reloadData];
		}
	}
}

//incoming connection and data
- (void)connectionReceived:(NSNotification *)aNotification {
    NSFileHandle *incomingConnection = [[aNotification userInfo] objectForKey:NSFileHandleNotificationFileHandleItem];
	
    [[aNotification object] acceptConnectionInBackgroundAndNotify];
	
    NSData *receivedData = [incomingConnection availableData];
	NSString *aStr = [[NSString alloc] initWithData:receivedData encoding:NSASCIIStringEncoding];
	[appDataFromClientField setStringValue:aStr];
    [incomingConnection closeFile];
}

-(IBAction) refreshAction:(id) sender
{
}

#pragma mark NSTextView delegates
- (void)controlTextDidEndEditing:(NSNotification *)aNotification
{
	if(!serviceStarted) return;
	appDataToClientField = [aNotification object];
}


-(IBAction) sendTextToClientAction:(id) sender
{
	if(!serviceStarted) return;
	NSData *appData = [[appDataToClientField stringValue] dataUsingEncoding:NSUTF8StringEncoding];
		if(netService) {
		NSOutputStream *outStream;
		[netService getInputStream:nil outputStream:&outStream];
		[outStream open];
		int bytes = [outStream write:[appData bytes] maxLength: [appData length]];
		[outStream close];
		
		NSLog(@"Wrote %d bytes", bytes);
	}
	
}

#pragma mark NSTableView delegates

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification {
	
	if([servicesTable selectedRow] >= 0)
		[broadcastButton setEnabled:YES];
	
	else
		[broadcastButton setEnabled:NO];
}

- (int)numberOfRowsInTableView:(NSTableView *)theTableView {
	if(!init) {
		init=1;
	}
    return [services count];
}


- (id)tableView:(NSTableView *)theTableView objectValueForTableColumn:(NSTableColumn *)theColumn row:(int)rowIndex {
	NSString *headerCellValue = [[theColumn headerCell] stringValue];
	if([headerCellValue isEqualToString: @"Service Name"])
		return [[services objectAtIndex:rowIndex] name];
	return (id)[[services objectAtIndex:rowIndex] status];
	
}

@end
