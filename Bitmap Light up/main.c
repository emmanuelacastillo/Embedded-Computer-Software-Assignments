/*
 * ECPII.c
 *
 * Created: 7/29/2016 7:55:27 PM
 * Author : Emmanuel
 */ 

#define F_CPU 16000000L // Specify MCU speed so that delay is correct
#define NELEMENTS(x) (sizeof(x)/sizeof((x)[0]))

#include <avr/io.h>
#include <util/delay.h>

const int LIGHT_UP_BITMAP[] =
{
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
};

const int bitMapSize = 4;
const int delayTime = 500;

void turnLightsOn();

int main(void)
{
	DDRB = 0b00100000;
	// set bit 5 high to set PORTB,5 as output
	// PORTB,5 is Digital Pin 13 on the Arduino
	
	while(1)
		for(int bitCount = 0; bitCount < NELEMENTS(LIGHT_UP_BITMAP); bitCount++)
			turnLightsOn(LIGHT_UP_BITMAP[bitCount]);
}

void turnLightsOn(int bits)
{
	PORTB = bits; // set bit 5 high to turn on pin
	_delay_ms(delayTime);
	PORTB = bits; // set bit 5 low to turn off pin
	_delay_ms(delayTime);
}