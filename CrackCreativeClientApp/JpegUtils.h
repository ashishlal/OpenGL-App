//
//  JpegUtils.h
//  CrackCreativeClientApp
//
//  Created by Ashish Lal on 24/11/10.
//  Copyright 2010 NetTech India. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#ifdef _cplusplus
extern "C" {

#endif
	
#include "jpeglib.h"
#include "jerror.h"
#define MY_GL_RGB                            0x1907
#define MY_GL_RGBA                           0x1908
	
	
boolean JpegDecompress(unsigned char *src, int srcLen, unsigned char *dest, int format, unsigned int *width, unsigned int *height);
#ifdef _cplusplus
};
#endif