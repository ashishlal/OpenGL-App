//
//  BonjourServerTableCell.m
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import "BonjourServerTableCell.h"


@implementation BonjourServerTableCell
@synthesize name, status;


- (id) init :(NSString*)newName andStatus:(NSString*)newStatus {
	if (self = [super init]) {
		name = newName;
		status = newStatus;
		return self;
	}
	return nil;
}


-(id) copyWithZone: (NSZone *) zone
{
	BonjourServerTableCell *cell = [[BonjourServerTableCell allocWithZone: zone] init];
	
	self->name = [name retain];
	self->status = [status retain];
	
	return cell;
}



@end
