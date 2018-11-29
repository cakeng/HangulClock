/*
*	Music.cpp
*
*	Created: 2018-06-26 오전 6:36:52
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/

#include "Music.h"


void MusicObj::dataRead(uint16_t data) //36 on MIDI is O2.
{
	musicTime = (data>>9)&(0b01111111);
	noteBuffer = (o2notebox[(((data>>2)&(0b01111111)))%12]>>((int8_t)(((data>>2)&(0b01111111))/12)-3));
	channelVolume = data&0b00000011;

	TCNT1 = 0;
	ICR1 = noteBuffer;
	OCR1A = noteBuffer*3/4*channelVolume/8;

}


void MusicObj::musicOff()
{
	onFlag = false;
	musicPosition = 0;
	musicDown();
}

void MusicObj::musicDown()
{
	TCCR1A = 0;
	TCCR1B = 0;
	ICR1= 0;
	DDRB &= ~(1<<DDB3);
	PORTB &= ~(1<<PORTB3);
}

void MusicObj::musicOut()
{
	TCCR1A = controlAOnSettings;
	TCCR1B = controlBOnSettings;
	DDRB |= (1<<DDB3);
}

MusicObj::MusicObj(uint16_t tickFreq) //Ticks to reach 1 second.
{
	//Clear Output on Compare match, Fast PWM, Top Input Capture Register
	//PWM Frequency 250000/ICR1 Hz.
	controlAOnSettings = (1<<COM1A1)|(1<<WGM11);
	controlBOnSettings = (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	musicPosition = 0;
	musicTime = 0;
	noteBuffer = 0;
	musicTicks = 0;
	channelVolume = 0;

	o2notebox[0] = 3822;
	o2notebox[1] = 3607;
	o2notebox[2] = 3405;
	o2notebox[3] = 3214;
	o2notebox[4] = 3033;
	o2notebox[5] = 2863;
	o2notebox[6] = 2702;
	o2notebox[7] = 2551;
	o2notebox[8] = 2407;
	o2notebox[9] = 2272;
	o2notebox[10] = 2145;
	o2notebox[11] = 2024;
	
	
	musicTimeConst = (((uint32_t)tickFreq)*8/100); //80ms

	sheetLength = sizeof(sheet0)/2;

	TCCR1A = 0;
	TCCR1B = 0;
	ICR1 = 0;
	OCR1A = 0;
	onFlag = false;
	musicOff();
}

void MusicObj::musicFunction()
{
	if (onFlag)
	{
		if (musicTicks > musicTime*musicTimeConst)
		{
			musicTicks = 0;
			musicPosition++;
			if (musicPosition == sheetLength)
			{
				musicPosition = 0;
			}
			uint16_t buf = pgm_read_word(sheet0+musicPosition);

			dataRead(buf);
			if (channelVolume != 0)
			{
				musicOut();
			}
			else
			{
				musicDown();
			}
		}
	}
}
void MusicObj::autoRoutine()
{
	musicTicks++;
	if(musicTicks & 0x8000)
	{
		musicTicks = 0;
	}
}

