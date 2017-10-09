#include <avr/io.h>
#include <util/delay.h>
#include <pinDefines.h>

#include "robotarms.h"
#include "navigation.h"

static inline void driveToFloor(uint8_t *floor)
{
	while(floor)
	{
		PORTD |= (1 << PD6);			/* Drive foward */
		_delay_ms(FLOOR_FWD_DELAY);
		PORTD &= ~(1 << PD6);			/* Stops */
		PORTD &= ~(1 << PD5);			/* Stops */
		_delay_ms(STOP_DELAY);
		PORTD |= (1 << PD6)				/* Turn on corner */
		PORTD |= (1 << PD5)
		_delay_ms(TURN_QUARTER_DELAY);	
		PORTD |= (1 << PD6);			/* Drive foward */
		_delay_ms(TURN_FWD_DELAY);
		PORTD &= ~(1 << PD6);			/* Stops */
		PORTD &= ~(1 << PD5);			/* Stops */
		_delay_ms(STOP_DELAY);
		PORTD |= (1 << PD6)				/* Turn on corner */
		PORTD |= (1 << PD5)
		_delay_ms(TURN_QUARTER_DELAY);	
		floor++;
	}
}

static inline void driveToSection(uint8_t *column, uint8_t *row)
{
	while(column)
	{
		PORTD |= (1 << PD6);			/* Drive foward */
		_delay_ms(SECTION_FWD_DELAY);
		column++;
	}
	PORTD |= (1 << PD6)				/* Turn on corner */
	PORTD |= (1 << PD5)
	_delay_ms(TURN_QUARTER_DELAY);
	pickupItem();
}

static inline void turnAround()
{
	PORTD |= (1 << PD6)				/* Turn to go back to start location */
	PORTD |= (1 << PD5)
	_delay_ms(TURN_DELAY);
}

static inline void driveHome(uint8_t *floor, uint8_t *row)
{
	turnAround();
	driveToSection(row)
	driveToFloor(floor);
}

void getItem(INVENTORY_ITEM *itemInfo)
{
	DDRD |= (1 << PD6);               /* now hooked up to bridge, input1 */
	DDRD |= (1 << PD5);               /* now hooked up to bridge, input2 */
	
	driveToFloor(itemInfo->floor);
	driveToSection(itemInfo->row);
	pickupItem();
	driveHome(itemInfo->floor, itemInfo->row);
}
