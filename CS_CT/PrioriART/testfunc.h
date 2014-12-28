/*********************************************************************/
/*	Test functions for codes debugging and testing					
/*	Copyright 2013(c) Huayu Zhang, all rights reserved
/*	Dept of Engineering Physics, Tsinghua University
/*********************************************************************/

#pragma once

#include <Windows.h>
#include <stdio.h>

void getPresentTime(SYSTEMTIME& sys);	//get present time
int getTimeConsumption(const SYSTEMTIME& start,const SYSTEMTIME& end);	//compute the time comsumption of a program
		
