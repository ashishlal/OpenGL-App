//
//  BonjourServerTableCell.h
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface BonjourServerTableCell : NSObject {

	NSString *name;
	NSString *status;
}

@property(nonatomic,retain) NSString* name;
@property(nonatomic,retain) NSString* status;

- (id) init :(NSString*)newName andStatus:(NSString*)newStatus;
-(id) copyWithZone: (NSZone *) zone;
@end
