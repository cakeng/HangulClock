/*
*	DisplayOut.cpp
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

#include "DisplayOut.h"

uint16_t hourData[12][2] =
{
	{
		0b0000000000000101,
		0b0000000000000010
	},//12
	{
		0b0000000000000010,
		0b0000000000000010
	},//1
	{
		0b0000000000000001,
		0b0000000000000010
	},//2
	{
		0b0000000000010000,
		0b0000000000000010
	},//3
	{
		0b0000000010000000,
		0b0000000000000010
	},//4
	{
		0b0000000100001000,
		0b0000000000000010
	},//5
	{
		0b0000001100000000,
		0b0000000000000010
	},//6
	{
		0b0000000001100000,
		0b0000000000000010
	},//7
	{
		0b0000001000000000,
		0b0000000000010010
	},//8
	{
		0b0000000000000000,
		0b0000000000001110
	},//9
	{
		0b0000000000000100,
		0b0000000000000010
	},//10
	{
		0b0000000000000110,
		0b0000000000000010
	}//11
};

uint16_t minData[11] =
{
	0b0000010000000000,//0
	0b0000000000000001,//1
	0b0000001000000000,//2
	0b0000000100000000,//3
	0b0100000000000000,//4
	0b0010000000000000,//5
	0b0000000001000000,//6
	0b0000000000100000,//7
	0b0001000000000000,//8
	0b0000100000000000,//9
	0b0000000010000000//10
};


uint16_t heartData[2] =

{
	0b0000000101000000,
	0b0001000101010101
};


DisplayOut::DisplayOut(uint8_t cons)
{
	PORTD &= 0b11100000; //Turn off columns. (NPN)
	PORTD |= 0b01100000;
	PORTB |= 0b00000111; //Turn off rows. (PNP)
	screenBuffer[0] = 0;
	screenBuffer[1] = 0;
	rowNum = 0;
	shiftOutBuffer = 0;
	flashHour = false;
	minNum = 0;
	brightness = 5;
}


void DisplayOut::refreshDisplay()
{
	PORTD &= 0b11100000; //Turn off columns. (NPN)
	PORTD |= 0b01100000;
	PORTB |= 0b00000111; //Turn off rows. (PNP)
	rowNum++;
	if (rowNum == 1)
	{
		PORTB &= 0b11111011; // Turn on row 1 
		PORTD |= (uint8_t)(screenBuffer[0]&0x001f); //Turn on columns.
	}
	else if (rowNum == 2)
	{
		PORTB &= 0b11111101; // Turn on row 2 
		PORTD |= (uint8_t)((screenBuffer[0]>>5)&0x001f);
	}
	else if (rowNum == 3)
	{
		PORTB &= 0b11111110; // Turn on row 3 
		PORTD |= (uint8_t)(screenBuffer[1]&0x001f);
	}
	else if (rowNum == 4)
	{
		PORTD &= 0b10111111; // Turn on row 4 
		PORTD |= (uint8_t)((screenBuffer[1]>>5)&0x001f);
	}
	else if (rowNum == 5)
	{
		PORTD &= 0b11011111; // Turn on row 5 
		PORTD |= (uint8_t)((screenBuffer[1]>>10)&0x001f);
	}
	else if (rowNum > brightness)
	{
		rowNum = 0;
	}
}

void DisplayOut::flashDisplay()
{

	static uint8_t ticks = 0;
	ticks++;
	if(ticks > 14)
	{
		minNum++;
		if (hourOut)
		{
			hourOut = false;
		}
		else
		{
			hourOut = true;
		}
		ticks = 0;
	}
}


void DisplayOut::setDisplay(uint8_t * arr)
{
screenBuffer[0] = (arr[0]&0x1f);
screenBuffer[0] |= ((arr[1]&0x1f)<<5);
screenBuffer[1] = (arr[2]&0x1f);
screenBuffer[1] |= ((arr[3]&0x1f)<<5);
screenBuffer[1] |= ((arr[4]&0x1f)<<10);
}

void DisplayOut::setDisplay(uint16_t data1, uint16_t data2)
{
screenBuffer[0] = data1;
screenBuffer[1] = data2;
}


void DisplayOut::setDisplay(ClockWorks& clockObj)
{
	if (hourOut|(!flashHour))
	{
		screenBuffer[0] = hourData[clockObj.getHour()%12][0];
	}
	else
	{
		screenBuffer[0] = 0;
	}
	
	
	if (clockObj.getMin() == 0)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1];
	}
	else if (clockObj.getMin()/10 == 0)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()]|minData[0];
	}
	else if (clockObj.getMin()%10 == 0)
	{
		if (clockObj.getMin()/10 == 1)
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[10]|minData[0];
		}
		else
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()/10]|minData[10]|minData[0];
		}
	}
	else
	{
		if (minNum == 0)
		{
			if (clockObj.getMin()/10 == 1)
			{
				screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[10];
			}
			else
			{
				screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()/10]|minData[10];
			}
		}
		if (minNum == 1)
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()%10]|minData[0];
		}
		else
		{
			minNum = 0;
		}
	}
	
	
	/*
	if (minNum == 0)
	{
		if (clockObj.getMin()/10 < 2)
		{
			minNum++;
		}
		else
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()/10];
		}
	}
	if (minNum == 1)
	{
		if (clockObj.getMin()/10 == 0)
		{
			minNum++;
		}
		else
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[10];
		}
	}
	if (minNum == 2)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1];
		minNum++;
	}
	if (minNum == 3)
	{
		if (clockObj.getMin()%10 == 0)
		{
			minNum++;
		}
		else
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[clockObj.getMin()%10];
		}
	}
	if (minNum == 4)
	{
		if (clockObj.getMin() != 0)
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|minData[0];
		}
	}
	if (minNum == 6)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1];
		minNum = 0;
	}
	if (minNum == 7)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1];
		minNum = 0;
	}
	*/
}

void DisplayOut::setDisplayBin(ClockWorks& clockObj)
{

	screenBuffer[0] = clockObj.getHour()&0x1F;
	screenBuffer[0] |= ((clockObj.getMin()/10)&0x1F)<<5;
	screenBuffer[1] = ((clockObj.getMin()%10)&0x1F);
	screenBuffer[1] |= ((clockObj.getSec()/10)&0x1F)<<5;
	screenBuffer[1] |= ((clockObj.getSec()%10)&0x1F)<<10;

	screenBuffer[0] = clockObj.getHour()&0x1F;
	screenBuffer[0] |= ((clockObj.getMin()/10)&0x1F)<<5;
	screenBuffer[1] = ((clockObj.getMin()%10)&0x1F);
	screenBuffer[1] |= ((clockObj.getSec()/10)&0x1F)<<5;
	screenBuffer[1] |= ((clockObj.getSec()%10)&0x1F)<<10;

}

void DisplayOut::setDisplayHeart()
{
	if (minNum)
	{
		screenBuffer[0] = heartData[0];
		screenBuffer[1] = heartData[1];
	}
	else
	{
		screenBuffer[0] = 0;
		screenBuffer[1] = 0;
	}
}
