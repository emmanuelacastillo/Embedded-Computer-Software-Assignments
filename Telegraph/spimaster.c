#include "spimaster.h"

void initSPI(void) {
  SPI_SS_DDR |= (1 << SPI_SS);                        /* set SS output */
  SPI_SS_PORT |= (1 << SPI_SS);       /* start off not selected (high) */

  SPI_MOSI_DDR |= (1 << SPI_MOSI);                   /* output on MOSI */
  SPI_MISO_PORT |= (1 << SPI_MISO);                  /* pullup on MISO */
  SPI_SCK_DDR |= (1 << SPI_SCK);                      /* output on SCK */

  /* Don't have to set phase, polarity b/c
   * default works with 25LCxxx chips */
  SPCR |= (1 << SPR1);                /* div 16, safer for breadboards */
  SPCR |= (1 << MSTR);                                  /* clockmaster */
  SPCR |= (1 << SPE);                                        /* enable */
}

void SPI_tradeByte(uint8_t byte) {
  SPDR = byte;                       /* SPI starts sending immediately */
  loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
                                /* SPDR now contains the received byte */
}

uint8_t readStatus(void) {
  SLAVE_SELECT;
  SPI_tradeByte(RDSR);
  SPI_tradeByte(0);                            /* clock out eight bits */
  SLAVE_DESELECT;
  return (SPDR);                                  /* return the result */
}

void writeEnable(void) {
  SLAVE_SELECT;
  SPI_tradeByte(WREN);
  SLAVE_DESELECT;
}

void writeByte(uint8_t byte) {
  writeEnable();
  SLAVE_SELECT;
  SPI_tradeByte(WRITE);
  SPI_tradeByte(byte);
  SLAVE_DESELECT;
  while (readStatus() & (1 << WRITE_IN_PROGRESS)) {;
  }
}
