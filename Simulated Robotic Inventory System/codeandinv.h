/* I am going to assume that one part of our program will give us a 
bit pattern of 0's and 1's representing wide and narrow lines 
(shades dont matter) in succession */

/*note that each character should have 9 narrow/wide bits, I will assume
wide to be 1 and narrow to be 0 

ex: 
A => w n n n n w n n w 
  => 1 0 0 0 0 1 0 0 1

*/

/*
assuming each of our codes will be 7 characters long (begin *, end *, 
error check character and 4 actaul character codes), we'll use pointer
to extract each character from a string of [thick/thin]
*/




unsigned int widthstring[7]; //array of 7 unsigned integers serving as a place holder
                             //for barcode character comprison (comparison needs integer)
                         
unsigned char errorvalue[7]; //error checking value array, filled in after
                             //after calling width2ascii() each time 
                             //[code 39 decimal]
                            
unsigned char totalcode[7]; //store all the scanned character in this array
                            //after converting them into width2ascii(),
                            //[ascii]
                            
unsigned char itemcode[4];  //the current scanned code in ascii, not containing 
                            //the starting and ending * nor the checking digit
                            //[ascii]
                            
unsigned char itemindex=20; //holds the corresponding item index of the scanned barcode
                            //initialize to be 20 (there are only 10 pre-defined items)
                            //so if there is no match, we will know
                            

//thick-thin code table, 1-thick, 0-thin
flash unsigned int codetbl[44]=
{
      //0
      0b0000000000110100,
      //1
      0b0000000100100001,
      //2
      0b0000000001100001,
      //3
      0b0000000101100000,
      //4
      0b0000000000110001,
      //5
      0b0000000100110000,
      //6
      0b0000000001110000,
      //7
      0b0000000000100101,
      //8
      0b0000000100100100,
      //9
      0b0000000001100100,
      //A
      0b0000000100001001,
      //B
      0b0000000001001001,
      //C
      0b0000000101001000,
      //D
      0b0000000000011001,
      //E
      0b0000000100011000,
      //F
      0b0000000001011000,
      //G
      0b0000000000001101,
      //H
      0b0000000100001100,
      //I
      0b0000000001001100,
      //J
      0b0000000000011100,
      //K
      0b0000000100000011,
      //L
      0b0000000001000011,
      //M
      0b0000000101000010,
      //N
      0b0000000000010011,
      //O
      0b0000000100010010,
      //P
      0b0000000001010010,
      //Q
      0b0000000000000111,
      //R
      0b0000000100000110,
      //S
      0b0000000001000110,
      //T
      0b0000000000010110,
      //U
      0b0000000110000001,
      //V
      0b0000000011000001,
      //W
      0b0000000111000000,
      //X
      0b0000000010010001,
      //Y
      0b0000000110010000,
      //Z
      0b0000000011010000,
      //-
      0b0000000010000101,
      //.
      0b0000000110000100,
      //Space
      0b0000000011000100,
      //*
      0b0000000010010100,
      //$
      0b0000000010101000,
      //"/", left slash
      0b0000000010100010,
      //+
      0b0000000010001010,
      //%
      0b0000000000101010
      };



// //structure isnt working
// //defined inventory type, each item has a 
// //corresponding barcodes, name and price      
// struct inventory_struct{
//           //predefined inventory barcodes in ascii decimal
//           unsigned char code[inven][4]; 
//           //predefined inventory item price in dollars, 
//           //each corresponding to a barcodes
//           unsigned char price[inven];
//           //predefined invetory item name, there are "inven"
//           //number of them, each name is 7 character long
//           //in ascii
//           unsigned char name[inven][7];
//   };
//  
//  struct inventory_struct inventory = {
// 
// //initialize inventory barcodes in ascii, need more codes
// .code[0] = {'1', '2', '3', '4'}, //1 2 3 4
// .code[1] = {'5', '6', '7', '8'}, //5 6 7 8
// .code[2] = {'A', 'B', 'C', 'C'}, //A B C D
// .code[3] = {'E', 'F', 'G', 'H'}, //E F G H
// 
// //initialize inventory price in decimal, need more prices
// .price[0] = {1},
// .price[1] = {2},
// .price[2] = {3},
// .price[3] = {4},
// 
// //initialize inventory name, need more names
// inventory.name[0] = {'C', 'R', 'A', 'C', 'K', 'E', 'R'},
// inventory.name[1] = {'P', 'R', 'O', 'T', 'E', 'I', 'N'}
// 
// }    


//predefined inventory barcodes in ascii
unsigned char inventory_code[inven][4] = 
{

'1', '2', '3', '4',
'5', '6', '7', '8',
'A', 'B', 'C', 'D',
'E', 'F', 'G', 'H',
'H', 'E', 'L', 'L',
'L', 'E', 'T', 'S',
'P', 'L', 'A', 'Y',
'T', 'O', 'N', 'Y',
'N', 'A', 'T', 'E',
'T', '1', '0', '0'

};
 
//predefined inventory item price in dollars, 
//each corresponding to a barcodes
unsigned char inventory_price[inven] = 
{

1, 2, 8, 3, 2, 4, 45, 35, 5, 10

};


//predefined invetory item name, there are "inven"
//number of them, each name is 7 character long
//in ascii
unsigned char inventory_name[inven][7] =
{

'C','R','A','C','K','E','R',
'P','R','O','T','E','I','N',
'R','A','T','T','R','A','P',
'O','R','A','N','G','E','S',
'C','R','E','A','M','E','R',
'J','A','W','B','R','K','R',
'C','O','L','T','4','5','M',
'L','U','G','E','R','9','M',
'N','E','R','F','B','A','T',
'P','O','T','B','O','N','G'

};      