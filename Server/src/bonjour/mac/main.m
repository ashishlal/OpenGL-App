//
//  main.m
//  BonjourServer
//
//  Created by Ashish Lal on 09/09/10.
//  Copyright 2010 NetTech India. All rights reserved.
//

#import <Cocoa/Cocoa.h>
char servicePort[128];
int main(int argc, char *argv[])
{
	memset(servicePort, 0, sizeof(servicePort));
	strcpy(servicePort, "5555");
	if (argc > 1)
    {
      	for (int count = 1; count < argc; count++)
		{
			if((strcmp(argv[count], "-p") == 0) || (strcmp(argv[count], "--port") == 0)) {
				memset(servicePort, 0, sizeof(servicePort));
				strcpy(servicePort, argv[++count]);
			}
		}
	}
    return NSApplicationMain(argc,  (const char **) argv);
}
