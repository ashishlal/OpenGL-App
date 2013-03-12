//
//  CCUDTRecvOperation.h
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 31/10/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CCConcurrentOperation : NSOperation {

    id target;
    SEL action;
}

- (id)init:(id)target action:(SEL)action;

@end
