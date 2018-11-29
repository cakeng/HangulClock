/*
*	HangulClock4313.cpp
*
*	Created: 2018-10-20 오후 11:56:43
*	Author : Cakeng
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/
#define  F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Music.h"
#include "AlarmWorks.h"
#include "Buttons.h"
#include "DisplayOut.h"
#include "MCP7940N_RTC.h"


//ClockWorks clockObj(0, 0, 0, 5000);
MusicObj musicObj(5000);
AlarmWorks alarmObj(5000);
Buttons buttonObj(5000);
DisplayOut displayObj(2);
MCP7940n clockObj;

bool displayHeartMode;
bool alarmTimeSetMode;
bool alarmDisplayMode;

ISR(TIMER0_COMPA_vect)
{
	displayObj.refreshDisplay();
	buttonObj.buttonTickCounter();
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
		displayObj.flashDisplay();
		clockObj.loadTime();
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
	TCCR0A = 0b00000010; // CTC Mode
	TCCR0B = 0b00000011; // 1/64 Prescaler
	OCR0A = 49; // 5000Hz Interrupt
	
	TIMSK = 0b00000001;
	
	DDRD = 0b11111111;
	DDRB = 0b00011111;
	
	cli();
	clockObj.loadTime();
	sei();
	_delay_ms(100);
	while (1)
	{
		
		if(alarmObj.checkAlarm(clockObj))
		{
			musicObj.musicOn();
			displayHeartMode = true;
		}
		
		buttonObj.buttonFunction();
		_delay_ms(1);
		inputFunction(buttonObj.getButtonPressed(), buttonObj.getButtonCounter());
		if (alarmObj.isSet())
		{
			DDRB |= 0b00010000;
			PORTB |= 0b00010000; //Turn on LED1 - Pin shared by Button 1 (PB4)
			//displayObj.setBrighness(10);
		}
		else
		{
			DDRB &= 0b11101111;
			PORTB &= 0b11101111; //Turn off LED1 - Pin shared by Button 1 (PB4)
			//displayObj.setBrighness(0);
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
			if (ticks > 1200)
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
