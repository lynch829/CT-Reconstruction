/*********************************************************************/
/*	Test functions for codes debugging and testing					
/*	Copyright 2013(c) Huayu Zhang, all rights reserved
/*	Dept of Engineering Physics, Tsinghua University
/*	Platform: Microsoft Visual C++ 2010, Windows7 *OS
/*********************************************************************/

#include "testfunc.h"

void getPresentTime(SYSTEMTIME& sys){
	GetLocalTime( &sys );  
	printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ÐÇÆÚ%1d\n",
	sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, 
	sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek);   
}

int getTimeConsumption(const SYSTEMTIME& start,const SYSTEMTIME& end){
	int tc = (end.wDay - start.wDay)*24;
	tc += end.wHour - start.wHour;
	tc *= 60;
	tc += end.wMinute - start.wMinute;
	tc *= 60;
	tc += end.wSecond-start.wSecond;
	tc *= 1000;
	tc += end.wMilliseconds - start.wMilliseconds;
	printf("Time comsumption:%d(ms)\n",tc);
	return tc;
}