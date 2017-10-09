#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "pinDefines.h"
#include "USART.h"
#include "organ.h"
#include "scale16.h"

#define SENSE_TIME   50
#define THRESHOLD    12000
#define SONG_LENGTH  (sizeof(song) / sizeof(uint16_t))

// -------  Global Variables ---------- //
volatile uint16_t chargeCycleCount;

const uint16_t song[] = {
	E6, E6, E6, C6, E6, G6, G5,
	C6, G5, E5, A5, B5, Ax5, A5,
	G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
	C6, G5, E5, A5, B5, Ax5, A5,
	G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
	/* etc */
};


// ------- Functions -------- //
static inline void runTune();

void initPinChangeInterrupt(void) {
	PCICR |= (1 << PCIE1);    /* enable Pin-change interrupts 1 (bank C) */
	PCMSK1 |= (1 << PC1);   /* enable specific interrupt for our pin PC1 */
}

ISR(PCINT1_vect) {
	chargeCycleCount++;                             /* count this change */

	CAP_SENSOR_DDR |= (1 << CAP_SENSOR);                  /* output mode */
	_delay_us(1);                                      /* charging delay */

	CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);                /* set as input */
	PCIFR |= (1 << PCIF1);             /* clear the pin-change interrupt */
}


int main(void) {
	// -------- Inits --------- //
	clock_prescale_set(clock_div_1);                       /* full speed */
	initUSART();

	LED_DDR = 0xff;
	MCUCR |= (1 << PUD);                        /* disable all pullups */
	CAP_SENSOR_PORT |= (1 << CAP_SENSOR);		/* we can leave output high */

	initPinChangeInterrupt();

	// ------ Event loop ------ //
	while (1) {
		chargeCycleCount = 0;                   /* reset counter */
		CAP_SENSOR_DDR |= (1 << CAP_SENSOR);    /* start with cap charged */
		sei();									/* start up interrupts, counting */
		_delay_ms(SENSE_TIME);
		cli();                                  /* done */
		if (chargeCycleCount < THRESHOLD)
		{
			runTune();
		}
		return 0;								/* This line is never reached */
	}
}

static void runTune()
{
	for(int noteIdx = 0; noteIdx < SONG_LENGTH; noteIdx++)
	{
		playNote(song[noteIdx], 1600);
	}
}