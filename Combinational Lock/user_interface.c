#include <avr/io.h>
#include <inttypes.h>

#include "lcd.h"
#include "keypad.h"
#include "user_interface.h"

uint16_t getEnteredCode(char *msg)
{
	uint8_t keyCode,x,pos;
	uint8_t prev_key=0xFF;
	
	while(1)
	{
		x=0;
		pos=6;
	
		uint8_t key[]="####";
		uint8_t system_reset=0;

    	while(!system_reset)
    	{
			keyCode=GetKeyPressed(); 							//Get the keycode of pressed key

			if(keyCode==prev_key)
				continue;
			else if(keyCode==0xFF)
			{
				prev_key=0xFF;
				continue;
			}
			else
			{
				prev_key=keyCode;
			}
				

			if(keyCode!=0xFF)
			{
				LCDBacklightOn();
			}

			_delay_ms(200); 									//key debouncing delay

			if(pos==10)
			{
				//Wait for enter key or reset key
				while(1)
				{
					keyCode=GetKeyPressed();
					if(keyCode==4)
					{
						//Enter key
						break;
					}
				
				}
				//Convert pass to a single integer
				return(((1000*key[0])+(100*key[1])+(10*key[2])+key[3]));		//save the password in 16 bit integer
			}
		}
	}
}
