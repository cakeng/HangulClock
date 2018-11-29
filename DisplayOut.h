/*
*	DisplayOut.h
*
*	Created: 2018-07-10 오전 6:43:41
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/

#ifndef DISPLAYOUT_CAKENG_H
#define DISPLAYOUT_CAKENG_H
#include <avr/io.h>
#include "Basic.h"
#include "ClockWorks.h"
#include "MCP7940N_RTC.h"

class DisplayOut
{
	private:
	uint16_t screenBuffer[2];
	uint8_t brightness;
	volatile  uint8_t rowNum;
	volatile  uint8_t shiftOutBuffer;
	uint8_t minNum;
	bool hourOut;
	
	public:
	bool flashHour;
	DisplayOut(uint8_t cons);

	void shiftOut(uint8_t data);
	void refreshDisplay();
	void flashDisplay();

	void setBrighness(uint8_t brt)
	{
		if (brt < 5)
		{
			brt = 5;
		}
		brightness = brt;
	}

	
	void setDisplay(uint8_t* arr);
	void setDisplay(uint16_t data1, uint16_t data2);
	
	void setDisplay(ClockWorks& clockObj);
	void setDisplayBin(ClockWorks& clockObj);
	void setDisplayHeart();
};

#endif