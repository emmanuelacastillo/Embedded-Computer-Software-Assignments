#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "robotarms.h"
#include "pinDefines.h"
#include "USART.h"

static void turnServoArmsForPickUp(uint16_t pulseLength)
{
	OCR1A = pulseLength;
	DDRB |= (1 << PB1);                        /* re-enable output pin for arm1 */
	DDRB |= (1 << PB2);                        /* re-enable output pin for arm2 */

	_delay_ms(1000);
	while (TCNT1 < 3000) {;}                          /* delay until pulse part of cycle done */
	
	DDRB &= ~(1 << PB1);                         /* disable output pin for arm1 */
	DDRB &= ~(1 << PB2);                         /* disable output pin for arm2 */
}

static void turnServoArmsForGrab(uint16_t pulseLength)
{
	OCR1A = pulseLength;
	DDRB |= (1 << PB3);                        /* re-enable output pin for arm1 */
	DDRB |= (1 << PB4);                        /* re-enable output pin for arm2 */

	_delay_ms(1000);
	while (TCNT1 < 3000) {;}                          /* delay until pulse part of cycle done */
	
	DDRB &= ~(1 << PB3);                         /* disable output pin for arm1 */
	DDRB &= ~(1 << PB4);                         /* disable output pin for arm2 */
}

static inline void initTimer0Servo(void) {
	/* Set up Timer1 (16bit) to give a pulse every 20ms */
	/* Use Fast PWM mode, counter max in ICR1 */
	TCCR0A |= (1 << WGM11);
	TCCR0B |= (1 << WGM12) | (1 << WGM13);
	TCCR0B |= (1 << CS10);						/* /1 prescaling -- counting in microseconds */
	ICR1 = 10000;                               /* TOP value = 10ms */
	TCCR1A |= (1 << COM1A1);					/* Direct output on PB1 / OC1A */
	DDRB |= (1 << PB1);                         /* set pin for output for arm1 */
	DDRB |= (1 << PB2);                         /* set pin for output for arm2 */
}

static inline void initTimer1Servo(void) {
	/* Set up Timer1 (16bit) to give a pulse every 20ms */
	/* Use Fast PWM mode, counter max in ICR1 */
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS10);						/* /1 prescaling -- counting in microseconds */
	ICR1 = 20000;                               /* TOP value = 20ms */
	TCCR1A |= (1 << COM1A1);					/* Direct output on PB1 / OC1A */
	DDRB |= (1 << PB3);                         /* set pin for output for arm1 */
	DDRB |= (1 << PB4);                         /* set pin for output for arm2 */
}

void pickupItem()
{
	initTimer0Servo();
	turnServoArmsForPickUp(PULSE_TO_REACH);
	initTimer1Servo();
	turnServoArmsForGrab(PULSE_TO_GRAB);
}

void dropItem()
{
	initTimer0Servo();
	turnServoArmsForPickUp(PULSE_TO_LOWER);
	initTimer1Servo();
	turnServoArmsForGrab(PULSE_TO_RELEASE);
}
