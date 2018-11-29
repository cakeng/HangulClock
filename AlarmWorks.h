/*
*	AlarmWorks.h
*
*	Created: 2018-06-28 ¿ÀÀü 8:56:47
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/

#ifndef _ALARMWORKS_CAKENG_H
#define _ALARMWORKS_CAKENG_H
#include <avr/io.h>
#include "Basic.h"
#include "ClockWorks.h"
#include "MCP7940N_RTC.h"

class AlarmWorks: public ClockWorks
{
	private:
	bool alarmTriggered;
	bool alarmSet;
	public:
	AlarmWorks(uint16_t tickFreq);
	
	bool checkAlarm(ClockWorks& clockWork);

	bool isTriggered()
	{
		return alarmTriggered;
	}
	void unTriggerAlarm()
	{
		alarmTriggered = false;
	}
	bool isSet()
	{
		return alarmSet;
	}
	void setAlarm()
	{
		alarmSet = true;
	}
	void unSetAlarm()
	{
		alarmSet = false;
	}
};

#endif
