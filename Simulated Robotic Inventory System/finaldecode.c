


//extract the 69 character array, widthbinary, into 7 separate integers for comparison
//widthstring[7] are rdy for comparison after this
void widthseparate(void)
begin
  unsigned char i;//temporary indices
     
  // widthbinary is type unsigned int array   
  // widthstring is type unsigned int array
  
  //clean the crap up in each integer, just in case
  for(i=0; i<7; i++)
  begin
    widthstring[i] = 0;
  end
   
  //I am sure there is a better way to do this(ie: one more outer loop)
  //but I cant think of anything right now
  for(i=0; i<9; i++)
  begin
    widthstring[0] = widthstring[0] | (widthbinary[8-i]<<i);
  end 
    
  
  for(i=0; i<9; i++)
  begin
    widthstring[1] = widthstring[1] | (widthbinary[18-i]<<i);
  end
  
  for(i=0; i<9; i++)
  begin
    widthstring[2] = widthstring[2] | (widthbinary[28-i]<<i);
  end
  
  for(i=0; i<9; i++)
  begin
    widthstring[3] = widthstring[3] | (widthbinary[38-i]<<i);
  end
  
  for(i=0; i<9; i++)
  begin
    widthstring[4] = widthstring[4] | (widthbinary[48-i]<<i);
  end
  
  for(i=0; i<9; i++)
  begin
    widthstring[5] = widthstring[5] | (widthbinary[58-i]<<i);
  end
  
  for(i=0; i<9; i++)
  begin
    widthstring[6] = widthstring[6] | (widthbinary[68-i]<<i);
  end 

end
  


  
  
      
  /* sample barcode character extraction
  widthstring[0] = widthstring[0] | widthbinary[0]<<0;
  widthstring[0] = widthstring[0] | widthbinary[1]<<1;
  widthstring[0] = widthstring[0] | widthbinary[2]<<2;
  widthstring[0] = widthstring[0] | widthbinary[3]<<3;
  widthstring[0] = widthstring[0] | widthbinary[4]<<4;
  widthstring[0] = widthstring[0] | widthbinary[5]<<5;
  widthstring[0] = widthstring[0] | widthbinary[6]<<6;
  widthstring[0] = widthstring[0] | widthbinary[7]<<7;
  widthstring[0] = widthstring[0] | widthbinary[8]<<8; 
  
  widthstring[1] = widthstring[1] | widthbinary[10]<<0;
  widthstring[1] = widthstring[1] | widthbinary[11]<<1;
  widthstring[1] = widthstring[1] | widthbinary[12]<<2;
  widthstring[1] = widthstring[1] | widthbinary[13]<<3;
  widthstring[1] = widthstring[1] | widthbinary[14]<<4;
  widthstring[1] = widthstring[1] | widthbinary[15]<<5;
  widthstring[1] = widthstring[1] | widthbinary[16]<<6;
  widthstring[1] = widthstring[1] | widthbinary[17]<<7;
  widthstring[1] = widthstring[1] | widthbinary[18]<<8;
  */ 
//end  //!!!! what is this end for
  

//translates width code into the appropriate ascii character according to
//standard Code 39, to be filled into totalcode array      
unsigned char width2ascii(unsigned int widthchar)
begin
  unsigned char charindex;

  if (widthchar != 0x0000) //crap data while not active scanning, either all white or all black
  begin  
    
    for (charindex=0; charindex<44; charindex++) //only accepting 0 to 43
    begin    
      if (codetbl[charindex]==widthchar)  break;   
    end
    
    if (charindex<10) charindex+= 48; //scanned character is a digit
    else if(charindex>9 & charindex<36) charindex+= 55; //scanner char is 
                                                        //an alphabet
    else if(charindex ==36) charindex = 45; //-
    else if(charindex ==37) charindex = 46; //.
    else if(charindex ==38) charindex = 32; //space
    else if(charindex ==39) charindex = 42; //*
    else if(charindex ==40) charindex = 36; //$
    else if(charindex ==41) charindex = 47; //"/"
    else if(charindex ==42) charindex = 43; //+
    else if(charindex ==43) charindex = 37; //%
    else charindex = 0; //invalid code, outout error
    
  end    
  return charindex; //charindex is the current barcode character in ascii
end

//return the error value of current character, to be filled into errorvalue array
unsigned char width2error(unsigned int widthchar)
begin
  unsigned char charindex, errorvalue;

  if (widthchar != 0x0000)
  begin  
    
    for (charindex=0; charindex<44; charindex++) //only accepting 0 to 43
    begin    
      if (codetbl[charindex]==widthchar)  break;   
    end
    
    /*assign the error checking value for each scanned character,*/
    //for character index 0 to 38 (0 to 9, A to Z, -, ., space) 
    //errorvalue = index
    if(charindex>=0 & charindex<39) errorvalue = charindex;
    //for character index 39 to 44 ($, /, +,%) errorvalue = index-1
    //since we have to skip *
    else if(charindex>39 & charindex<44) errorvalue=  charindex-1;
    else if(charindex==39) errorvalue = 0; //doesnt matter, since element 0 
                                           //is not accounted for in errorcheck
  end
  return errorvalue;
end 

//extract the actual item code, from the 2nd character to 
//the 3rd from the last character,
//ex: * 1 2 3 4 A *
//we only want to store 1 2 3 4, since the two * are starting and ending 
//character, and A (errorvalue 10) is the error checking character
void getcode(void)
begin
  unsigned char i;
  for(i=1; i<stringlength-2; i++)
  begin
    itemcode[i-1]= totalcode[i];
  end
end

//check and see if the string we just scanned is correct, need to pass in
//errorvalue array and 2nd to last scanned character of the scanned string 
//in ascii
unsigned char errorcheck (unsigned char* errorarray)
begin
  unsigned char i, sum=0;
  /*
  errorarray[1] = 1;
  errorarray[2] = 2;
  errorarray[3] = 3;
  errorarray[4] = 4;
  errorarray[5] = 10;
  */
  for (i=1; i<stringlength-2;i++)
  begin
    sum += errorarray[i];
  end
  sum = sum % 43;
  //a scanned string is only correct if its character sum mod 43 == checkchat
  if (sum == errorarray[stringlength-2]) return 1;
  else return 0;
end 

//initialize the lcd screen, call in the beginning of the main program
void preplcd(void)
begin 
  lcd_init(LCDwidth);
  lcd_clear();
  lcd_gotoxy(0,0);
  //lcd_putsf("Number=");   //another string from flash 
  lcd_putsf("CDE:"); //product barcode (0,0) to (0,3) 

  lcd_gotoxy(9,0);
  lcd_putsf("PRC:"); //product price (0,9) to (0,12)  

  lcd_gotoxy(0,1);
  lcd_putsf("NAME:");//product name (1,0) to (1,4)

 
  /*
  sprintf(temp, "CDE:");
  lcd_puts(temp); //product barcode (0,0) to (0,3)
  lcd_gotoxy(0,9);
  sprintf(temp, "PRC:");
  lcd_puts(temp); //product price (0,9) to (0,12)
  lcd_gotoxy(1,0); 
  sprintf(temp, "NAME:");
  lcd_puts(temp);//product name (1,0) to (1,4) 
  */  
 

end

//wipe lcd clean except labels
void wipelcd(void)
begin
  lcd_clear();
  lcd_gotoxy(0,0);
  lcd_putsf("CDE:"); //product barcode (0,0) to (0,3)
  lcd_gotoxy(9,0);
  lcd_putsf("PRC:"); //product price (0,9) to (0,12)
  lcd_gotoxy(0,1);
  lcd_putsf("NAME:");//product name (1,0) to (1,4)
end

//takes in a inventory index and then outputs the product's barcode, 
//price and name onto the LCD screen
void printproduct(unsigned char invenindex)
begin
  unsigned char code_buffer[10], price_buffer[10], name_buffer[10];    
  unsigned char string[10];
  unsigned char indx;
  
  wipelcd();
  
  //not sure about conversion type, code and name are in ascii, price is in decimal
  sprintf(code_buffer, "%-i", inventory_code[invenindex]);
  sprintf(name_buffer, "%-i", inventory_name[invenindex]);
  sprintf(price_buffer, "%-i", inventory_price[invenindex]);
  wipelcd(); 
  for (indx = 0; indx < 4; indx++)
  begin
    string[indx] = inventory_code[invenindex][indx];
  end        
  string[4] = 0;
  lcd_gotoxy(4,0); // print inventory code
  //sprintf(string, "test");
  lcd_puts(string);   

  //lcd_puts(code_buffer); //print product barcode
  lcd_gotoxy(5,1);  
  for (indx = 0; indx < 7; indx++)
  begin
    string[indx] = inventory_name[invenindex][indx];
  end        
  string[7] = 0;  
  lcd_puts(string);
  
  lcd_gotoxy(13,0);
  lcd_puts(price_buffer);//print product price  
  //lcd_puts(price_buffer);  
  //lcd_puts(inventory_name[invenindex]);//print product name  

end

void printready(void)
begin
  wipelcd();
  lcd_gotoxy(11,1);
  lcd_putsf("READY");
end

// pass an array of length 4 to this function that contains the ascii code
void printascii(unsigned char *ascii)
begin
  ascii[4] = 0;
  lcd_gotoxy(4,0);
  lcd_puts(ascii);
end

/*
outputs error when:
1. Scanned character does not match any code 39 ascii codes
2. Scanned codes does not match any pre-defined inventory barcodes
*/
void printerror(void)
begin
  lcd_gotoxy(13,1);
  lcd_putsf("ERR"); //print ERR
end

  
