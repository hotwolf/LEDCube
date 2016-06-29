//###############################################################################
//# LEDCube - Text Routines                                                     #
//###############################################################################
//#    Copyright 2015 - 2016 Dirk Heisswolf                                     #
//#    This file is part of the LEDCube project.                                #
//#                                                                             #
//#    LEDCube is free software: you can redistribute it and/or modify          #
//#    it under the terms of the GNU General Public License as published by     #
//#    the Free Software Foundation, either version 3 of the License, or        #
//#    (at your option) any later version.                                      #
//#                                                                             #
//#    LEDCube is distributed in the hope that it will be useful,               #
//#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
//#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
//#    GNU General Public License for more details.                             #
//#                                                                             #
//#    You should have received a copy of the GNU General Public License        #
//#    along with LEDCube.  If not, see <http://www.gnu.org/licenses/>.         #
//###############################################################################
//# Description:                                                                #
//#                                                                             #
//#   Y                                                                         #
//#  /         C3---C7---C11--C15    Anode connections:                         #
//#  --X       /    /    /    / |    C0-C15: Q outputs of 74HC595s              #
//# |        C2---C6---C10--C14 @                                               #
//# Z        /    /    /    / |/|    74HC595 control:                           #
//#        C1---C5---C9---C13 @ @    DS (serial data input):         3 (PD3)    #
//#        /    /    /    / |/|/|    OE (output enable, active low): 2 (PD2)    #
//#  L0  C0---C4---C8---C12 @ @ @    ST (storage clock):             1 (PD1)    #
//#       |    |    |    | /|/|/     SH (shift clock):               0 (PD0)    #
//#  L1  C0---C4---C8---C12 @ @                                                 #
//#       |    |    |    | /|/       Cathode connections:                       #
//#  L2  C0---C4---C8---C12 @        L0: 4 (PD4)                                #
//#       |    |    |    | /         L1: 5 (PD5)                                #
//#  L3  C0---C4---C8---C12          L2: 6 (PD6)                                #
//#                                  L3: 7 (PD7)                                #
//#                                                                             #
//# LED state format (unsigned 64-bit integer):                                 #
//#                                                                             #
//#                  C15         C14         C13         C12                    #
//#             +-----------+-----------+-----------+-----------+               #
//#             |L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|               #
//#             +-----------+-----------+-----------+-----------+               #
//#              63       60 59       56 55       52 51       48                #
//#                  C11         C10         C9          C8                     #
//#             +-----------+-----------+-----------+-----------+               #
//#             |L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|               #
//#             +-----------+-----------+-----------+-----------+               #
//#              47       44 43       40 39       36 35       32                #
//#                  C7          C6          C5          C4                     #
//#             +-----------+-----------+-----------+-----------+               #
//#             |L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|               #
//#             +-----------+-----------+-----------+-----------+               #
//#              31       28 27       24 23       20 19       16                #
//#                  C3          C2          C1          C0                     #
//#             +-----------+-----------+-----------+-----------+               #
//#             |L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|L3 L2 L1 L0|               #
//#             +-----------+-----------+-----------+-----------+               #
//#              15       12 11        8  7        4  3        0                #
//#                                                                             #
//###############################################################################
//# Version History:                                                            #
//#    January 4, 2016                                                          #
//#      - Initial release                                                      #
//###############################################################################

// General definitions
//====================
//Framerate
#define FRAME_REPETITION        4                   //frames per second         

// Font look-up table
//===================
const unsigned int txtFontTable[]PROGMEM = {0x0660, // 0x00 NUL  ....  ....  ....  ....
                                            0x0660, // 0x01 SOH  .##.  .##.  .##.  .##.
                                            0x0660, // 0x02 STX  .##.  .##.  .##.  .##.
                                            0x0660, // 0x03 ETX  ....  ....  ....  ....

                                            0x0660, // 0x04 EOT  ....  ....  ....  ....
                                            0x0660, // 0x05 ENQ  .##.  .##.  .##.  .##.
                                            0x0660, // 0x06 ACK  .##.  .##.  .##.  .##.
                                            0x0660, // 0x07 BEL  ....  ....  ....  ....

                                            0x0660, // 0x08 BS   ....  ....  ...#  ....
                                            0x0660, // 0x09 HT   .##.  .##.  .#.#  .##.
                                            0x4E47, // 0x0A LF   .##.  .##.  ####  .##.
                                            0x0660, // 0x0B VT   ....  ....  .#..  ....

                                            0x0660, // 0x0C FF   ....  ...#  ....  ....
                                            0x4E47, // 0x0D CR   .##.  .#.#  .##.  .##.
                                            0x0660, // 0x0E SO   .##.  ####  .##.  .##.
                                            0x0660, // 0x0F SI   ....  .#..  ....  ....

                                            0x0660, // 0x10 DLE  ....  ....  ....  ....
                                            0x0660, // 0x11 DC1  .##.  .##.  .##.  .##.
                                            0x0660, // 0x12 DC2  .##.  .##.  .##.  .##.
                                            0x0660, // 0x13 DC3  ....  ....  ....  ....

                                            0x0660, // 0x14 DC4  ....  ....  ....  ....
                                            0x0660, // 0x15 NAK  .##.  .##.  .##.  .##.
                                            0x0660, // 0x16 SYN  .##.  .##.  .##.  .##.
                                            0x0660, // 0x17 ETB  ....  ....  ....  ....

                                            0x0660, // 0x18 CAN  ....  ....  ....  ....
                                            0x0660, // 0x19 EM   .##.  .##.  .##.  .##.
                                            0x0660, // 0x1A SUB  .##.  .##.  .##.  .##.
                                            0x0660, // 0x1B ESC  ....  ....  ....  ....

                                            0x0660, // 0x1C FS   ....  ....  ....  ....
                                            0x0660, // 0x1D GS   .##.  .##.  .##.  .##.
                                            0x0660, // 0x1E RS   .##.  .##.  .##.  .##.
                                            0x0660, // 0x1F US   ....  ....  ....  ....

                                            0x0000, // 0x20 SP   ....  ..#.  .#.#  .##.
                                            0x00B0, // 0x21 !    ....  ..#.  .#.#  ####
                                            0x0303, // 0x22 "    ....  ....  ....  ####
                                            0x6FF6, // 0x23 #    ....  ..#.  ....  .##.

                                            0x46F2, // 0x24 $    ..#.  #..#  .#..  ..#.
                                            0x9429, // 0x25 %    .###  ..#.  ##.#  ..#.
                                            0x6F4A, // 0x26 &    ###.  .#..  ###.  ....
                                            0x0030, // 0x27 '    ..#.  #..#  .#.#  ....

                                            0x0690, // 0x28 (    ..#.  .#..  ..#.  ..,.
                                            0x0960, // 0x29 )    .#..  ..#.  ####  ..#.
                                            0xA67A, // 0x2A *    .#..  ..#.  .##.  .###
                                            0x04E4, // 0x2B +    ..#.  .#..  #..#  ..#.

                                            0x0840, // 0x2C ,    ....  ....  ....  ...#
                                            0x0444, // 0x2D -    ....  ....  ....  ..#.
                                            0x0080, // 0x2E .    ..#.  .###  ....  .#..
                                            0x8421, // 0x2F /    .#..  ....  ..#.  #...

                                            0x6DB6, // 0x30 0    .##.  ..#.  ###.  ###.
                                            0x42F0, // 0x31 1    #.##  .##.  ...#  .###
                                            0x9DDA, // 0x32 2    ##.#  #.#.  .##.  ...#
                                            0x9BB6, // 0x33 3    .##.  ..#.  ####  ###.

                                            0x74E4, // 0x34 4    #...  ####  .##.  ####
                                            0xBBB5, // 0x35 5    #.#.  ###.  #...  ...#
                                            0x6DD4, // 0x36 6    ####  ...#  ####  ..#.
                                            0x11D3, // 0x37 7    ..#.  ###.  .##.  ..#.

                                            0x4BB4, // 0x38 8    .##.  .##.  ....  ..#.
                                            0x2BB6, // 0x39 9    .##.  ####  ..#.  ....
                                            0x0660, // 0x3A :    #..#  ...#  ....  ..#.
                                            0x0850, // 0x3B ;    .##.  .##.  ..#.  .#..

                                            0x04A0, // 0x3C <    ....  ....  ....  ###.
                                            0x0AAA, // 0x3D =    ..#.  .###  .#..  ...#
                                            0x0A40, // 0x3E >    .#..  ....  ..#.  ..#.
                                            0x11D2, // 0x3F ?    ..#.  .###  .#..  ..#.

                                            0x6FDA, // 0x40 @    .##.  .##.  ###.  .###
                                            0xE55E, // 0x41 A    ##.#  #..#  #.#.  #...
                                            0xF9B4, // 0x42 B    ###.  ####  #..#  #...
                                            0x6999, // 0x43 C    .###  #..#  ###.  .###

                                            0xF996, // 0x44 D    ###.  ####  ####  .##.
                                            0xFBB9, // 0x45 E    #..#  ###.  #...  #...
                                            0xF551, // 0x46 F    #..#  #...  ###.  #..#
                                            0x699C, // 0x47 G    ###.  ####  #...  .###

                                            0xF44F, // 0x48 H    #..#  .###  ####  #.#.
                                            0x09F9, // 0x49 I    #..#  ..#.  ...#  ##..
                                            0x5997, // 0x4A J    ####  ..#.  #..#  #.#.
                                            0xF258, // 0x4B K    #..#  .###  .##.  #..#

                                            0xF888, // 0x4C L    #...  #.##  #..#  .##.
                                            0xF23F, // 0x4D M    #...  ####  ##.#  #..#
                                            0xF24F, // 0x4E N    #...  #..#  #.##  #..#
                                            0x6996, // 0x4F O    ####  #..#  #..#  .##.

                                            0xF552, // 0x50 P    ###.  .##.  ###.  .###
                                            0x69DE, // 0x51 Q    #..#  #..#  #..#  ###.
                                            0xF55A, // 0x52 R    ###.  #.##  ###.  ...#
                                            0xABB5, // 0x53 S    #...  .###  #..#  ###.

                                            0x11F1, // 0x54 T    ####  #..#  #..#  #..#
                                            0x7887, // 0x55 U    ..#.  #..#  #..#  #..#
                                            0x3487, // 0x56 V    ..#.  #..#  .#.#  ####
                                            0xFC4F, // 0x57 W    ..#.  .##.  ..#.  ##.#

                                            0x9669, // 0x58 X    #..#  #..#  ####  .###
                                            0x12C3, // 0x59 Y    .##.  .#.#  ..#.  .#..
                                            0x9DB9, // 0x5A Z    .##.  ..#.  .#..  .#..
                                            0x0F99, // 0x5B [    #..#  ..#.  ####  .###

                                            0x1248, // 0x5C \    #...  ###.  ..#.  ....
                                            0x99F0, // 0x5D ]    .#..  ..#.  .#.#  ....
                                            0x0212, // 0x5E ^    ..#.  ..#.  ....  ....
                                            0x8888, // 0x5F _    ...#  ###.  ....  ####

                                            0x0120, // 0x60 `    .#..  .##.  ###.  .###
                                            0xE55E, // 0x61 a    ..#.  #..#  #.#.  #...
                                            0xF9B4, // 0x62 b    ....  ####  #..#  #...
                                            0x6999, // 0x63 c    ....  #..#  ###.  .###

                                            0xF996, // 0x64 d    ###.  ####  ####  .##.
                                            0xFBB9, // 0x65 e    #..#  ###.  #...  #...
                                            0xF551, // 0x66 f    #..#  #...  ###.  #..#
                                            0x699C, // 0x67 g    ###.  ####  #...  .###

                                            0xF44F, // 0x68 h    #..#  .###  ####  #.#.
                                            0x09F9, // 0x69 i    #..#  ..#.  ...#  ##..
                                            0x5997, // 0x6A j    ####  ..#.  #..#  #.#.
                                            0xF258, // 0x6B k    #..#  .###  .##.  #..#

                                            0xF888, // 0x6C l    #...  #.##  #..#  .##.
                                            0xF23F, // 0x6D m    #...  ####  ##.#  #..#
                                            0xF24F, // 0x6E n    #...  #..#  #.##  #..#
                                            0x6996, // 0x6F o    ####  #..#  #..#  .##.

                                            0xF552, // 0x70 p    ###.  .##.  ###.  .###
                                            0x69DE, // 0x71 q    #..#  #..#  #..#  ###.
                                            0xF55A, // 0x72 r    ###.  #.##  ###.  ...#
                                            0xABB5, // 0x73 s    #...  .###  #..#  ###.

                                            0x11F1, // 0x74 t    ####  #..#  #..#  #..#
                                            0x7887, // 0x75 u    ..#.  #..#  #..#  #..#
                                            0x3487, // 0x76 v    ..#.  #..#  .#.#  ####
                                            0xFC4F, // 0x77 w    ..#.  .##.  ..#.  ##.#

                                            0x9669, // 0x78 x    #..#  #..#  ####  ..#.
                                            0x12C3, // 0x79 y    .##.  .#.#  ..#.  ##..
                                            0x9DB9, // 0x7A z    .##.  ..#.  .#..  ##..
                                            0x6690, // 0x7B {    #..#  ..#.  ####  ..#.

                                            0x00F0, // 0x7C |    ..#.  .#..  ....  ....
                                            0x0966, // 0x7D }    ..#.  ..##  .#.#  .##.
                                            0x4242, // 0x7E ~    ..#.  ..##  #.#.  .##.
                                            0x0660};// 0x7F DEL  ..#.  .#..  ....  ....

// Display text (back to front)
//=============================
ledState txtDispBtf (ledState frame, char *text) {
  //Shift out previous content
  //frame = txtClrBtf (frame);

  //Display string 
  while (*text != '\0') {
    frame =  txtDispCharBtf (frame, *text);         //display character
    text++;                                         //increment string pointer
  }
  
  //Shift out text
  frame = txtClrBtf (frame);

  //Return updated frame
  return frame;
}

// Display char (back to front)
//=============================
ledState txtDispCharBtf (ledState frame, char character) {
  //Local variables
  unsigned int fontPattern;

  //Display empty slice
  frame = sketUnshiftY(frame);                      //shift frame by one position
  dispQueueFrames(frame, 4);                             //display frame
  
  //Lookup font pattern
  fontPattern = txtFontTable[character & 0x7F];

  //Display font pattern
  frame = sketUnshiftY(frame);                      //shift frame by one position
  frame |= (((0xF000 & fontPattern)            ) |  //C3
            ((0x0F00 & fontPattern) << ( 5 * 4)) |  //C7
            ((0x00F0 & fontPattern) << (10 * 4)) |  //C11
            ((0x000F & fontPattern) << (15 * 4)));  //C11
  dispQueueFrames(frame, 4);                          //display frame

  //Return updated frame
  return frame;
}

// Shift out content
//==================
ledState txtClrBtf (ledState frame) {
  while (frame) {                                   //check if any LEDs are lit
    frame = sketUnshiftY(frame);                    //shift frame by one position
    dispQueueFrames(frame, 4);                           //display frame
  }
  return frame;                                     //return empty frame
}

// Display text (scrol)
//=====================
ledState txtDispScr (ledState frame, char *text) {
  //Shift out previous content
  //frame = txtClrScr (frame);

  //Display string 
  while (*text != '\0') {
    frame =  txtDispCharscr (frame, *text);         //display character
    text++;                                         //increment string pointer
  }
  
  //Shift out text
  frame = txtClrScr (frame);

  //Return updated frame
  return frame;
}


// Display char (scroll)
//=============================
ledState txtDispCharScr (ledState frame, char character) {
  //Local variables
  unsigned int fontPattern;

  //Print one blank column 
  frame = txtShiftScr (frame);                       //scroll by one column
  dispQueueFrame(frame);                             //display frame
  
  //Lookup font pattern
  fontPattern = txtFontTable[character & 0x7F];     //look up char pitmap

  //Print first column
  if (0xF000 & fontPattern) {                        //skip empty column
    frame =  txtShiftScr (frame);                    //scroll by one column
    frame |= (0xF000 & fontPattern)<<48;             //insert char column
    dispQueueFrame(frame);                           //display frame
  }

  //Print second column
  if (0x0F00 & fontPattern) {                        //skip empty column
    frame =  txtShiftScr (frame);                    //scroll by one column
    frame |= (0x0F00 & fontPattern)<<52;             //insert char column
    dispQueueFrame(frame);                           //display frame
  }

  //Print third column
  frame =  txtShiftScr (frame);                      //scroll by one column
  frame |= (0x00F0 & fontPattern)<<56;               //insert char column
  dispQueueFrame(frame);                             //display frame

  //Print forth column
  if (0x000F & fontPattern) {                        //skip empty column
    frame =  txtShiftScr (frame);                    //scroll by one column
    frame |= (0x000F & fontPattern)<<60;             //insert char column
    dispQueueFrame(frame);                           //display frame
  }
  
  return frame;
}

// Scroll text by one line (scroll)
//=================================
ledState txtShiftScr (ledState frame) {
  return (0x0000_0000_0000_FFFF & (frame<<4))  |     //shift C0->C1->C2->C3
  	     (0x000F_000F_000F_000F & (frame>>16)) |     //shift C12->C8->C4->C0
  	     (0xFFFF_0000_0000_0000 & (frame>>4));       //shift C15->C14->C13->C12
}

// Scroll out content
//===================
ledState txtClrScr (ledState frame) {
  while (frame) {                                    //check if any LEDs are lit
    frame = txtShiftScr(frame);                      //shift frame by one position
    dispQueueFrame(frame, 4);                        //display frame
  }
  return frame;                                      //return empty frame
}





