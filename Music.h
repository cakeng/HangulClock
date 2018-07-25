﻿/*
*	Music.h
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

#ifndef MUSIC_CAKENG_H
#define MUSIC_CAKENG_H
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Basic.h"

<<<<<<< HEAD
#ifndef MUSIC_CONTROL_A
=======
#ifndef MUSIC_CONTROL_A	
>>>>>>> be5ce2b47a916e376bcfe0e026002c3cdaf2fabe
#define MUSIC_CONTROL_A				TCCR0A
#define MUSIC_CONTROL_B				TCCR0B
#define MUSIC_INTERRUPT_MASK_REG	TIMSK0
#define MUSIC_COMPARE_REG			OCR0A
#define MUSIC_TIMER1_REG				TCNT0

#define MUSIC_OUT_DATA_GROUP		DDRB
#define MUSIC_OUT_PORT_GROUP		PORTB
#define MUSIC_OUT_PIN					DDB2
#endif

class MusicObj
{
	private:

	bool onFlag;
	uint16_t sheetLength;
	volatile uint16_t musicPosition;
	volatile uint16_t musicTime;
	volatile uint16_t noteBuffer;
	volatile uint8_t channelVolume;
	uint16_t musicTimeConst;
	uint16_t o2notebox[12]; //Used Set of max values of ICR
	volatile uint16_t musicTicks;

	uint8_t controlAOffSettings;
	uint8_t controlBOffSettings;
	uint8_t controlAOnSettings;
	uint8_t controlBOnSettings;

	void dataRead(uint16_t data);

	public:
	bool isOn()
	{
		return onFlag;
	}
	void musicOn()
	{
		onFlag = true;
	}
	void musicOff();

	void musicDown();

	void musicOut();

	MusicObj(uint32_t cpuFreq128th, uint16_t tickFreq);

	void musicFunction();
	
	void autoRoutine();
};

#define _MUSIC_YUKINOHANA


#ifdef _MUSIC_YUKINOHANA
const PROGMEM uint16_t sheet0[] = {
	0
	,0b0001001001011111,0b0001001001011111,0b0001001010011111,0b0001001010010111,0b0001001010011111,0b0000101010010111,0b0001101010011111,0b0001001010010111,0b0001001010000111,
	0b0001001001110111,0b0001001010000111,0b0000101010111111,0b0010101010111111,0b0000101010010000,0b0001001010010111,0b0000101010011111,0b0000111010101111,0b0000111010010111,0b0000101010000111,
	0b0001001010000111,0b0000101001110111,0b0010101001110111,0b0010001001011000,0b0000111001011111,0b0000111001100111,0b0000101001110111,0b0001001001110111,0b0001001001100111,0b0001001001100111,
	0b0000101010000111,0b0001001001110111,0b0000101001011111,0b0000101001001111,0b0001011000111111,0b0000011000100000,0b0000101000100111,0b0000101000111111,0b0001001001110111,0b0001001001100111,
	0b0001001001100111,0b0000101010000111,0b0001101010000111,0b0001001001111111,0b0001001010000111,0b0001001010010111,0b0001001010011111,0b0001001010010111,0b0001001010011111,0b0000101010010111,
	0b0001101010011111,0b0001001010010111,0b0001001010000111,0b0001001001110111,0b0001001010000111,0b0000101010111111,0b0010101010111111,0b0000101010010000,0b0001001010010111,0b0000101010011111,
	0b0000111010101111,0b0000111010010111,0b0000101010000111,0b0001001010000111,0b0001001001110111,0b0001001001110111,0b0000101010101111,0b0001111010011111,0b0000111001011000,0b0000111001011111,
	0b0000111001100111,0b0000101001110111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0001001001110111,0b0001111001011111,0b0000011001001000,0b0001001001001111,0b0000101000111111,
	0b0000101000111111,0b0000101001110111,0b0000101000111111,0b0000101001001111,0b0000101000111111,0b0000101001011111,0b0000101000010111,0b0000101000111111,0b0000101001001111,0b0000101001011111,
	0b0000101000111111,0b0000101001011111,0b0001101001100111,0b0001001000111111,0b0001001010000111,0b0001001010000111,0b0001001010000111,0b0000101010011111,0b0001101010010111,0b0000101010011111,

	0b0001101010101111,0b0000101010010111,0b0001101001110111,0b0001001001110111,0b0001001001011111,0b0000101001110111,0b0010101010000111,0b0001001001011000,0b0001001001011111,0b0001001001100111,
	0b0001001001011111,0b0001001000111111,0b0000101001001111,0b0001101001100111,0b0001001001011111,0b0001001001001111,0b0001001000111111,0b0001001001001111,0b0000101000111111,0b0001001001001111,
	0b0000101001110111,0b0010011001011111,0b0000111000111000,0b0001001000111111,0b0001001000111111,0b0001001010000111,0b0000101010000111,0b0001101010000111,0b0001001010011111,0b0001001010010111,
	0b0000101010011111,0b0001101010101111,0b0001001010010111,0b0001001001110111,0b0000101001110111,0b0001001010101111,0b0000101010010111,0b0010101010011111,0b0000101010000000,0b0001001010000111,
	0b0001001010010111,0b0001001010011111,0b0000101010011111,0b0001001010011111,0b0000101010010111,0b0001101010011111,0b0000101010000000,0b0001001010000111,0b0001001010010111,0b0001001010011111,
	0b0000011010011111,0b0000111010011000,0b0000011010011111,0b0000111010011000,0b0000011010011111,0b0000111010011000,0b0000011010011111,0b0000111010101000,0b0000011010101111,0b0000111010101000,
	0b0000011010101111,0b0000111010101000,0b0000011010101111,0b0000111010101000,0b0000011010101111,0b0000111000110000,0b0000101000110111,0b0000101001101111,0b0000101010000111,0b0000101001101111,
	0b0000101010010111,0b0000101010000111,0b0000101010011111,0b0000101010101111,0b0001101011000111,0b0000101001011000,0b0001001001011111,0b0001001001011111,0b0001001010011111,0b0001001010010111,
	0b0001001010011111,0b0000101010010111,0b0001101010011111,0b0001001010010111,0b0001001010000111,0b0001001001110111,0b0001001010000111,0b0000101010111111,0b0010101010111111,0b0000101010010000,
	0b0001001010010111,0b0000101010011111,0b0000111010101111,0b0000111010010111,0b0000101010000111,0b0001001010000111,0b0000101001110111,0b0010101001110111,0b0010001001011000,0b0000111001011111,

	0b0000111001100111,0b0000101001110111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0000101010000111,0b0001001001110111,0b0000101001011111,0b0000101001001111,0b0001011000111111,
	0b0000011000100000,0b0000101000100111,0b0000101000111111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0000101010000111,0b0001101010000111,0b0001001001111111,0b0001001010000111,
	0b0001001010010111,0b0001001010011111,0b0001001010010111,0b0001001010011111,0b0000101010010111,0b0001011010011111,0b0000011010010000,0b0001001010010111,0b0001001010000111,0b0001001001110111,
	0b0001001010000111,0b0000101010111111,0b0010101010111111,0b0000101010010000,0b0001001010010111,0b0000101010011111,0b0000111010101111,0b0000111010010111,0b0000101010000111,0b0001001010000111,
	0b0000101001110111,0b0010101001110111,0b0010001001011000,0b0000111001011111,0b0000111001100111,0b0000101001110111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0000101010000111,
	0b0001001001110111,0b0000101001011111,0b0000101001001111,0b0001011000111111,0b0000011000100000,0b0000101000100111,0b0000101000111111,0b0001001001110111,0b0001001001100111,0b0001001001100111,
	0b0000101010000111,0b0001101010000111,0b0001001001111111,0b0001001010000111,0b0001001010010111,0b0001001010011111,0b0001001010010111,0b0001001010011111,0b0000101010010111,0b0001011010011111,
	0b0000011010010000,0b0001001010010111,0b0001001010000111,0b0001001001110111,0b0001001010000111,0b0000101010111111,0b0010101010111111,0b0000101010010000,0b0001001010010111,0b0000101010011111,
	0b0000111010101111,0b0000111010010111,0b0000101010000111,0b0001001010000111,0b0001001001110111,0b0001001001110111,0b0000101010101111,0b0001111010011111,0b0000111001011000,0b0000111001011111,
	0b0000111001100111,0b0000101001110111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0000101010000111,0b0001001001110111,0b0000101000111111,0b0001111000111111,0b0000011000100000,

	0b0000101000100111,0b0000101000111111,0b0001001001110111,0b0001001001100111,0b0001001001100111,0b0000101010000111,0b0001001001110111,0b0000101010011111,0b0001111010011111,0b0000011000111000,
	0b0000101000111111,0b0000101001001111,0b0001011001100111,0b0000011001011000,0b0001001001011111,0b0000011001100000,0b0001001001100111,0b0000111001110111,0b0010011001011111,0b0000011001001000,
	0b1000001001001111,0b0010001000111000,0b0001001000111111,0b0000101000110111,0b0110101000111111,0b0001000111111111,0b0000101000010111,0b0001001000010111,0b0001001000000111,0b0010101000000111,
	0b0010001000111111,0b0001001000100111,0b0000101000111111,0b0001101000110111,0b0000100110110111,0b0000100111010111,0b0000100111101111,0b0000101000010111,0b0010001001001111,0b0001001000100000,
	0b0001001000100111,0b0000101000110111,0b0001101000111111,0b0001000111011111,0b0000011000000000,0b0001001000000111,0b0001001000010111,0b0000011000111000,0b0001001000111111,0b0000011001100000,
	0b0001011001100111,0b0001111001110111,0b0000011010011000,0b0001111010011110,0b0001001001110000,0b0111001001110111,0b0000011001011000,0b0100001001011111
	,0
};
//714bytes



#endif


#endif
