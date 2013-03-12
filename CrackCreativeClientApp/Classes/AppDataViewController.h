//
//  AppDataViewController.h
//  CoreCreativeClientApp
//
//  Created by Ashish Lal on 10/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface AppDataViewController : UIViewController {

	IBOutlet UITextField *appDataFromServerField;
	IBOutlet UITextField *appDataToServerField;
}

-(IBAction) refreshAction:(id)sender;

@end
