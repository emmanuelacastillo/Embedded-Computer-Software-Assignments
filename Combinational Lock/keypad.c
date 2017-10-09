#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "keypad.h"

#define KEYPAD_PORT PORT(KEYPAD)
#define KEYPAD_DDR   DDR(KEYPAD)
#define KEYPAD_PIN   PIN(KEYPAD)

uint8_t GetKeyPressed()
{
      uint8_t r,c;

      KEYPAD_PORT|= 0X0F;
	  

      for(c=0;c<4;c++)
      {
         KEYPAD_DDR&=~(0XFF);

         KEYPAD_DDR|=(0X80>>c);
         for(r=0;r<4;r++)
         {
            if(!(KEYPAD_PIN & (0X08>>r)))
            {
               return (r*4+c);
            }
         }
      }
	  return 0XFF;//Indicate No key pressed
}

uint16_t getEnteredPassword(char *msg)
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

