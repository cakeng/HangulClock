/*
*	ClockWorks.cpp
*
*	Created: 2018-06-28 ���� 6:36:52
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
<<<<<<< HEAD
*	use, modify, or share the software for any purpose
*	other than personal use.
=======
*	use, modify, or share the software for any purpose.
>>>>>>> be5ce2b47a916e376bcfe0e026002c3cdaf2fabe
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

