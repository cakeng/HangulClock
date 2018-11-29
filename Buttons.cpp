/*
*	Buttons.cpp
*
*	Created: 2018-06-26 오전 8:26:47
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/
#include "Buttons.h"


void Buttons::checkInputs()
{
	DDRB &= 0b10001111;
	PORTB &= 0b10001111;
	buttonPressed = NULL;
	if(!(PINB&(1<<PORTB6)))
	{
		buttonPressed = BUTTON_UP_PRESSED;
	}
	else if(!(PINB&(1<<PORTB5)))
	{
		buttonPressed = BUTTON_DOWN_PRESSED;
	}
	else if(!(PINB&(1<<PORTB4)))
	{
		buttonPressed = BUTTON_ALARM_PRESSED;
	}
}

Buttons::Buttons(uint16_t tickFreq, bool pinOn) //Ticks to reach 1 second.
{
	buttonTicks = 0;
	buttonCheckRateConsts[0] = (tickFreq/500*9); //18ms
	buttonCheckRateConsts[1] = (tickFreq/500*75); //150ms
	buttonCheckRateConsts[2] = (tickFreq/500*30); //60ms
	buttonCheckRate = buttonCheckRateConsts[2];
	buttonCounter = 0;
	buttonPressed = NULL;
	buttonLast = NULL;
	DDRB &= 0b10001111;
	PORTB &= 0b10001111;
}

Buttons::Buttons(uint16_t tickFreq) //Ticks to reach 1 second.
{
	buttonTicks = 0;
	buttonCheckRateConsts[0] = (tickFreq/500*9); //18ms
	buttonCheckRateConsts[1] = (tickFreq/500*75); //150ms
	buttonCheckRateConsts[2] = (tickFreq/500*30); //60ms
	buttonCheckRate = buttonCheckRateConsts[2];
	buttonCounter = 0;
	buttonPressed = NULL;
	buttonLast = NULL;
	DDRB &= 0b10001111;
	PORTB &= 0b10001111;
}

void Buttons::buttonFunction()
{
	buttonPressedOut = BUTTON_UNDER_TICKS;
	buttonCountNum = buttonCounter;
	if(buttonTicks < buttonCheckRate)
	{
		return;
	}
	buttonPressedOut = NULL;
	buttonTicks = 0;
	checkInputs();
	if(buttonPressed == NULL) // If no buttons were pressed
	{
		if(buttonLast != NULL) // If something was pressed before
		{
			buttonPressedOut = buttonLast;
			buttonLast = NULL;
			buttonCounter = 0;
			buttonCheckRate = buttonCheckRateConsts[2];
		}
		return;
	}
	buttonCounter++;
	if(buttonCounter > 24)
	{
		buttonCheckRate = buttonCheckRateConsts[0];
	}
	else if(buttonCounter > 4)
	{

		buttonCheckRate = buttonCheckRateConsts[1];
	}
	buttonPressedOut = buttonPressed;
	buttonLast = buttonPressed+3;
}
