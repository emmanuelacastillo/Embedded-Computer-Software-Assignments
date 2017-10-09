#include <avr/io.h>

#define DOT = 0b00000001
#define DASH = 0b00000010

void initSPISlave(void);

void triggerDot();

void triggerDash();

uint8_t receiveMasterData(void);