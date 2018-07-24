/*
*	DisplayOut.cpp
*
*	Created: 2018-07-10 오전 6:43:41
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/

#include "DisplayOut.h"

uint16_t hourData[12][2] =
{
	{
		0b0000000000010100,
		0b0000000000001000
	},//12
	{
		0b0000000000001000,
		0b0000000000001000
	},//1
	{
		0b0000000000010000,
		0b0000000000001000
	},//2
	{
		0b0000000000000001,
		0b0000000000001000
	},//3
	{
		0b0000000010000000,
		0b0000000000001000
	},//4
	{
		0b0000000001000010,
		0b0000000000001000
	},//5
	{
		0b0000000001100000,
		0b0000000000001000
	},//6
	{
		0b0000001100000000,
		0b0000000000001000
	},//7
	{
		0b0000000000100000,
		0b0000000000001001
	},//8
	{
		0b0000000000000000,
		0b0000000000001110
	},//9
	{
		0b0000000000000100,
		0b0000000000001000
	},//10
	{
		0b0000000000001100,
		0b0000000000001000
	}//11
};

uint16_t min1Data[10] =
{
	0b0100000000000000,//0
	0b0000000000010000,//1
	0b0000000000100000,//2
	0b0000000001000000,//3
	0b0000010000000000,//4
	0b0000100000000000,//5
	0b0000000100000000,//6
	0b0000001000000000,//7
	0b0001000000000000,//8
	0b0010000000000000//9
};

uint16_t min10Data[5] =
{
	0b0000000010000000,//1
	0b0000000010100000,//2
	0b0000000011000000,//3
	0b0000010010000000,//4
	0b0000100010000000//5
};

uint16_t heartData[2] = 
{
	0b0000000101000000,
	0b0001000101010101
};


DisplayOut::DisplayOut(uint8_t cons)
{
	BIT_ON(SHIFTOUT_DATA_GROUP, SHIFTOUT_SERIAL_CLOCK_PIN);
	BIT_ON(SHIFTOUT_DATA_GROUP, SHIFTOUT_SERIAL_DATA_PIN);
	BIT_ON(SHIFTOUT_DATA_GROUP, SHIFTOUT_SERIAL_LATCH_PIN);
	BIT_ON(SHIFTOUT_DATA_GROUP, SHIFTOUT_ROW1);
	BIT_ON(SHIFTOUT_DATA_GROUP, SHIFTOUT_ROW2);
	//DDRA = 0b00111000;
	screenBuffer[0] = 0;
	screenBuffer[1] = 0;
	rowNum = 0;
	shiftOutBuffer = 0;
	flashHour = false;
	min1Out = false;
	brightness = 0;
	shiftOut(0x07);
	BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW1);
	BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW2);
}

void DisplayOut::shiftOut(uint8_t data)
{
	BIT_OFF(SHIFTOUT_PORT_GROUP, SHIFTOUT_SERIAL_LATCH_PIN);
	for(uint8_t i = 8; 0 < i; i--)
	{
		BIT_OFF(SHIFTOUT_PORT_GROUP, SHIFTOUT_SERIAL_CLOCK_PIN);
		
		BIT_OFF(SHIFTOUT_PORT_GROUP, SHIFTOUT_SERIAL_DATA_PIN);
		SHIFTOUT_PORT_GROUP |= (((data>>(i-1))&1)<<SHIFTOUT_SERIAL_DATA_PIN);
		
		BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_SERIAL_CLOCK_PIN);
	}
	BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_SERIAL_LATCH_PIN);
}

void DisplayOut::refreshDisplay()
{
	shiftOut(0x07);
	BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW1);
	BIT_ON(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW2);
	shiftOutBuffer = 0x07;
	if (rowNum - brightness == 0)
	{
		BIT_OFF(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW1);
		shiftOutBuffer |= ((screenBuffer[0]&0x1F)<<3);
	}
	else if (rowNum - brightness == 1)
	{
		BIT_OFF(SHIFTOUT_PORT_GROUP, SHIFTOUT_ROW2);
		shiftOutBuffer |= (((screenBuffer[0]>>5)&0x1F)<<3);
	}
	else if(rowNum - brightness > 1)
	{
		shiftOutBuffer &= ~(1<<(rowNum - brightness - 2));
		shiftOutBuffer |= (((screenBuffer[1]>>(5*(rowNum - brightness - 2)))&0x1F)<<3);
	}
	shiftOut(shiftOutBuffer);
	rowNum++;
	if (rowNum - brightness == 5)
	{
		rowNum = 0;
	}
}

void DisplayOut::flashDisplay()
{
	static uint8_t ticks = 0;
	ticks++;
	if(ticks > 1)
	{
		if (min1Out)
		{
			min1Out = false;
		}
		else
		{
			min1Out = true;
		}
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

/*
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
*/

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
		screenBuffer[1] = hourData[clockObj.getHour()%12][1]|min1Data[clockObj.getMin()%10]|min1Data[0];
	}
	else if (clockObj.getMin()%10 == 0)
	{
		screenBuffer[1] = hourData[clockObj.getHour()%12][1]|min10Data[clockObj.getMin()/10 - 1]|min1Data[0];
	}
	else
	{
		if (min1Out)
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|min1Data[clockObj.getMin()%10]|min1Data[0];

		}
		else
		{
			screenBuffer[1] = hourData[clockObj.getHour()%12][1]|min10Data[clockObj.getMin()/10 - 1];

		}
	}
}

void DisplayOut::setDisplayBin(ClockWorks& clockObj)
{
screenBuffer[0] = clockObj.getHour()&0x1F;
screenBuffer[0] |= ((clockObj.getMin()/10)&0x1F)<<5;
screenBuffer[1] = ((clockObj.getMin()%10)&0x1F);
screenBuffer[1] |= ((clockObj.getSec()/10)&0x1F)<<5;
screenBuffer[1] |= ((clockObj.getSec()%10)&0x1F)<<10;
}

void DisplayOut::setDisplayHeart()
{
	if (min1Out)
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
