//
//  CCUDTRecvOperation.m
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 31/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import "CCConcurrentOperation.h"


@implementation CCConcurrentOperation

- (id)init:(id)theTarget action:(SEL)theAction
{
    self = [super init];
    if (self) {
        target = theTarget;
        action = theAction;
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)main
{
    
    [target performSelectorOnMainThread:action withObject:nil waitUntilDone:NO];
    
}


@end
