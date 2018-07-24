/*
*	Basic.h
*
*	Created: 2018-06-26 오전 6:26:58
*	Author: Cakeng (PARK JONG SEOK)
*
*	NO LICENCE INCLUDED
*	Contact cakeng@naver.com to
*	use, modify, or share the software for any purpose.
*
*/
#ifndef _BASIC_CAKENG_H
#define _BASIC_CAKENG_H
#include <avr/io.h>

#define BIT_SELECT(x) (1<<(x))
#define BIT_EXCEPT(x) (~(1<<(x)))
#define BIT_ON(x, y) ((x) |= (1<<(y)))
#define BIT_OFF(x, y) ((x) &= ~(1<<(y)))
#define BIT_CHECK(x, y) (((x) & (1<<(y)))>0)

#endif
