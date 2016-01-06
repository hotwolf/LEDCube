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
//###############################################################################
//# Version History:                                                            #
//#    January 4, 2016                                                          #
//#      - Initial release                                                      #
//###############################################################################

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

// Display text
//=============
void txtDisp (char *text) {
  //Clear buffer if necessary
  while (sketTstBuf()) {
    sketShFront;                                 //shift buffer front
    dispNextFrame();                             //wait for frame boundary
  }

  //Clear buffer if necessarySet through string
  while (*text != '\0') {
    unsigned int bitmap = txtFontTable[*text];  //lookup char
    sketLdCol(15, (bitmap >>  0));              //write bitmap to buffer
    sketLdCol(11, (bitmap >>  4));
    sketLdCol( 7, (bitmap >>  8));
    sketLdCol( 0, (bitmap >> 12));
    dispNextFrame();             //wait for frame boundary

    sketShFront;                 //shift buffer front
    dispNextFrame();             //wait for frame boundary

    text++;                                     //increment pointer
  }

  //Clear buffer
  while (sketTstBuf()) {
    sketShFront;                                 //shift buffer front
    dispNextFrame();                             //wait for frame boundary
  }
}

