#include <io.h>
#include <stdlib.h>
#include <string-avr.h>
#include <progmem.h>

#include <io.h>
#include <stdlib.h>
#include <string-avr.h>
#include <progmem.h>

#include "barcode.h"
#include "stddefs.h"

char codetable[] = "n/o//defghijklm///////JKLMNOPQRSTUVWXYZ[\\]^_'abc//////0123456789:;<=>?@ABCDEFGHI";

// advance 4 chars at a time through encoded sequence,
// produce base64-decoded output
char *decode_cat(char *buf, int length)
{
	CHARU 	i,j,k,l,x;
	CHARU	b[4];
	CHARU ret[MAX_CODE_LEN];
	CHARU txt[MAX_CODE_LEN];

	if (length > 0)
	{
		memset(ret,0,MAX_CODE_LEN);

		// copy source string into zeros-padded area
		memset(txt,0,MAX_CODE_LEN);
		memcpy(txt,buf,length);
		length=strlen(txt);
		l=4-((length)%4);
		j=0;
		k=0;

		while(1)
		{
			memset(b,0,4);
			x=0;
			for(i=0; i<4; i++)
			{
				x++;
				if (x>length)
				{
					//putchar('&');   //debug
					return NULL;
				};
				
				b[i] = PRG_RDB(&codetable[txt[j+i]] - 43) - 48;
				
			}
			ret[k] =   (((b[0] << 2) | ((b[1]>>4) & 0x03))^67) - 32;
			ret[k+1] = (((b[1] << 4) | ((b[2]>>2) & 0x0F))^67) - 32;
			ret[k+2] = (((b[2] << 6) | (b[3] & 0x3F))^67) - 32;
			
			j+=4;
			k+=3;

			if(j >= length)
			{
				if (l!=4)
				{
					ret[strlen(ret)-l]=0;
				}

				if (txt !=NULL)
				{
					memcpy(txt,NULL,0);
				}
				return ret;
			}
		}
	}
	else
	{
		return NULL;
	}
}
