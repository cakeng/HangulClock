/*
*	MCP7940N_RTC.h
*
*	Created: 2018-07-13 오후 8:40:18
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/
#define  F_CPU 8000000
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
#define ADR_WRITE_1307 (0b11010000)
#define ADR_READ_1307  (0b11010001)

#define RTCSEC    (0x00)
#define RTCMIN    (0x01)
#define RTCHOUR   (0x02)
#define RTCWKDAY  (0x03)
#define RTCDATE   (0x04)
#define RTCMTH    (0x05)
#define RTCYEAR   (0x06)

#ifndef	I2C_PORT_GROUP
#define I2C_PORT_GROUP			PORTB
#define I2C_DATA_GROUP			DDRB
#define  I2C_INPUT_GROUP			PINB
#define I2C_CLOCK_PIN	PORTB0
#define I2C_DATA_PIN	PORTB1
#endif

class MCP7940n : public ClockWorks
{
	private:
	bool i2cWriteLoop(uint8_t data);
	void i2cReadLoop(uint8_t& data, bool ack);
	bool i2cWrite(uint8_t address, uint8_t data);
	bool i2cRead(uint8_t address, uint8_t& data);
	/*
	bool i2cReadConcecutive(uint8_t address, uint8_t& data1, uint8_t& data2, uint8_t& data3);
	bool i2cWriteConcecutive(uint8_t address, uint8_t data1, uint8_t data2, uint8_t data3);
	*/
	
	inline bool i2cReadRoutine(uint8_t address, int8_t& data);
	inline bool i2cWriteRoutine(uint8_t address, int8_t data);

	int dec2bcd(uint8_t dec)
	{
		return ((dec%10)&0x0f)|((dec/10)<<4);
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
	bool loadTime();
	bool saveTime();

	MCP7940n();

};

#endif