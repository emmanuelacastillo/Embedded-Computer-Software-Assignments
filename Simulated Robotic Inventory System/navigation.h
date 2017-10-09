#include <inventory.h>

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#define FLOOR_FWD_DELAY 100000;
#define SECTION_FWD_DELAY 5000;
#define TURN_FWD_DELAY 50000;
#define TURN_DELAY 2000;
#define TURN_QUARTER_DELAY (TURN_DELAY/2);
#define STOP_DELAY 1000;

void getItem(INVENTORY_ITEM* itemInfo);

#endif /* NAVIGATION_H_ */
