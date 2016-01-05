//###############################################################################
//# LEDCube - Sketching Routines                                                #
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
//# Buffer format (array of 8 bytes):                                           #
//#              7  6  5  4    3  2  1  0                                       #
//#  index:    +--+--+--+--+ +--+--+--+--+                                      #
//#    0    C0 |L3|L2|L1|L0| |L3|L2|L1|L0| C1                                   #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    1    C2 |L3|L2|L1|L0| |L3|L2|L1|L0| C3                                   #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    2    C4 |L3|L2|L1|L0| |L3|L2|L1|L0| C5                                   #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    3    C6 |L3|L2|L1|L0| |L3|L2|L1|L0| C7                                   #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    4    C8 |L3|L2|L1|L0| |L3|L2|L1|L0| C9                                   #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    5   C10 |L3|L2|L1|L0| |L3|L2|L1|L0| C11                                  #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    6   C12 |L3|L2|L1|L0| |L3|L2|L1|L0| C13                                  #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#    7   C14 |L3|L2|L1|L0| |L3|L2|L1|L0| C15                                  #
//#            +--+--+--+--+ +--+--+--+--+                                      #
//#                                                                             #
//###############################################################################
//# Version History:                                                            #
//#    November 30, 2015                                                        #
//#      - Initial release                                                      #
//###############################################################################

// Variables
//==========
//Skeich buffer
byte      sketBuffer[8]      = {0, 0, 0, 0, 0, 0, 0, 0};

// Setup routine
//==============
void sketSetup() {
  //Clear sketBuffer
  sketClrBuf();
}

// Pixel operations
//=================
//Set a single pixel
void sketSetPx(byte column, byte level) {
  sketBuffer[column >> 1] |= (1 << (level + 4 - (4 * (column & 1))));
}

//Clear a single pixel
void sketClrPx(byte column, byte level) {
  sketBuffer[column >> 1] &= ~(1 << (level + 4 - (4 * (column & 1))));
}

//Invert a single pixel
void sketInvPx(byte column, byte level) {
  sketBuffer[column >> 1] = sketBuffer[column >> 1] ^ (1 << (level + 4 - (4 * (column & 1))));
}

//Test if a single pixel is set
boolean sketTstPx(byte column, byte level) {
  return ((sketBuffer[column >> 1] & (1 << (level + 4 - (4 * (column & 1))))) != 0);
}

// Column operations
//==================
//Set all pixels in a column
void sketSetCol(byte column) {
    sketBuffer[column >> 1] |= ((columnCount - 1) << (4 - (4 * (column & 1))));
}

//Clear all pixels in a column
void sketClrCol(byte column) {
  sketBuffer[column >> 1] &= ~((columnCount - 1) << (4 - (4 * (column & 1))));
}

//Invert all pixels in a column
void sketInvCol(byte column) {
  sketBuffer[column >> 1] = sketBuffer[column >> 1] ^ ((columnCount - 1) << (4 - (4 * (column & 1))));
}

//Test if any pixel in a column is set
boolean sketTstCol(byte column) {
  return ((sketBuffer[column >> 1] & ((columnCount - 1) << (4 - (4 * (column & 1))))) != 0);
}

//load a pattern into a column
void sketLdCol(byte column, byte pattern) {
  sketBuffer[column >> 1] = ((sketBuffer[column >> 1] & ((columnCount - 1) << (4 * (column & 1)))) |
                             (pattern & (columnCount - 1)) << (4 - (4 * (column & 1))));
}

// Level operations
//=================
//Set all pixels in a level
void sketSetLev(byte level) {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] |= ((1 << level) | (1 << (level + 4)));
  }
}

//Clear all pixels in a level
void sketClrLev(byte level) {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] &= ~((1 << level) | (1 << (level + 4)));
  }
}

//Invert all pixels in a level
void sketInvLev(byte level) {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] = sketBuffer[dcolumn] ^ ((1 << level) | (1 << (level + 4)));
  }
}

//Test if any pixel in a level is set
boolean sketTstLev(byte level) {
  boolean result = false;
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    result |= ((sketBuffer[dcolumn] & ((1 << level) | (1 << (level + 4)))) != 0);
  }
  return result;
}

// Buffer operations
//==================
//Set all pixels
void sketSetBuf() {
  sketBuffer[]  = {0xFF, 0xFF, 0xFF, 0xFF, 0XFF, 0XFF, 0XFF, 0xFF}; //set sketBuffer
}

//Clear all pixels
void sketClrBuf() {
    sketBuffer[]  = {0, 0, 0, 0, 0, 0, 0, 0}; //clear sketBuffer
}

//Invert all pixels
void sketInvBuf() {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] = ~sketBuffer[dcolumn];
  }
}

//Test if any pixel in the buffer is set
boolean sketTstBuf() {
  boolean result = false;
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    result |= (sketBuffer[dcolumn] != 0);
  }
  return result;
}

// Shift operations
//=================
//Shift all pixels one position down
void sketShDown() {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] & 0x77) << 1;
  }
}

//Shift all pixels one position up
void sketShUp() {
  for (byte dcolumn = 0; dcolumn < (columnCount/2); dcolumn++) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] & 0xEE) >> 1;
  }
}

//Shift all pixels one position front
void sketShFront() {
  //C0, C4, C8, and C12
  for (byte dcolumn = 0; dcolumn < (12/2); dcolumn += 2) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] << 8) & (sketBuffer[dcolumn+ 1] >> 8);
  }
  //C2, C6, C10, and C14
  for (byte dcolumn = (2/2); dcolumn < (14/2); dcolumn += 2) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] << 8);
  }
}

//Shift all pixels one position back
void sketShBack() {
  //C2, C6, C10, and C14
  for (byte dcolumn = (2/2); dcolumn < (14/2); dcolumn += 2) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] >> 8) & (sketBuffer[dcolumn - 1] << 8);
  }
  //C0, C4, C8, and C12
  for (byte dcolumn = 0; dcolumn < (12/2); dcolumn += 2) {
    sketBuffer[dcolumn] = (sketBuffer[dcolumn] >> 8);
  }
}

//Shift all pixels one position left
void sketShLeft() {
  //C0..C10
  for (byte dcolumn = 0; dcolumn < (10/2); dcolumn += 2) {
    sketBuffer[dcolumn] = sketBuffer[dcolumn + 2];
  }
  //C12 and C14
  sketBuffer[12/2] = 0;
  sketBuffer[14/2] = 0;
}

//Shift all pixels one position right
void sketShRight() {
  //C4..C14
  for (byte dcolumn = (4/2); dcolumn < (14/2); dcolumn += 2) {
    sketBuffer[dcolumn] = sketBuffer[dcolumn - 2];
  }
  //C0 and C2
  sketBuffer[0]   = 0;
  sketBuffer[2/2] = 0;
}
