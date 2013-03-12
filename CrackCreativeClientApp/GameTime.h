/*
 *  GameTime.h
 *  CrackCreativeClientApp
 *
 *  Created by Ashish Lal on 21/10/10.
 *  Copyright 2010 NetTech India. All rights reserved.
 *
 */

#ifndef __GAMETIME_H__
#define __GAMETIME_H__ 

#include <sys/time.h> 

typedef struct timeval TIMEVALUE;
#ifdef _cplusplus
extern "C" {
#endif
TIMEVALUE GetTimeNow(); 
TIMEVALUE AddTime( TIMEVALUE a, TIMEVALUE b ); 
TIMEVALUE SubtractTime( TIMEVALUE a, TIMEVALUE b );

void SetFramesPerSecond( const int Ticks ); 
int GetFrames( TIMEVALUE* tv ); 
void AddFrame( TIMEVALUE* tv ); 
void ResetTime();
#ifdef _cplusplus
};
#endif
#endif /* __UNIXTIME_H__ */