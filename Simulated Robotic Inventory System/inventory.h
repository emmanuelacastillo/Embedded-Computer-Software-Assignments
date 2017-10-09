/*
 * Total EEPROM Memory Usage: 702 Bytes out of 1KB 
 */ 

#ifndef INVENTORY_H_
#define INVENTORY_H_

typedef struct {
	uint8_t  floor;			  // Floor in warehouse (3 floors)
	uint8_t  row;			  // Row in floor (3 rows per floor)
	uint8_t  column;		  // Column in floor (3 columns per floor)
	bool  existInd;		      // Determines if it exist in the warehouse
} INVENTORY_ITEM;

INVENTORY_ITEM EEMEM ITEM1 = { 1, 1, 1, true };
	
INVENTORY_ITEM EEMEM ITEM2 = { 1, 1, 2, true };
	
INVENTORY_ITEM EEMEM ITEM3 = { 1, 1, 3, true };
	
INVENTORY_ITEM EEMEM ITEM4 = { 1, 2, 1, false };
	
INVENTORY_ITEM EEMEM ITEM5 = { 1, 2, 2, false };
	
INVENTORY_ITEM EEMEM ITEM6 = { 1, 2, 3, true };
	
INVENTORY_ITEM EEMEM ITEM7 = { 1, 3, 1, true };
	
INVENTORY_ITEM EEMEM ITEM8 = { 1, 3, 2, true };
	
INVENTORY_ITEM EEMEM ITEM9 = { 1, 3, 3, true };
	
INVENTORY_ITEM EEMEM ITEM10 = { 2, 1, 1, false };

INVENTORY_ITEM EEMEM ITEM11 = { 2, 1, 2, true };

INVENTORY_ITEM EEMEM ITEM12 = { 2, 1, 3, false };

INVENTORY_ITEM EEMEM ITEM13 = { 2, 2, 1, true };

INVENTORY_ITEM EEMEM ITEM14 = { 2, 2, 2, false };

INVENTORY_ITEM EEMEM ITEM15 = { 2, 2, 3, true };

INVENTORY_ITEM EEMEM ITEM16 = { 2, 3, 1, true };

INVENTORY_ITEM EEMEM ITEM17 = { 2, 3, 2, true };

INVENTORY_ITEM EEMEM ITEM18 = { 2, 3, 3, true };
	
INVENTORY_ITEM EEMEM ITEM19 = { 3, 1, 1, false };

INVENTORY_ITEM EEMEM ITEM20 = { 3, 1, 2, false };

INVENTORY_ITEM EEMEM ITEM21 = { 3, 1, 3, true };

INVENTORY_ITEM EEMEM ITEM22 = { 3, 2, 1, true };

INVENTORY_ITEM EEMEM ITEM23 = { 3, 2, 2, true };

INVENTORY_ITEM EEMEM ITEM24 = { 3, 2, 3, true };

INVENTORY_ITEM EEMEM ITEM25 = { 3, 3, 1, true };

INVENTORY_ITEM EEMEM ITEM26 = { 3, 3, 2, true };

INVENTORY_ITEM EEMEM ITEM27 = { 3, 3, 3, false };
	
void getLocation(const char *barcode, INVENTORY_ITEM *inventoryRef);
void updateInventory(INVENTORY_ITEM *inventoryRef);

#endif /* INVENTORY_H_ */
