/*********************************************************************/
/*	Test functions for codes debugging and testing					
/*	Copyright 2013(c) Huayu Zhang, all rights reserved
/*	Dept of Engineering Physics, Tsinghua University
/*********************************************************************/

#pragma once


#include <Windows.h>
#include <stdio.h>

// The following part comes from Prof Deng Junhui, Dept of Computer Science, Tsinghua Univ
/******************************************************************************************
* Data Structures in C++
* ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
* Junhui DENG, deng@tsinghua.edu.cn
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2006-2013. All rights reserved.
******************************************************************************************/
#include <stdlib.h>
#include <time.h> //通常用当前时刻设置随机种子
//srand((unsigned int)time(NULL)); //设置随机种子
/******************************************************************************************
* 在[0, range)内随机生成一个数
******************************************************************************************/
static int dice(int range) { return rand() % range; } //取[0, range)中的随机整数
static int dice(int lo, int hi) { return lo + rand() % (hi - lo); } //取[lo, hi)中的随机整数
static float dice(float range) { return rand() % (1000 * (int)range) / (float)1000.; }
static double dice(double range) { return rand() % (1000 * (int)range) / (double)1000.; }
static char dice(char range) { return (char)(32 + rand() % 96); }

		
//get present time
void getPresentTime(SYSTEMTIME& sys){
	GetLocalTime(&sys);
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute,
		sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
}

//compute the time comsumption of a program
int getTimeConsumption(const SYSTEMTIME& start, const SYSTEMTIME& end){
	int tc = (end.wDay - start.wDay) * 24;
	tc += end.wHour - start.wHour;
	tc *= 60;
	tc += end.wMinute - start.wMinute;
	tc *= 60;
	tc += end.wSecond - start.wSecond;
	tc *= 1000;
	tc += end.wMilliseconds - start.wMilliseconds;
	printf("Time comsumption:%d(ms)\n", tc);
	return tc;
}