/*
*	ClockWorks.cpp
*
*	Created: 2018-06-28 ¿ÀÀü 6:36:52
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/

#include "ClockWorks.h"

ClockWorks::ClockWorks(uint8_t h, uint8_t m, uint8_t s, uint16_t tickFreq)
{
	hours = h;
	mins = m;
	secs = s;
	timeTicks = 0;
	timeTicksConstant = tickFreq;
	clockKeeping();
}
void ClockWorks::clockKeeping()
{
	if(secs > 59)
	{
		secs -= 60;
		mins++;
	}
	else if (secs < 0)
	{
		secs += 60;
		mins--;
	}
	if(mins > 59)
	{
		mins -= 60;
		hours++;
	}
	else if(mins < 0)
	{
		mins += 60;
		hours--;
	}
	if(hours > 23)
	{
		hours -= 24;
	}
	else if(hours < 0)
	{
		hours += 24;
	}
}

void ClockWorks::setTime(uint8_t h, uint8_t m, uint8_t s)
{
	hours = h;
	mins = m;
	secs = s;
	clockKeeping();
}
void ClockWorks::getTime(uint8_t& h, uint8_t& m, uint8_t& s)
{
	h = hours;
	m = mins;
	s = secs;
}

void ClockWorks::autoRoutine()
{
	if(timeTicks > timeTicksConstant)
	{
		secs++;
		timeTicks -= timeTicksConstant;
		clockKeeping();
	}
}

