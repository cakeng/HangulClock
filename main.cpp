/*
*	HangulClock.cpp
*
*	Created: 2018-07-22 오후 3:15:21
*	Author : Cakeng
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
#define  F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Music.h"
#include "AlarmWorks.h"
#include "Buttons.h"
#include "DisplayOut.h"
#include "MCP7940N_RTC.h"

MCP7940n clockObj;
//ClockWorks clockObj(0, 0, 0, 5000);
MusicObj musicObj(62500, 5000);
AlarmWorks alarmObj(5000);
Buttons buttonObj(5000);
DisplayOut displayObj(2);

bool displayHeartMode;
bool alarmTimeSetMode;
bool alarmDisplayMode;
ISR(TIM1_COMPA_vect)
{
	displayObj.refreshDisplay();
	buttonObj.buttonTickCounter();
	//clockObj.clockTickCounter();
	musicObj.autoRoutine();
}


inline void inputFunction(uint8_t buttonReturn, uint16_t buttonCounts)
{
	if (buttonReturn == BUTTON_UNDER_TICKS)
	{
	}
	else if (buttonReturn == NULL)
	{
		static uint8_t loadCounter = 0;
		loadCounter++;
		if (loadCounter == 5)
		{
			displayObj.flashDisplay();
			clockObj.loadTime();
			loadCounter = 0;
		}
	}
	else
	{
		displayObj.flashDisplay();
		ClockWorks* tempObj;
		if (alarmTimeSetMode)
		{
			tempObj = &alarmObj;
		}
		else
		{
			tempObj = &clockObj;
		}
		
		if (buttonReturn == BUTTON_ALARM_UNPRESSED)
		{
			if (buttonCounts <12)
			{
				if (alarmObj.isTriggered())
				{
					displayHeartMode = false;
					alarmObj.unTriggerAlarm();
					alarmObj.unSetAlarm();
					musicObj.musicOff();
				}
				else if (alarmTimeSetMode)
				{
					alarmTimeSetMode = false;
					alarmObj.setAlarm();
				}
				else if (alarmObj.isSet())
				{
					alarmDisplayMode = true;
					alarmObj.unSetAlarm();
				}
				else
				{
					alarmDisplayMode = true;
					alarmObj.setAlarm();
				}
			}
			else
			{
				if (alarmTimeSetMode)
				{
					alarmTimeSetMode = false;
					alarmObj.setAlarm();
				}
				else
				{
					alarmTimeSetMode = true;
					alarmObj.unSetAlarm();
				}
			}
		}
		else if (buttonCounts < 6)
		{
			if (buttonReturn == BUTTON_UP_UNPRESSED)
			{
				tempObj->addMin();
				clockObj.saveTime();
			}
			else if(buttonReturn == BUTTON_DOWN_UNPRESSED)
			{
				tempObj->subMin();
				clockObj.saveTime();
			}
			
		}
		else
		{
			if (buttonReturn == BUTTON_UP_PRESSED)
			{
				tempObj->addMin();
			}
			else if(buttonReturn == BUTTON_DOWN_PRESSED)
			{
				tempObj->subMin();
			}
			else if (buttonReturn == BUTTON_UP_UNPRESSED)
			{
				clockObj.saveTime();
			}
			else if(buttonReturn == BUTTON_DOWN_UNPRESSED)
			{
				clockObj.saveTime();
			}
		}
	}
}

int main(void)
{
	alarmTimeSetMode = false;
	// Timer Setups
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001011;
	TIMSK1 = 0b00000010;
	OCR1A = 24; //5000Hz, 1/64 preScaler
	sei();
	while (1)
	{
		
		if(alarmObj.checkAlarm(clockObj))
		{
			musicObj.musicOn();
			displayHeartMode = true;
		}
		
		buttonObj.buttonFunction();
		inputFunction(buttonObj.getButtonPressed(), buttonObj.getButtonCounter());
		//clockObj.autoRoutine();
		if (alarmObj.isSet())
		{
			displayObj.setBrighness(10);
		}
		else
		{
			displayObj.setBrighness(0);
		}
		musicObj.musicFunction();
		
		if (alarmTimeSetMode)
		{
			cli();
			displayObj.flashHour = true;
			displayObj.setDisplay(alarmObj);
			sei();
		}
		else if (alarmDisplayMode)
		{
			static uint16_t ticks;
			ticks++;
			if (ticks > 700)
			{
				alarmDisplayMode = false;
				ticks = 0;
			}
			cli();
			displayObj.flashHour = true;
			displayObj.setDisplay(alarmObj);
			sei();
		}
		else if (displayHeartMode)
		{
			cli();
			displayObj.setDisplayHeart();
			sei();
		}
		else
		{
			cli();
			displayObj.flashHour = false;
			displayObj.setDisplay(clockObj);
			sei();
		}
		_delay_ms(1);
	}
}
