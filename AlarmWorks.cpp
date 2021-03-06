/*
*	AlarmWorks.cpp
*
*	Created: 2018-06-28 ���� 8:56:47
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/

#include "AlarmWorks.h"

AlarmWorks::AlarmWorks(uint16_t tickFreq) : ClockWorks(0, 0, 0, tickFreq)
{
	
}
bool AlarmWorks::checkAlarm(ClockWorks& clockWork)
{
	if (hours == clockWork.getHour() && mins == clockWork.getMin() && secs == clockWork.getSec() && alarmSet)
	{
		alarmTriggered = true;
		return true;
	}
	return false;
}




