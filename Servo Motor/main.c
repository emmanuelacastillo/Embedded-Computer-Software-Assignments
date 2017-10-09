#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "USART.h"

#define PULSE_45	700		/* Turns 45 degrees */
#define PULSE_90	1500	/* Turns 90 degrees */
#define PULSE_120	2400	/* Turns 120 degrees */

static inline uint16_t turnServo(void);

static inline void initTimer1Servo(void) {
                   /* Set up Timer1 (16bit) to give a pulse every 20ms */
                             /* Use Fast PWM mode, counter max in ICR1 */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  TCCR1B |= (1 << CS10);  /* /1 prescaling -- counting in microseconds */
  ICR1 = 20000;                                    /* TOP value = 20ms */
  TCCR1A |= (1 << COM1A1);              /* Direct output on PB1 / OC1A */
  DDRB |= (1 << PB1);                            /* set pin for output */
}


int main(void) {

  // -------- Inits --------- //
  initTimer1Servo();
  initUSART();

  // ------ Event loop ------ //
  while (1) {
	turnServo(PULSE_45);
	turnServo(PULSE_90);
	turnServo(PULSE_120);
  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}

static void turnServo(uint16_t pulseLength)
{
	 OCR1A = pulseLength;
	 DDRB |= (1 << PB1);                        /* re-enable output pin */

	 _delay_ms(1000);
	 while (TCNT1 < 3000) {;}                          /* delay until pulse part of cycle done */
		 
	DDRB &= ~(1 << PB1);                         /* disable output pin */
}
