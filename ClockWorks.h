/*
*	Music.h
*
*	Created: 2018-06-28 ¿ÀÀü 6:36:52
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/


#ifndef _CLOCKWORKS_CAKENG_H
#define _CLOCKWORKS_CAKENG_H
#include <avr/io.h>
#include "Basic.h"

class ClockWorks
{
	private:

	public:
	int8_t hours;
	int8_t mins;
	int8_t secs;
	volatile uint16_t timeTicks;
	uint16_t timeTicksConstant; // Ticks to reach 1 second.

	ClockWorks(uint8_t h, uint8_t m, uint8_t s, uint16_t tickFreq);
	
	void clockKeeping();
	
	void addMin()
	{
		mins++;
		clockKeeping();
	}
	void subMin()
	{
		mins--;
		clockKeeping();
	}
	uint8_t getSec()
	{
		return secs;
	}
	uint8_t getMin()
	{
		return mins;
	}
	uint8_t getHour()
	{
		return hours;
	}
	
	void setTime(uint8_t h, uint8_t m, uint8_t s);
	void getTime(uint8_t& h, uint8_t& m, uint8_t& s);
	
	void clockTickCounter()
	{
		timeTicks++;
		if(timeTicks >= timeTicksConstant)
		{
			secs++;
			timeTicks -= timeTicksConstant;
			clockKeeping();
		}
	}
};


#endif // _CLOCKWORKS_CAKENG_H
