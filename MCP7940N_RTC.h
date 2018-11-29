/*
*	MCP7940N_RTC.h
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

#define  F_CPU 16000000
#ifndef MCP7940N_RTC_CAKENG_H
#define MCP7940N_RTC_CAKENG_H

#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Basic.h"
#include "ClockWorks.h"


#define ADR_WRITE (0b11011110)
#define ADR_READ  (0b11011111)

/* For DS1307...*/
/*
#define ADR_WRITE (0b11010000)
#define ADR_READ (0b11010001)*/

#define RTCSEC    (0x00)
#define RTCMIN    (0x01)
#define RTCHOUR   (0x02)
#define RTCWKDAY  (0x03)
#define RTCDATE   (0x04)
#define RTCMTH    (0x05)
#define RTCYEAR   (0x06)

class MCP7940n : public ClockWorks
{
	private:
	
	inline void i2cRead(uint8_t address, uint8_t& data);

	inline void i2cWrite(uint8_t address, uint8_t data);
	
	inline void i2cStart();
	
	inline void i2cEnd();
	
	inline void i2cSend(uint8_t data);
	
	inline uint8_t i2cRecive();
	
	inline void i2cAck();
	
	inline void i2cAck(bool b);
	
	void rtcReadRoutine(uint8_t address, int8_t& data);
	
	void rtcWriteRoutine(uint8_t address, int8_t data);
	
	void rtcReadRoutineAll(int8_t& h,int8_t& m,int8_t& s);


	uint8_t dec2bcd(int8_t dec)
	{
		uint8_t temp = (uint8_t) dec;
		return ((temp%10)&0x0f)|((temp/10)<<4);
	}
	
	int bcd2dec(uint8_t bcd)
	{
		return ((bcd&(0x70))>>4)*10+(bcd&(0x0f));
	}
	
	public:
	/*
	bool loadSec();
	bool loadMin();
	bool loadHour();
	bool saveSec();
	bool saveMin();
	bool saveHour();
	*/
	void loadTime();
	void saveTime(bool secT);
	void saveTime();

	void enableOscillator()
	{
		i2cWrite(RTCSEC, (secs|0x80));
	}

	void enableBattery()
	{
		i2cWrite(RTCWKDAY, 0x08);
	}

	MCP7940n();

};

#endif