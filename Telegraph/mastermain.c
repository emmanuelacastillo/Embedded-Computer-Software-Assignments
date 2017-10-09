                                            /* SPI EEPROM 25LC256 Demo */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

#include "pinDefines.h"

#include "USART.h"
#include "spimaster.h"

#define DOT = 0b00000001
#define DASH = 0b00000010

int main(void) {
  uint8_t i;
  uint8_t address;

  initSPI();
  initUSART();

  /* Send data to slave repetitively */
  while (1) 
  {
    writeByte(DOT);
	writeByte(DASH);
	_delay_ms(100);
  }                                               
  return 0;                           
}
