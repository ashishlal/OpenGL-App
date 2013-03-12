/*
 *  CCUDTClientEngine.cpp
 *  CrackCreativeClientApp
 *
 *  Created by Ashish Lal on 14/10/10.
 *  Copyright 2010 NetTech India. All rights reserved.
 *
 */


#include "CCInterfaces.h"
namespace UDTClientEngine {
	
class CCUDTClientEngine:public ICCUDTClientEngine 
{
	CCUDTClientEngine();
	CCUDTClientEngine();

	virtual void TestUDTConnectionToServer(char *serverIP, char *serverPort);
};


		
ICCUDTEngine *CreateUDTClientEngine()
{
	return new CCUDTClientEngine(); // return a new object
}
		
CCUDTClientEngine::CCUDTClientEngine() 
{
	UDT::startup(); // start the UDT library
			
}
CCUDTClientEngine::CCUDTClientEngine()
{
	UDT::cleanup(); // release the UDT library
			
}
		
}