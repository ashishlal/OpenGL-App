/*
 *  CCInterfaces.h
 *  CrackCreativeClientApp
 *
 *  Created by Ashish Lal on 13/10/10.
 *  Copyright 2010 NetTech India. All rights reserved.
 *
 */

class ICCUDTClientEngine {
public:
	virtual ~ICCUDTClientEngine(){};
	virtual void TestUDTConnectionToServer(char *serverIP, char *serverPort)=0;
};

namespace UDTClientEngine {ICCUDTClientEngine *CreateUDTClientEngine();};