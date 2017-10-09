#ifndef ROBOTARMS_H_
#define ROBOTARMS_H_

#define PULSE_TO_REACH		1500	/* Pulse to move arms to item height */
#define PULSE_TO_GRAB		750		/* Pulse to move arms together to grab item */
#define PULSE_TO_LOWER		-1500	/* Pulse to lower item to drop */
#define PULSE_TO_RELEASE	-750	/* Pulse to move arms apart to drop item */

void pickupItem();
void dropItem();

#endif /* ROBOTARMS_H_ */