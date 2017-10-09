#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "spislave.h"

ISR(SPI_STC_vect)
{
	uint8_t y = receiveMasterData();
	if(DOT)
	{
		triggerDot()
	}
	else if(DASH)
	{
		triggerDash();
	}
		
	PORTD = y;
}

int main(void)
{
	DDRD = (1<<PIND0)
	initSPISlave();                             //Initialize slave SPI

	sei();
	while(1)
	{
		//Interrupt will do the work
		//when a master sends it data.  
	}
}