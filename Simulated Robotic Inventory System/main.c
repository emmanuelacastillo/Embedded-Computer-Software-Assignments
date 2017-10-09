#include <avr/io.h>

#include "navigation.h"
#include "barcode.h"
#include "inventory.h"
#include "robotarms.h"

char barcodeData[16];
INVENTORY_ITEM inventoryItem;

ISR(INT0_vect) {                 /* ISR for navigating to item after a scan */
	decode_cat(barcodeData, sizeof(barcodeData));
	getLocation(&barcodeData, &inventoryItem);
	
	if(inventoryItem != NULL)		/* Proceed only if corresponding item info exists for barcode */
	{
		if(*inventoryItem->existInd) /* Proceed only if item exists in warehouse */
		{
			getItem(&inventoryItem);
			dropItem();
			updateInventory(&inventoryItem);
		}
	}
}

int main(void)
{
	 clock_prescale_set(clock_div_1);                  /* CPU clock 8 MHz */

    while (1) 
    {
		/* An interrupt when a barcode is scanned will handle everything. */
    }
}
