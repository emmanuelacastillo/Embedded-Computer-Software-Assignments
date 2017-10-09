#include <avr/io.h>
#include "pinDefines.h"


/* Sets 2nd AVR as slave */
#define SLAVE_SELECT    SPI_SS_PORT &= ~(1 << SPI_SS)
#define SLAVE_DESELECT  SPI_SS_PORT |= (1 << SPI_SS)

#define WRITE     0b00000010                 /* write to memory */
#define WREN      0b00000110                    /* write enable */
#define RDSR      0b00000101            /* read status register */

#define WRITE_IN_PROGRESS    0

/* Init SPI to run EEPvROM with phase, polarity = 0,0 */
void initSPI(void);

/* Generic.  Just loads up HW SPI register and waits */
void SPI_tradeByte(uint8_t byte);

/* reads the EEPROM status register */
uint8_t readStatus(void);

 /* helper: sets EEPROM write enable */
void writeEnable(void);

/* writes a byte to a given memory location */
void writeByte(uint8_t byte);
