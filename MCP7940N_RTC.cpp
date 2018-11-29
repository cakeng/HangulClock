/*
*	MCP7940N_RTC.cpp
*
*	Created: 2018-07-13 오후 8:40:18
*	Modified 2018-11-19 for Attiny 4313
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose
*	other than personal use.
*
*/

#include "MCP7940N_RTC.h"

inline void MCP7940n::i2cWrite(uint8_t address, uint8_t data)
{
	i2cStart();
	i2cSend(ADR_WRITE);
	i2cAck();
	i2cSend(address);
	i2cAck();
	i2cSend(data);
	i2cAck();
	i2cEnd();
}

inline void MCP7940n::i2cRead(uint8_t address, uint8_t& data)
{
	i2cStart();
	i2cSend(ADR_WRITE);
	i2cAck();
	i2cSend(address);
	i2cAck();
	i2cEnd();
	
	i2cStart();
	i2cSend(ADR_READ);
	i2cAck();
	data = i2cRecive();
	i2cAck();
	i2cEnd();
}



inline void MCP7940n::i2cSend(uint8_t data)
{
	USIDR = data;
	USISR &= 0xf0;
	while (!(USISR&(1<<USIOIF)))
	{
		USICR |= (1<<USITC);
		_delay_us(2);
	}
	USIDR = 0xff;
	USISR |= (1<<USIOIF);
}

inline uint8_t MCP7940n::i2cRecive()
{
	uint8_t dat;
	USIDR = 0xff;
	USISR &= 0xf0;
	while (!(USISR&(1<<USIOIF)))
	{
		USICR |= (1<<USITC);
		_delay_us(2);
	}
	dat = USIDR;
	USIDR = 0xff;
	USISR |= (1<<USIOIF);
	return dat;
}


inline void MCP7940n::i2cStart()
{
	DDRB |= 0b10100000;
	PORTB |= 0b00100000;
	PORTB &= 0xdf;
	_delay_us(2);
	USICR |= (1<<USITC);
	_delay_us(2);
	PORTB |= 0x20;
}

inline void MCP7940n::i2cEnd()
{
	PORTB &= 0xdf;
	_delay_us(2);
	USICR |= (1<<USITC);
	_delay_us(2);
	PORTB |= 0x20;
	_delay_us(2);
}

inline void MCP7940n::i2cAck()
{
	USICR |= (1<<USITC);
	_delay_us(2);
	USICR |= (1<<USITC);
	_delay_us(2);
}

inline void MCP7940n::i2cAck(bool b)
{
	PORTB &= 0xdf;
	_delay_us(2);
	USICR |= (1<<USITC);
	_delay_us(2);
	USICR |= (1<<USITC);
	_delay_us(2);
	PORTB |= 0x20;
}

void MCP7940n::rtcReadRoutine(uint8_t address, int8_t& data)
{
	uint8_t temp = 0;
	i2cRead(address, temp);
	if(address == RTCHOUR)
	{
		temp &= 0x3f;
	}
	temp = bcd2dec(temp);
	data = temp;
}

void MCP7940n::rtcWriteRoutine(uint8_t address, int8_t data)
{
	uint8_t temp = dec2bcd(data);
	if (address == RTCHOUR)
	{
		temp &= 0x3f;
	}
	if (address == RTCSEC)
	{
		temp |= 0x80;
		//data &= 0x7f;
		//DS1307
	}
	i2cWrite(address, temp);
}

inline void MCP7940n::rtcReadRoutineAll(int8_t& h,int8_t& m,int8_t& s)
{
	i2cStart();
	i2cSend(ADR_WRITE);
	i2cAck();
	i2cSend(RTCSEC);
	i2cAck();
	i2cEnd();
	
	i2cStart();
	i2cSend(ADR_READ);
	i2cAck();
	s = i2cRecive();
	i2cAck(true);
	m = i2cRecive();

	i2cAck(true);
	h = i2cRecive();
	i2cAck();
	i2cEnd();
	
	h &= 0x3f;
	s = bcd2dec(s);
	m = bcd2dec(m);
	h = bcd2dec(h);
}

void MCP7940n::loadTime()
{
	DDRB |= 0b10100000;
	PORTB |= 0b00100000;
	/*rtcReadRoutine(RTCSEC, secs);
	rtcReadRoutine(RTCMIN, mins);
	rtcReadRoutine(RTCHOUR, hours);*/
	rtcReadRoutineAll(hours, mins, secs);
}

void MCP7940n::saveTime(bool secT)
{
	DDRB |= 0b10100000;
	PORTB |= 0b00100000;
	if (secT)
	{
		rtcWriteRoutine(RTCSEC, secs);
	}
	else
	{
		rtcWriteRoutine(RTCSEC, 0);
	}
	rtcWriteRoutine(RTCMIN, mins);
	rtcWriteRoutine(RTCHOUR, hours);
}

void MCP7940n::saveTime()
{
	DDRB |= 0b10100000;
	PORTB |= 0b00100000;
	rtcWriteRoutine(RTCSEC, 0);
	rtcWriteRoutine(RTCMIN, mins);
	rtcWriteRoutine(RTCHOUR, hours);
}

MCP7940n::MCP7940n() : ClockWorks(0,0,0,0)
{
	//PB5 - SDA, PB7 -SCK
	cli();
	DDRB |= 0b10100000;
	PORTB |= 0b00100000;
	USICR = (1<<USIWM1)|(1<<USICS1)|(1<<USICLK);
	USICR |= (1<<USITC);
	loadTime();
	//enableOscillator();
	enableBattery();
	sei();
}

