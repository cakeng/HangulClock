/*
*	MCP7940N_RTC.cpp
*
*	Created: 2018-07-13 오후 8:40:18
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/

#include "MCP7940N_RTC.h"


bool MCP7940n::i2cWriteLoop(uint8_t data)
{
	_delay_loop_1(2);
	for(uint8_t i = 0; i < 8; i++)
	{
		BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		if((data<<i)&0x80)
		{
			BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN);
		}
		else
		{
			BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN);
		}
		_delay_loop_1(2);
		BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
	}
	BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); // SDA Line released
	_delay_loop_1(2);
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	return BIT_CHECK(PINB, I2C_DATA_PIN);
}

void MCP7940n::i2cReadLoop(uint8_t& data, bool ack)
{
	data = 0;
	_delay_loop_1(2);
	for(uint8_t i = 0; i < 8; i++)
	{
		BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
		BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		if(BIT_CHECK(I2C_INPUT_GROUP, I2C_DATA_PIN))
		{
			data |= (0x80>>i);
		}
		_delay_loop_1(2);
	}
	if(ack)
	{
		BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
		BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN); // Acknowledge Bit
		_delay_loop_1(2);
		BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
	}
	else
	{
		BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
		BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); // NOT Acknowledge Bit
		_delay_loop_1(2);
		BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
		_delay_loop_1(2);
	}
}

bool MCP7940n::i2cWrite(uint8_t address, uint8_t data)
{
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN); // Start Bit, Data Line Sourced Low
	bool valid = true;
	_delay_loop_1(2);
	valid &= i2cWriteLoop(ADR_WRITE);
	_delay_loop_1(2);
	valid &= i2cWriteLoop(address);
	if(data!=0xff)
	{
		valid &= i2cWriteLoop(data);
	}
	BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN); // Stop Sequence starts from here.
	_delay_loop_1(2);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN);
	_delay_loop_1(2);
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); // Stop Bit
	return valid;
}

bool MCP7940n::i2cRead(uint8_t address, uint8_t& data)
{
	data = 0;
	bool valid = true;
	valid &= i2cWrite(address, 0xff);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN); // Start Bit, Data Line Sourced Low
	_delay_loop_1(2);
	valid &= i2cWriteLoop(ADR_READ);
	i2cReadLoop(data, false);
	BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN); // Stop Sequence starts from here.
	_delay_loop_1(2);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN);
	_delay_loop_1(2);
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); //Stop Bit
	return valid;
}
/*
bool MCP7940n::i2cWriteConcecutive(uint8_t address, uint8_t data1, uint8_t data2, uint8_t data3)
{
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN); // Start Bit, Data Line Sourced Low
	bool valid = true;
	_delay_loop_1(2);
	valid &= i2cWriteLoop(ADR_WRITE);
	valid &= i2cWriteLoop(address);
	valid &= i2cWriteLoop(data1);
	valid &= i2cWriteLoop(data2);
	valid &= i2cWriteLoop(data3);
	BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN); // Stop Sequence starts from here.
	_delay_loop_1(2);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN);
	_delay_loop_1(2);
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); // Stop Bit
	return valid;
}

bool MCP7940n::i2cReadConcecutive(uint8_t address, uint8_t& data1, uint8_t& data2, uint8_t& data3)
{
	data1 = 0;
	data2 = 0;
	data3 = 0;
	bool valid = true;
	valid &= i2cWrite(address, 0xff);
	_delay_loop_1(2);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN); // Start Bit, Data Line Sourced Low
	valid &= i2cWriteLoop(ADR_READ);
	i2cReadLoop(data1, true);
	i2cReadLoop(data2, true);
	i2cReadLoop(data3, false);
	BIT_OFF(I2C_PORT_GROUP, I2C_CLOCK_PIN); // Stop Sequence starts from here.
	_delay_loop_1(2);
	BIT_ON(I2C_DATA_GROUP, I2C_DATA_PIN);
	_delay_loop_1(2);
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	_delay_loop_1(2);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); //Stop Bit
	return valid;
}
*/
inline bool MCP7940n::i2cReadRoutine(uint8_t address, int8_t& data)
{
	uint8_t temp = 0;
	bool valid = i2cRead(address, temp);
	if(address == RTCHOUR)
	{
		temp &= 0x3f;
	}
	temp = bcd2dec(temp);
	data = temp;
	return valid;
}

inline bool MCP7940n::i2cWriteRoutine(uint8_t address, int8_t data)
{
	data = dec2bcd(data);
	if (address == RTCHOUR)
	{
		data &= 0x3f;
	}
	if (address == RTCSEC)
	{
		data |= 0x80;
	}
	return i2cWrite(address, data);
}
/*
bool MCP7940n::loadSec()
{
	bool valid = true;
	cli();
	valid &= i2cReadRoutine(RTCSEC, secs);
	sei();
	return valid;
}

bool MCP7940n::loadMin()
{
	bool valid = true;
	cli();
	valid &= i2cReadRoutine(RTCMIN, mins);
	sei();
	return valid;
}

bool MCP7940n::loadHour()
{
	bool valid = true;
	cli();
	valid &= i2cReadRoutine(RTCHOUR, hours);
	sei();
	return valid;
}

bool MCP7940n::saveSec()
{
	bool valid = true;
	cli();
	valid &= i2cWriteRoutine(RTCSEC, secs);
	sei();
	return valid;
}

bool MCP7940n::saveMin()
{
	bool valid = true;
	cli();
	valid &= i2cWriteRoutine(RTCMIN, mins);
	sei();
	return valid;
}

bool MCP7940n::saveHour()
{
	bool valid = true;
	cli();
	valid &= i2cWriteRoutine(RTCHOUR, hours);
	sei();
	return valid;
}
*/
bool MCP7940n::loadTime()
{
	bool valid = true;
	cli();
	valid &= i2cReadRoutine(RTCSEC, secs);
	valid &= i2cReadRoutine(RTCMIN, mins);
	valid &= i2cReadRoutine(RTCHOUR, hours);
	sei();
	return valid;
}


bool MCP7940n::saveTime()
{
	cli();
	bool valid = true;
	valid &= i2cWriteRoutine(RTCHOUR, hours);
	valid &= i2cWriteRoutine(RTCMIN, mins);
	valid &= i2cWriteRoutine(RTCSEC, 0);
	sei();
	return valid;
}

MCP7940n::MCP7940n() : ClockWorks(0,0,0,0)
{
	BIT_ON(I2C_DATA_GROUP, I2C_CLOCK_PIN);
	BIT_OFF(I2C_DATA_GROUP, I2C_DATA_PIN); // Clock Line Sourced High
	BIT_ON(I2C_PORT_GROUP, I2C_CLOCK_PIN);
	BIT_OFF(I2C_PORT_GROUP, I2C_DATA_PIN); // DATA Line Pulled High
	loadTime();
	i2cWriteRoutine(RTCSEC, 0);
	i2cWrite(RTCWKDAY, 0x08); // Enable Backup battery
}
