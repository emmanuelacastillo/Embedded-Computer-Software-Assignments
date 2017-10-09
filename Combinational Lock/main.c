// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "keypad.h"

uint16_t passWord = 12345;

static inline void setBridgeState(uint8_t bridgeA, uint8_t bridgeB) {
	/* Utility function that lights LEDs when it energizes a bridge side */
	if (bridgeA) {
		PORTD |= (1 << PD6);
	}
	else {
		PORTD &= ~(1 << PD6);
	}
	if (bridgeB) {
		PORTD |= (1 << PD5);
	}
	else {
		PORTD &= ~(1 << PD5);
	}
}

int main(void) {
	// -------- Inits --------- //
	uint16_t enteredPassword;
	DDRD |= (1 << PD6);               /* now hooked up to bridge, input1 */
	DDRD |= (1 << PD5);               /* now hooked up to bridge, input2 */
	
	while (1)
	{
		setBridgeState(1, 0);                                 /* "forward" */
		_delay_ms(2000);
		
		enteredPassword = getEnteredPassword();
		
		if(enteredPassword == passWord)
		{
			setBridgeState(0, 0);                      /* both low stops motor */
			_delay_ms(2000);
		}
	}
	/* End event loop */
	return 0;
}
