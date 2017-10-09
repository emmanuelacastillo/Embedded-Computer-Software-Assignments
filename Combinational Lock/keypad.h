#include <avr/io.h>

#include <util/delay.h>

#include "myutils.h"

#ifndef _KEYPAD_H
#define _KEYPAD_H


/************************************************
	KEYPAD CONNECTIONS
*************************************************/

 #define KEYPAD D

uint8_t GetKeyPressed();
uint16_t getEnteredPassword(char *msg);

/*_________________________________________________________________________________________*/
#endif
