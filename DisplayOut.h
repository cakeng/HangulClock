/*
*	DisplayOut.h
*
*	Created: 2018-07-10 오전 6:43:41
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/

#ifndef DISPLAYOUT_CAKENG_H
#define DISPLAYOUT_CAKENG_H
#include <avr/io.h>
#include "Basic.h"
#include "ClockWorks.h"
#include "MCP7940N_RTC.h"

#ifndef SHIFTOUT_PORT_GROUP
#define SHIFTOUT_PORT_GROUP			PORTA
#define SHIFTOUT_DATA_GROUP			DDRA
#define SHIFTOUT_SERIAL_CLOCK_PIN	PORTA4
#define SHIFTOUT_SERIAL_DATA_PIN	PORTA2
#define SHIFTOUT_SERIAL_LATCH_PIN	PORTA3
#define SHIFTOUT_ROW1	PORTA0
#define SHIFTOUT_ROW2	PORTA1
#endif

class DisplayOut
{
	private:
	uint16_t screenBuffer[2];
	uint8_t brightness;
	volatile  uint8_t rowNum;
	volatile  uint8_t shiftOutBuffer;
	bool min1Out;
	bool hourOut;
	
	public:
	bool flashHour;
	DisplayOut(uint8_t cons);

	void shiftOut(uint8_t data);
	void refreshDisplay();
	void flashDisplay();

	void setBrighness(uint8_t brt)
	{
		brightness = brt;
	}

	/*
	void setDisplay(uint8_t* arr);
	void setDisplay(uint16_t data1, uint16_t data2);
	*/
	void setDisplay(ClockWorks& clockObj);
	void setDisplayBin(ClockWorks& clockObj);
	void setDisplayHeart();
};

#endif