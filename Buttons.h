/*
*	Buttons.h
*
*	Created: 2018-06-26 오전 8:26:47
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/
#ifndef _BUTTONS_CAKENG_H
#define _BUTTONS_CAKENG_H
#include <avr/io.h>
#include "Basic.h"

#define BUTTON_UP_PRESSED			1
#define BUTTON_DOWN_PRESSED	    2
#define BUTTON_ALARM_PRESSED		3
#define BUTTON_UP_UNPRESSED	    4
#define BUTTON_DOWN_UNPRESSED	5
#define BUTTON_ALARM_UNPRESSED	6
#define BUTTON_UNDER_TICKS		7
#define NULL 0

#ifndef	BUTTON_INPUT_UP   
#define BUTTON_INPUT_UP                PINA5
#define BUTTON_INPUT_DOWN			PINA6
#define BUTTON_INPUT_ALARM			PINA7
#define BUTTON_INPUT_UP_PORT_GROUP  PINA
#define BUTTON_INPUT_UP_DATA_GROUP  DDRA
#define BUTTON_INPUT_DOWN_PORT_GROUP  PINA
#define BUTTON_INPUT_DOWN_DATA_GROUP  DDRA
#define BUTTON_INPUT_ALARM_PORT_GROUP  PINA
#define BUTTON_INPUT_ALARM_DATA_GROUP  DDRA
#endif



class Buttons
{
	private:
	volatile uint16_t buttonTicks;
	uint16_t buttonCheckRateConsts[3];
	uint16_t buttonCheckRate;
	uint16_t buttonCounter;
	uint8_t buttonPressed;
	uint8_t buttonLast;
	uint8_t buttonPressedOut;
	uint8_t buttonCountNum;
	
	void checkInputs();
	
	public:
	Buttons(uint16_t tickFreq);
	Buttons(uint16_t tickFreq, bool pinOn);
	
	void buttonTickCounter()
	{
		buttonTicks++;
	}
	
	void buttonFunction();
	
	uint8_t getButtonPressed()
	{
		return buttonPressedOut;
	}
	uint8_t getButtonCounter()
	{
		return buttonCountNum;
	}
	
};

#endif
