#include "inventory.h"

void getLocation(const char *barcode, INVENTORY_ITEM *inventoryRef)
{	
	if(strcmp(barcode, "item1"))
		eeprom_read_block(&ITEM1, &inventoryRef, sizeof(ITEM1));
	else if(strcmp(barcode, "item2"))
		eeprom_read_block(&ITEM2, &inventoryRef, sizeof(ITEM2));
	else if(strcmp(barcode, "item2"))
		eeprom_read_block(&ITEM3, &inventoryRef, sizeof(ITEM3));
	else if(strcmp(barcode, "item3"))
		eeprom_read_block(&ITEM4, &inventoryRef, sizeof(ITEM4));
	else if(strcmp(barcode, "item5"))
		eeprom_read_block(&ITEM5, &inventoryRef, sizeof(ITEM5));
	else if(strcmp(barcode, "item6"))
		eeprom_read_block(&ITEM6, &inventoryRef, sizeof(ITEM6));
	else if(strcmp(barcode, "item7"))
		eeprom_read_block(&ITEM7, &inventoryRef, sizeof(ITEM7));
	else if(strcmp(barcode, "item8"))
		eeprom_read_block(&ITEM8, &inventoryRef, sizeof(ITEM8));
	else if(strcmp(barcode, "item9"))
		eeprom_read_block(&ITEM9, &inventoryRef, sizeof(ITEM9));
	else if(strcmp(barcode, "item10"))
		eeprom_read_block(&ITEM10, &inventoryRef, sizeof(ITEM10));
	else if(strcmp(barcode, "item11"))
		eeprom_read_block(&ITEM11, &inventoryRef, sizeof(ITEM11));
	else if(strcmp(barcode, "item12"))
		eeprom_read_block(&ITEM12, &inventoryRef, sizeof(ITEM12));
	else if(strcmp(barcode, "item13"))
		eeprom_read_block(&ITEM13, &inventoryRef, sizeof(ITEM13));
	else if(strcmp(barcode, "item14"))
		eeprom_read_block(&ITEM14, &inventoryRef, sizeof(ITEM14));
	else if(strcmp(barcode, "item15"))
		eeprom_read_block(&ITEM15, &inventoryRef, sizeof(ITEM15));
	else if(strcmp(barcode, "item16"))
		eeprom_read_block(&ITEM16, &inventoryRef, sizeof(ITEM16));
	else if(strcmp(barcode, "item17"))
		eeprom_read_block(&ITEM17, &inventoryRef, sizeof(ITEM17));
	else if(strcmp(barcode, "item18"))
		eeprom_read_block(&ITEM18, &inventoryRef, sizeof(ITEM18));
	else if(strcmp(barcode, "item19"))
		eeprom_read_block(&ITEM19, &inventoryRef, sizeof(ITEM19));
	else if(strcmp(barcode, "item20"))
		eeprom_read_block(&ITEM20, &inventoryRef, sizeof(ITEM20));
	else if(strcmp(barcode, "item21"))
		eeprom_read_block(&ITEM21, &inventoryRef, sizeof(ITEM21));
	else if(strcmp(barcode, "item22"))
		eeprom_read_block(&ITEM22, &inventoryRef, sizeof(ITEM22));
	else if(strcmp(barcode, "item23"))
		eeprom_read_block(&ITEM23, &inventoryRef, sizeof(ITEM23));
	else if(strcmp(barcode, "item24"))
		eeprom_read_block(&ITEM24, &inventoryRef, sizeof(ITEM24));
	else if(strcmp(barcode, "item25"))
		eeprom_read_block(&ITEM25, &inventoryRef, sizeof(ITEM25));
	else if(strcmp(barcode, "item26"))
		eeprom_read_block(&ITEM26, &inventoryRef, sizeof(ITEM26));
	else if(strcmp(barcode, "item27"))
		eeprom_read_block(&ITEM27, &inventoryRef, sizeof(ITEM27));
	else
		*inventoryRef = NULL;
}

void updateInventory(INVENTORY_ITEM *inventoryRef)
{
	*inventoryRef->existInd = false;
	eeprom_write_block(inventoryRef, &inventoryRef, sizeof(inventoryRef));
}
