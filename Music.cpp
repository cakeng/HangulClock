/*
*	Music.cpp
*
*	Created: 2018-06-26 오전 6:36:52
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

#include "Music.h"


void MusicObj::dataRead(uint16_t data)
{
	musicTime = (data>>9)&(0b01111111);
	noteBuffer = (o2notebox[(((data>>3)&(0b00111111)))%12]>>(((data>>3)&(0b00111111))/12+2));
	channelVolume = data&0b00000111;

	MUSIC_TIMER1_REG = 0;
	MUSIC_COMPARE_REG = noteBuffer;

}


void MusicObj::musicOff()
{
	onFlag = false;
	MUSIC_CONTROL_A = controlAOffSettings;
	MUSIC_CONTROL_B = controlBOffSettings;
	MUSIC_COMPARE_REG = 0;
	BIT_ON(MUSIC_OUT_PORT_GROUP, MUSIC_OUT_PIN);
	BIT_OFF(MUSIC_OUT_DATA_GROUP, MUSIC_OUT_PIN);
	musicPosition = 0;
}

void MusicObj::musicDown()
{
	MUSIC_CONTROL_A = controlAOffSettings;
	MUSIC_CONTROL_B = controlBOffSettings;
	MUSIC_COMPARE_REG = 0;
	BIT_OFF(MUSIC_OUT_DATA_GROUP, MUSIC_OUT_PIN);
}

void MusicObj::musicOut()
{
	MUSIC_CONTROL_A = controlAOnSettings;
	MUSIC_CONTROL_B = controlBOnSettings;
	BIT_ON(MUSIC_OUT_DATA_GROUP, MUSIC_OUT_PIN);
}

MusicObj::MusicObj(uint32_t cpuFreq128th, uint16_t tickFreq) //Ticks to reach 1 second.
{
	//Clear Output on Compare match, Fast PWM, Top Input Capture Register
	//PWM Frequency 31250/OCR0A Hz.
	controlAOnSettings = (1<<COM0A0)|(1<<WGM01);
	controlBOnSettings = (1<<CS01)|(1<<CS00);
	controlAOffSettings = (1<<WGM01)|(1<<WGM00);
	controlBOffSettings = (WGM02);
	musicPosition = 0;
	musicTime = 0;
	noteBuffer = 0;
	musicTicks = 0;
	channelVolume = 0;
	/*
	for (uint8_t i = 0; i < 12; i++)
	{
	const float o2noteboxVal = 15.289;
	//Value of Max ICR of O2 with 1000Hz Max frequency
	float val = ((float)cpuFreq128th/500);
	float val2 = 1.0594631;
	float temp[12];
	temp[i] = (o2noteboxVal*val);
	for (int j = 0; j < i; j++)
	{
	temp[i] /= val2;
	}
	o2notebox[i] = temp[i];
	}
	*/
	o2notebox[0] = 477;
	o2notebox[1] = 450;
	o2notebox[2] = 425;
	o2notebox[3] = 401;
	o2notebox[4] = 379;
	o2notebox[5] = 357;
	o2notebox[6] = 337;
	o2notebox[7] = 318;
	o2notebox[8] = 300;
	o2notebox[9] = 284;
	o2notebox[10] = 268;
	o2notebox[11] = 253;
	
	
	musicTimeConst = (tickFreq/32); //80ms

	sheetLength = sizeof(sheet0)/2;

	MUSIC_CONTROL_A = controlAOffSettings;
	MUSIC_CONTROL_B = controlBOffSettings;
	MUSIC_INTERRUPT_MASK_REG = 0;
	MUSIC_COMPARE_REG = 0;
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
			MUSIC_COMPARE_REG = noteBuffer;
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

