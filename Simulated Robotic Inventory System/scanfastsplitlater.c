// headers
#include <Mega32.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <delay.h>
#include <string.h>
#include <math.h> 
#include <lcd.h>


#define begin {
#define end }   

// dependent on scan velocity and ADC rate
#define longwidth 10
#define shortwidth 3    

#define delta 50  // assumed difference in a transition, may change to dynamic later
                                          
//***************************//
//   Final Decode Defines    

#define stringlength 7 //for convinience, we'll assume we only ever scanner
                       //barcode string of length 7, with first and last 
                       //characters being *, 2nd to last character will
                       //be the error checking 
#define inven 10       //size of our inventory, assume 10 at the moment

//lcd definition and setup
#define LCDwidth 16

//LCD uses PORTC
#asm
    .equ __lcd_port=0x15
#endasm

//**************************//


typedef unsigned char uchar;

flash char start[] = "\r\nStarting ...\r\n";  
flash char done[] = "\r\nEnd of Printing data\r\n";
flash char minpoints[] = "\r\nPrinting Minimum Array\r\n";
flash char maxpoints[] = "\r\nPrinting Maximum Array\r\n"; 
flash char test1[] = "\r\nData\r\n"; 
flash char widthprint[] = "\r\nUnprocessed Width:\r\n";
flash char widthbiprint[] = "\r\nProcessed Width in binary:\r\n";  
flash char widthstuff[] = "Printing Widthstring\r\n";

//***************************//
// codetbl, inventory code, price, and name
#include "codeandinv.h"
//***************************//


//flash char newline[] = "\r";

int readADC;
uchar Ain;
uchar t_buffer[20]; // output string
uchar t_ready;
uchar t_index;
uchar t_char;
uchar formatindex;    
unsigned char data[700]; // maximally sized, can be increased by decreasing data stack size, stores read ADCH vals
uchar blockwidth[100]; // store the length of each bar
uchar *blockwidthptr;  
uchar currentwidth; // width of current block so far
unsigned char *datalast = data + 699;
unsigned char *dataptr;    
uchar takedata; // tells the timer0 interrupt to put ADCH data in data 
uchar newdata;   

// vars for peakdet algorithm
int mn; // current minimum value
uchar mnpos; // point number of minimum value
int mx; // current maximum  
uchar mxpos; // point number of maximum value  
uchar lookformax; // are we looking for the next max or the next min transition
unsigned int minima[40]; // store indices of the transitions to minima (minima start points)  
unsigned int mintime[40];    
unsigned int *mintimeptr;
unsigned int *minimalastptr = minima + 39;
unsigned int *minimaptr;
unsigned int maxima[40]; // store indices of the transitions to maxima (maxima start points)  
unsigned int maxtime[40];    
unsigned int *maxtimeptr;
unsigned int *maximalastptr = maxima + 39; 
unsigned int *maximaptr;
int currentval; // last read val from ADC                                 
unsigned int pointnum; // count number of points read on this scan   
unsigned int lastpoint;
uchar debugloopcounter; 
uchar debugloopcounter2;

// Variables for transition detection
uchar longaverage;  // keeps average of previous 10 values or so
uchar shortaverage; // keep average of previous 3 (to catch the transition)
// are we looking for a 0-1 or 1-0 transition
// 0 = 0 to 1
// 1 = 1 to 0
uchar lookfor; 


//width code string
unsigned int width[69], widthbinary[69];
                              
void spitData(void);
void spitIndices(void); 
void fmtEndline(unsigned char*); // adds \n\r\0 to end of t_buffer 
void getwidthbinary(void); //fills in width and widthbinary array
void spitwidth(void); //spits out width and widthbinary on hyper trm    

/* finaldecode function listing */

//extract the 69 character array, widthbinary, into 7 separate integers for comparison
//widthstring[7] are rdy for comparison after this        
void widthseparate(void);

//convert a scanned width code character into an ascii, takes
//an unsigned int that contains the character in width code format
unsigned char width2ascii(unsigned int widthchar); 

//convert a scanned character width code into an error value, 
//according to barcode standard, Code 39
unsigned char width2error(unsigned int widthchar);

//extract the actual code from the overall code
void getcode(void);

//returns 1 if the scanned barcode strings passes the code 39 error checking
//prodedure, returns 0 otherwise
unsigned char errorcheck (unsigned char* errorarray);                


// LCD functions //

//initialize the LCD, called once in the beginning before while loop
void preplcd(void);

//clean up the LCD screen, other than the labels, no need to call directly
void wipelcd(void);

//print out scanned product's code, name and price 
void printproduct(unsigned char invenindex);   

// print ascii values in itemcode
void printascii(unsigned char *ascii);
 
// print ready on lcd
void printready(void);

//print out error on to the LCD screen, use when appropriate
void printerror(void); 


//**********************************************************
//timer 0 compare ISR // executes every 1 ms
interrupt [TIM0_COMP] void timer0_compare(void)
begin    
  *dataptr = ADCH; 
  currentval = *dataptr;  // currentval: int  dataptr: uchar  

  if (takedata & (dataptr < datalast) )
  begin
    //*dataptr = ADCH;
    dataptr++;
  end    
  
  if (takedata & (maximaptr < maximalastptr) & (minimaptr < minimalastptr) )  // don't overrun memory
  begin  
    //putsf(test1);
    pointnum++;  // we are adding a new point
    // currentval already updated above  
    if (currentval > mx)
    begin
      mx = currentval;
      //mxpos = pointnum;
    end    
    if (currentval < mn)
    begin
      mn = currentval;
      //mnpos = pointnum;
    end     
    if (lookformax) // if looking for 0-1 transition
    begin
      if (currentval < mx - delta)
      begin
        *minimaptr = (pointnum - lastpoint);   
        *mintimeptr = pointnum;
        mintimeptr++;
        lastpoint = pointnum;
        minimaptr++;  
        mn = currentval;
        lookformax = 0; 
        debugloopcounter++;
      end
    end
    else //(!lookformax)
    begin
      if (currentval > mn + delta)
      begin
        *maximaptr = (pointnum - lastpoint); 
        *maxtimeptr = pointnum;
        maxtimeptr++;
        lastpoint = pointnum;        
        maximaptr++;
        mx = currentval; 
        lookformax = 1;
        debugloopcounter2++;
      end
    end // end if !lookformax
  end // end if takedata
end  

// Uart transmit complete interrupt, could be used to
// start spitting out next ADC conversion
interrupt [USART_TXC] void uart_send_complete(void)
begin

end

/**********************************************************/
//UART xmit-empty ISR
interrupt [USART_DRE] void uart_send(void)
begin
	t_char = t_buffer[++t_index];
	if (t_char == 0)	
	begin   
		UCSRB.5=0; //kill isr 
		t_ready=1; //transmit done 
	end

	else	UDR = t_char ;     //send the char 
end

/**********************************************************/

//********************************************************** 
//  -- nonblocking print: initializes ISR-driven
// transmit. This routine merely sets up the ISR, then
//send one character, The ISR does all the work.   
void puts_int(void) 
begin
  t_ready=0;
  t_index=0;
  if (t_buffer[0]>0) 
  begin
  	putchar(t_buffer[0]);
  	UCSRB.5=1;  // turn on UDR empty interrupt
  end 
end

void main(void)
begin  
  unsigned char loopi,inveni, characteri;  
  unsigned char codematch=0;
  char full = 0;
  dataptr = data;
  takedata = 0; // variable to tell timer0 interrupt when to write new data
  
  DDRD = 0x80;
  PORTD.7 = 0; // turn protoboard led on

  UCSRB = 0x08; // Enable Tx only
  UBRRL = 103;  // 9600 Baud
  
  TIMSK = 2;		    //turn on timer 0 cmp match ISR 
  //OCR0 = 249;  	        //set the compare register to 250 time ticks - 1ms
  OCR0 = 125;
  TCCR0 = 0b00001011;	//prescalar to 64 and turn on clear-on-match    

  ADMUX = 0b01100000;  // AVCC (with capacitor), Left Adjust result, ADC0
  //enable ADC and set prescaler to 1/128*16MHz=125,000
  //and clear interupt enable
  //and start a conversion
  ADCSR = 0b11000111;      
  SFIOR = 0b00000000; // set to free running mode

  #asm
  	sei
  #endasm
  
  formatindex = 0;//edit 420
  
  putsf(start); // print startup string
  
  delay_ms(500);

  while (ADCSR.6) {} // wait for first conversion to finish
  readADC = ADCH;
  ADCSR.5 = 1; // auto trigger the ADC
  ADCSR.6 = 1; // start a new conversion so that it free runs
  
  itoa(readADC, t_buffer); // convert ADCH to a string
  fmtEndline(t_buffer);
  formatindex = 0;

  puts_int(); // print first ADC value
          
  //while(1) {}
  DDRB = 0x00; // PORTB input 
  PORTB = 0xFF;   // pull-up
  
  dataptr = data;  
  blockwidthptr = blockwidth; 
  currentwidth = 0;   
  lookfor = 1; // looking for 1-0 transition (white -> black)
  longaverage = 0;
  shortaverage = 0;
  full = 0;  
  
  preplcd();
  wipelcd();
  printerror();
  wipelcd();
  printproduct(9);  
  delay_ms(3000);
  printready();
  
  while(1)
  begin     
    char tempi, tempi2;   
    unsigned int tempint;   
    unsigned char tempstring;
    if (PINB.0 == 0)
    begin    
      if (!takedata)
      begin
      for (tempi = 0; tempi < 40; tempi ++)
      begin
        minima[tempi] = 0; 
        maxima[tempi] = 0;
      end
      end
//       mn = 1000;
//       mx = -1000;
//       lookformax = 1; //edit 426, ignore the first minimum, the rest is good
//       pointnum = 0;
//       lastpoint = 0; // remember the previous index of transition
//       maximaptr = maxima;
//       minimaptr = minima; 
//       maxtimeptr = maxtime;
//       mintimeptr = mintime;
//       mxpos = 0;
//       mnpos = 0;
//       debugloopcounter = 0;  
//       debugloopcounter2 = 0;
//       dataptr = data;  
             
      takedata = 1; // interrupt should call interpret function also
      newdata = 1; 
    end        
    else {takedata = 0;}
    if (PINB.2 == 0) // initialize routine, and spit the last taken data
    begin  
      widthstring[0] = 0xFFFF;

      delay_ms(500); 

      //spitData(); 
      //spitIndices();
      // the below is useful info from spitindices
      sprintf(t_buffer, "numpoints: %d\r\n", pointnum); 
      puts_int();
      delay_ms(500);
      delay_ms(500);
      sprintf(t_buffer, "debuglp: %d\r\n", debugloopcounter);
      puts_int();  
      delay_ms(500);   
      sprintf(t_buffer, "debuglp2: %d\r\n", debugloopcounter2);
      puts_int();  
      delay_ms(500);  
      getwidthbinary();
      //spitwidth();  
      //putsf(done);  
      
      
      
      ////
      wipelcd();  // clean up lcd for this barcode
      
      // Decode and print barcode //
      
      // guarantee widthbinary is natex
      //widthbinary[0] = 0; widthbinary[1] = 1; widthbinary[2] = 0; widthbinary[3] = 0; widthbinary[4] = 1; widthbinary[5] = 0; widthbinary[6] = 1; widthbinary[7] = 0; widthbinary[8] = 0; widthbinary[9] = 0; widthbinary[10] = 0; widthbinary[11] = 0; widthbinary[12] = 0; widthbinary[13] = 0; widthbinary[14] = 1; widthbinary[15] = 0; widthbinary[16] = 0; widthbinary[17] = 1; widthbinary[18] = 1; widthbinary[19] = 0; widthbinary[20] = 1; widthbinary[21] = 0; widthbinary[22] = 0; widthbinary[23] = 0; widthbinary[24] = 0; widthbinary[25] = 1; widthbinary[26] = 0; widthbinary[27] = 0; widthbinary[28] = 1; widthbinary[29] = 0; widthbinary[30] = 0; widthbinary[31] = 0; widthbinary[32] = 0; widthbinary[33] = 0; widthbinary[34] = 1; widthbinary[35] = 0; widthbinary[36] = 1; widthbinary[37] = 1; widthbinary[38] = 0; widthbinary[39] = 0; widthbinary[40] = 1; widthbinary[41] = 0; widthbinary[42] = 0; widthbinary[43] = 0; widthbinary[44] = 1; widthbinary[45] = 1; widthbinary[46] = 0; widthbinary[47] = 0; widthbinary[48] = 0; widthbinary[49] = 0; widthbinary[50] = 0; widthbinary[51] = 1; widthbinary[52] = 0; widthbinary[53] = 0; widthbinary[54] = 1; widthbinary[55] = 0; widthbinary[56] = 0; widthbinary[57] = 0; widthbinary[58] = 1; widthbinary[59] = 0; widthbinary[60] = 0; widthbinary[61] = 1; widthbinary[62] = 0; widthbinary[63] = 0; widthbinary[64] = 1; widthbinary[65] = 0; widthbinary[66] = 1; widthbinary[67] = 0; widthbinary[68] = 0; 
      
      spitwidth();
      putsf(done);
      
      widthseparate(); // takes widthbinary and converts to widthstring[7] 
      
      
      putsf(widthstuff);
      for (tempi2 = 0; tempi2 < 7; tempi2++)
      begin
      
        tempint = widthstring[tempi2]; 
        sprintf(t_buffer, "widthstr[%i]: ", tempi2);
        puts(t_buffer);
      
        for (tempi = 0; tempi < 16; tempi++)
        begin        
          if (tempint & 0x8000)
          begin
            sprintf(t_buffer, "1");
          end      
          else
          begin
            sprintf(t_buffer, "0", tempint & 0x80);
          end
          puts_int();
          while (!t_ready) {};
          tempint = tempint << 1;
        end
        sprintf(t_buffer, "\r\n");
        puts_int();    
        while (!t_ready) {};
      end  
      
      /*debug
      widthstring[0] = 0b0000000010010100; //*
      widthstring[1] = 0b0000000000010011; //N
      widthstring[2] = 0b0000000100001001; //A
      widthstring[3] = 0b0000000000010110; //T
      widthstring[4] = 0b0000000100011000; //E
      widthstring[5] = 0b0000000010010001; //X
      widthstring[6] = 0b0000000010010100; //*
      */
      putsf(widthstuff);
      for (tempi2 = 0; tempi2 < 7; tempi2++)
      begin
      
        tempint = widthstring[tempi2]; 
        sprintf(t_buffer, "widthstr[%i]: ", tempi2);
        puts(t_buffer);
      
        for (tempi = 0; tempi < 16; tempi++)
        begin        
          if (tempint & 0x8000)
          begin
            sprintf(t_buffer, "1");
          end      
          else
          begin
            sprintf(t_buffer, "0", tempint & 0x80);
          end
          puts_int();
          while (!t_ready) {};
          tempint = tempint << 1;
        end
        sprintf(t_buffer, "\r\n");
        puts_int();    
        while (!t_ready) {};
      end
      
      for (loopi = 0; loopi < 7; loopi++)
      begin
        totalcode[loopi] = width2ascii(widthstring[loopi]); // totalcode contains ascii conversions  
        errorvalue[loopi] = width2error(widthstring[loopi]);
      end
      // only continue if we have no errors, otherwise, display error
      if (! errorcheck(errorvalue) ) // if there is an error
      begin
        printerror();
        delay_ms(3000);
        printready();
      end
      else // no error
      begin 
        //printproduct(3);
        //delay_ms(3000);
        
        itemindex = 0xFF; // invalid so far
        getcode(); // store the 4 chars in itemcode   
        // find invindex  
        
        /*debug
        itemcode[0] = 'N';
        itemcode[1] = 'A';
        itemcode[2] = 'T';
        itemcode[3] = 'E';
         */
        for(inveni=0; inveni<inven;  inveni++)
        begin
          for(characteri=0; characteri<stringlength-3; characteri++)
          begin   
            if(itemcode[characteri]==inventory_code[inveni][characteri]) codematch++;
          end
          if (codematch==4) //found a match
          begin
            itemindex = inveni;
          end
          codematch=0;
          if (itemindex != 0xFF) break; // if we found a match, stop
        end 
        //itemindex = 7;
        if(itemindex<10) printproduct(itemindex);
        else  // the valid code found was ok
        begin
          printerror();  
        end
        
        delay_ms(5000);
        printready();
        
        
      end // end else no error found in ascii
      
      
      
      mn = 1000;
      mx = -1000;
      lookformax = 1; //edit 426, ignore the first minimum, the rest is good
      pointnum = 0;
      lastpoint = 0; // remember the previous index of transition
      maximaptr = maxima;
      minimaptr = minima; 
      maxtimeptr = maxtime;
      mintimeptr = mintime;
      mxpos = 0;
      mnpos = 0;
      debugloopcounter = 0;  
      debugloopcounter2 = 0;
      dataptr = data;  
         
    end  // end if PINB.0   
    
    if (PINB.3 == 0)
    begin
    
    end
                    

  end // end while(1)
end 

/*           
unsigned int minima[30]; // store indices of the transitions to minima (minima start points)
unsigned int *minimaptr;
unsigned int maxima[30]; // store indices of the transitions to maxima (maxima start points)  
unsigned int *maximaptr; 
*/
// minimaptr, maximaptr point 1 beyond last valid transition size
void spitIndices(void)
begin
  unsigned int *pmin;  
  unsigned int *pmax;   
  unsigned int *pmintime;
  unsigned int *pmaxtime;
  pmin = minima;
  pmax = maxima;  
  pmintime = mintime;
  pmaxtime = maxtime;    
  sprintf(t_buffer, "pmin %d\r\n", *pmin);     
  puts_int();
  delay_ms(500);            
  *minimaptr = lastpoint;
  sprintf(t_buffer, "minmaptr %d\r\n", *minimaptr);     
  puts_int();
  delay_ms(500);
  sprintf(t_buffer, "numpoints: %d\r\n", pointnum); 
  puts_int();
  delay_ms(500);
  sprintf(t_buffer, "debuglp: %d\r\n", debugloopcounter);
  puts_int();  
  delay_ms(500);   
  sprintf(t_buffer, "debuglp2: %d\r\n", debugloopcounter2);
  puts_int();  
  delay_ms(500);  
  putsf(minpoints); 
  while (pmin < minimaptr)
  begin               
    //sprintf(t_buffer, "%d\r\n", *pmin);
    //puts_int();
    //delay_ms(1000);
    itoa(*pmin, t_buffer);
    //fmtEndline(t_buffer);
    puts_int();
    while (!t_ready) {};
    pmin++;
    
    sprintf(t_buffer, " time: %d\r\n", *pmintime);
    puts_int();
    while (!t_ready) {};
    pmintime++;
  end  
  putsf(maxpoints);
  while (pmax < maximaptr)
  begin
    itoa(*pmax, t_buffer);
    //fmtEndline(t_buffer);
    puts_int();
    while (!t_ready) {};
    pmax++;  
    
    sprintf(t_buffer, " time: %d\r\n", *pmaxtime);
    puts_int();
    while (!t_ready) {};
    pmaxtime++;
  end
end

// dataptr points to one beyond last value           
void spitData(void)
begin
  char *loopdata;  
  putsf(test1);
  loopdata = data;
  while (loopdata < dataptr)
  begin
    itoa(*loopdata, t_buffer); // convert array val to a string 
    fmtEndline(t_buffer);   
    puts_int(); 
    while (!t_ready) {}; // wait for transmit to complete
    loopdata++;
  end         
  dataptr = data; // put dataptr back to beginning of array
end  

void fmtEndline(unsigned char *array)
begin      
    formatindex = 0;
    while (array[formatindex] != 0) formatindex++; //edit 420
    array[formatindex] = '\n';         //edit 420
    array[formatindex+1] = '\r';       //edit 420
    array[formatindex+2] = 0;       //edit 420       
end
   
 
// this function should attempt to make sense of the data  
// at first it will update its variables only depending on the current input
// it should be called once per input
// *dataptr should point to last updated value
void interpret(void)
begin
  longaverage = 0;
end 




//fills in the width, widthbinary array, call after all scanning is done in main
void getwidthbinary(void)
begin 
  unsigned int *wmin;
  unsigned int *wmax;
  unsigned char i;
  unsigned int minreference; // min thickness 
  unsigned int maxreference; // max thickness value
  unsigned int threshold; // average of last max and min
  //float threshold;
  unsigned int dynamic; 
  wmin = minima;
  wmax = maxima;
  width[0] = *wmax;
  //merge maxima and minima array into one array, ignoring the first minima pt
  //(initial white space to first black stripe)        
  // WIDTH: contains actual width
  for(i=1; i<69; i=i+2)
  begin
    wmax++;
    wmin++;
    width[i] = *wmin;
    width[i+1] = *wmax;
  end
  //since the first width binary for "*" is thin, we use it as a reference
  minreference = width[0];
  maxreference = width[1];
  threshold = (minreference+maxreference)>>1;
  //threshold = (.66*(float)minreference + .33*(float)maxreference)/2; // threshold is average of last min,max     
  sprintf(t_buffer, "minref: %i\r\n", minreference);
  puts_int();
  while (!t_ready) {};
  widthbinary[0] = 0; //first stripe is always thin (black)\
  dynamic = 2.0;
  for (i=1; i<69; i++)
  begin
    //if the thickness is greater than twice the thin width, we got a thick one  
    if (width[i] > (threshold) ) 
    begin
      widthbinary[i] = 1;  
      maxreference = width[i];  
      //threshold = (.66*(float)minreference + .33*(float)maxreference)/2; // threshold is average of last min,max   
      threshold = (minreference+maxreference)>>1;
    end
    else 
    begin
      widthbinary[i] = 0; //else we got a thin one 
      minreference =  width[i]; // update minreference to be this thin 
      //threshold = (.66*(float)minreference + .33*(float)maxreference)/2; // threshold is average of last min,max
      threshold = (minreference+maxreference)>>1;
    end   
/*    
  for (i=1; i<69; i++)
  begin
    //if the thickness is greater than twice the thin width, we got a thick one
    //if (width[i] > (dynamic* width[i-1])) widthbinary[i] = 1;    
    if (width[i] > (dynamic*minreference)) widthbinary[i] = 1;
    else 
    begin
      widthbinary[i] = 0; //else we got a thin one 
      minreference =  width[i]; // update minreference to be this thin bar
    end    
*/    
    /* // old stuff
    //if (widthbinary[i] = 1)//if the current value is thick
    //begin
    //  dynamic = width[i]/width[i-1];
    //  minreference = width[i]/dynamic; //
    //end 
    //if (widthbinary[i] = 0)//if the current value is thin
    //minreference = width[i];
    */ 
  end
end

void spitwidth(void)
begin
  unsigned char i;
  putsf(widthprint);
  for (i=0; i<69; i++)
  begin   
    sprintf(t_buffer, "%i: ", i);    
    puts_int();
    while (!t_ready) {};    
    itoa(width[i], t_buffer);
    fmtEndline(t_buffer);
    puts_int();
    while (!t_ready) {}; 
  end 
  putsf(widthbiprint);
  for (i=0; i<69; i++)
  begin
    sprintf(t_buffer, "%i: ", i);    
    puts_int();
    while (!t_ready) {}; 
    itoa(widthbinary[i], t_buffer);
    fmtEndline(t_buffer);
    puts_int();
    while (!t_ready) {}; 
  end 
end
  
#include "finaldecode.c"

/*

    // the below if statement could be done with a TX complete interrupt
    if (t_ready) // if we are done sending the last ADC conversion
    begin
      formatindex = 0; //reset formatindex for each conversion edit 420
      readADC = ADCH;
      ADCSR.6 = 1; // start new conversion
      itoa(readADC, t_buffer); // convert ADCH to a string
      while (t_buffer[formatindex] != 0) formatindex++;   //edit 420
      t_buffer[formatindex] = "\n";                       //edit 420
      t_buffer[formatindex+1] = "\r";                     //edit 420
      t_buffer[formatindex+2] = "\0";                     //edit 420
      // print a newline
      puts_int(); // print ADC value
      //t_buffer = "\r\n";
//      t_buffer[0] = '\r';
//      t_buffer[1] = '\n';
//      t_buffer[2] = 0;
//      puts_int(); // print newline   
       //putsf(newline); edit 420, see UART ISR
    //delay_ms(1000);
    end  // end if t_ready    
    */