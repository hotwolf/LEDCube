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
//# Buffer format:                                                              #
//#   Array of 16 bytes. The index of the array corresponds to the column       #
//#   number. The upper 4-bits of each byte correspond to the LEDs in each      #
//#   column:    7  6  5  4  3  2  1  0                                         #
//#            +--+--+--+--+--+--+--+--+                                        #
//#            |L3|L2|L1|L0|   unused  |                                        #
//#            +--+--+--+--+--+--+--+--+                                        #
//#                                                                             #
//###############################################################################
//# Version History:                                                            #
//#    January 4, 2016                                                          #
//#      - Initial release                                                      #
//###############################################################################


const unsigned int fontTable[]PROGMEM = {0x0660, // 0x00 NUL  ....  ....  ....  ....
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
                                         0x0660, // 0x21 !    ....  ..#.  .#.#  ####
                                         0x0660, // 0x22 "    ....  ....  ....  ####
                                         0x0660, // 0x23 #    ....  ..#.  ....  .##.
						      2
                                         0x0660, // 0x24 $    ....  ....  ....  ....
                                         0x0660, // 0x25 %    .##.  .##.  .##.  .##.
                                         0x0660, // 0x26 &    .##.  .##.  .##.  .##.
                                         0x0660, // 0x27 '    ....  ....  ....  ....
						      2
                                         0x0660, // 0x28 (    ....  ....  ....  ....
                                         0x0660, // 0x29 )    .##.  .##.  .##.  .##.
                                         0x0660, // 0x2A *    .##.  .##.  .##.  .##.
                                         0x0660, // 0x2B +    ....  ....  ....  ....
						      2
                                         0x0660, // 0x2C ,    ....  ....  ....  ....
                                         0x0660, // 0x2D -    .##.  .##.  .##.  .##.
                                         0x0660, // 0x2E .    .##.  .##.  .##.  .##.
                                         0x0660, // 0x2F /    ....  ....  ....  ....

                                         0x0660, // 0x30 0    ....  ....  ....  ....
                                         0x0660, // 0x31 1    .##.  .##.  .##.  .##.
                                         0x0660, // 0x32 2    .##.  .##.  .##.  .##.
                                         0x0660, // 0x33 3    ....  ....  ....  ....
						      3
                                         0x0660, // 0x34 4    ....  ....  ....  ....
                                         0x0660, // 0x35 5    .##.  .##.  .##.  .##.
                                         0x0660, // 0x36 6    .##.  .##.  .##.  .##.
                                         0x0660, // 0x37 7    ....  ....  ....  ....
						      3
                                         0x0660, // 0x38 8    ....  ....  ....  ....
                                         0x0660, // 0x39 9    .##.  .##.  .##.  .##.
                                         0x0660, // 0x3A :    .##.  .##.  .##.  .##.
                                         0x0660, // 0x3B ;    ....  ....  ....  ....
						      3
                                         0x0660, // 0x3C <    ....  ....  ....  ....
                                         0x0660, // 0x3D =    .##.  .##.  .##.  .##.
                                         0x0660, // 0x3E >    .##.  .##.  .##.  .##.
                                         0x0660, // 0x3F ?    ....  ....  ....  ....

const byte fontChar_A[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .##.  ###.  .###  ###.
const byte fontChar_B[]PROGMEM = {0xF0, 0x90, 0xB0, 0x40); // #..#  #.#.  #...  #..#
const byte fontChar_C[]PROGMEM = {0x60, 0x90, 0x90, 0x90); // ####  #..#  #...  #..#
const byte fontChar_D[]PROGMEM = {0xF0, 0x90, 0x90, 0x60); // #..#  ###.  .###  ###.

const byte fontChar_E[]PROGMEM = {0xF0, 0xB0, 0xB0, 0x90); // ####  ####  .##.  #..#
const byte fontChar_F[]PROGMEM = {0xF0, 0x50, 0x10, 0x10); // ###.  #...  #...  #..#
const byte fontChar_G[]PROGMEM = {0x60, 0x90, 0x90, 0xC0); // #...  ##..  #..#  ####
const byte fontChar_H[]PROGMEM = {0xF0, 0x40, 0x40, 0xF0); // ####  #...  .###  #..#

const byte fontChar_I[]PROGMEM = {0x00, 0x90, 0xF0, 0x90); // .###  ####  #.#.  #...
const byte fontChar_J[]PROGMEM = {0x50, 0x90, 0x90, 0x70); // ..#.  ...#  ##..  #...
const byte fontChar_K[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ..#.  #..#  #.#.  #...
const byte fontChar_L[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .###  .##.  #..#  ####

const byte fontChar_M[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ####  #..#  .##.  ###.
const byte fontChar_N[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ####  ##.#  #..#  #..#
const byte fontChar_O[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  #.##  #..#  ###.
const byte fontChar_P[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  #..#  .##.  #...

const byte fontChar_Q[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .##.  ###.  .###  ####
const byte fontChar_R[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  #..#  ###.  ..#.
const byte fontChar_S[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #.##  ###.  ...#  ..#.
const byte fontChar_T[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .###  #..#  ###.  ..#.

const byte fontChar_U[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  #..#  #..#  #..#
const byte fontChar_V[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  #..#  #..#  .##.
const byte fontChar_W[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  .#.#  ####  .##.
const byte fontChar_X[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .##.  ..#.  ####  #..#

const byte fontChar_Y[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #..#  ####
const byte fontChar_Z[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .#.#  ..#.
                                                           // ..#.  .#..
                                                           // ..#.  ####

const byte fontChar_1[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ..#.  ###.  ###.  #...
const byte fontChar_2[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .##.  ...#  .###  #.#.
const byte fontChar_3[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // #.#.  .##.  ...#  ####
const byte fontChar_4[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ..#.  ####  ###.  ..#.

const byte fontChar_5[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ####  .###  ####  .##.
const byte fontChar_6[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ###.  #...  ...#  ####
const byte fontChar_7[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ...#  ####  ..#.  #..#
const byte fontChar_8[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ###.  .##.  ..#.  .##.

const byte fontChar_9[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // .##.  .##.
const byte fontChar_0[]PROGMEM = {0xE0, 0x50, 0x50, 0xE0); // ####  #..#
                                                           // ...#  #..#
                                                           // ###.  .##.
//
// ###.   ..#.  ....  ....
// ...#   ..#.  .##.  ....
// .#..   ....  .##.  ....
// .#..   ..#.  ....  ..#.
//
// ....   ....  ....  ....
// ....   ....  ....  ....
// ..#.   ....  ....  ....
// ..#.   ....  ....  ....
//
// ....   ....  ....  ....
// ....   ....  ....  ....
// ....   ....  ....  ....
// ....   ....  ....  ....
//

   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,??
   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,   5,
   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,   6,   7,   7,
