#include "spislave.h"

#define DOT = 0b00000001
#define DASH = 0b00000010

void inline beep()
{
	for(char i=50;i;i--)
	{
		DDRD|=(1<<PIND0);
		_delay_ms(1);
		DDRD&=~(1<<PIND0);
		_delay_ms(1);
	}
}

void triggerDot()
{
	beep();_delay_ms(100);
}
void triggerDash()
{
	beep();
	beep();
	beep();
	_delay_ms(100);
}

volatile uint8_t data;

void initSPISlave(void)
{
	DDRB=(1<<PINB4);               //MISO as OUTPUT
	SPCR=(1<<SPE)|(1<<SPIE);       //Enable SPI && interrupt enable bit
	SPDR=0;
}

uint8_t receiveMasterData(void)
{
	/* Wait for reception complete */
	while (!(SPSR & (1<<SPIF)))   ;
	/* Return data register */
	return    SPDR;
}