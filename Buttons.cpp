/*
*	Buttons.cpp
*
*	Created: 2018-06-26 오전 8:26:47
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
#include "Buttons.h"


void Buttons::checkInputs()
{
	buttonPressed = NULL;
	if(!BIT_CHECK(BUTTON_INPUT_UP_PORT_GROUP, BUTTON_INPUT_UP))
	{
		buttonPressed = BUTTON_UP_PRESSED;
	}
	else if(!BIT_CHECK(BUTTON_INPUT_DOWN_PORT_GROUP, BUTTON_INPUT_DOWN))
	{
		buttonPressed = BUTTON_DOWN_PRESSED;
	}
	else if(!BIT_CHECK(BUTTON_INPUT_ALARM_PORT_GROUP, BUTTON_INPUT_ALARM))
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
	BIT_OFF(BUTTON_INPUT_UP_DATA_GROUP, BUTTON_INPUT_UP);
	BIT_OFF(BUTTON_INPUT_DOWN_DATA_GROUP, BUTTON_INPUT_DOWN);
	BIT_OFF(BUTTON_INPUT_ALARM_DATA_GROUP, BUTTON_INPUT_ALARM);
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
<<<<<<< HEAD
	
=======
				
>>>>>>> be5ce2b47a916e376bcfe0e026002c3cdaf2fabe
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
