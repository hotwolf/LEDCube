//###############################################################################
//# LEDCube - Buffer Routines                                                   #
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
//#    November 30, 2015                                                        #
//#      - Initial release                                                      #
//###############################################################################

// Pixel operations
//=================
//Set a single pixel
void setPixel(byte column, byte level) {
     buffer[column] |= (1 << (level + 4));
}

//Clear a single pixel
void clearPixel(byte column, byte level) {
     buffer[column] &= ~(1 << (level + 4));
}

//Invert a single pixel
void clearPixel(byte column, byte level) {
     buffer[column] = buffer[column] ^ (1 << (level + 4));
}

//Test if a single pixel is set
boolean testPixel(byte column, byte level) {
     return (buffer[column] | (1 << (level + 4));
}

// Column operations
//==================
//Set all pixels in a column
void setColumn(byte column) {
     buffer[column] |= drvLevelMask;
}

//Clear all pixels in a column
void clearColumn(byte column) {
     buffer[column] &= ~drvLevelMask;
}

//Invert all pixels in a column
void clearPixel(byte column) {
     buffer[column] = buffer[column] ^ drvLevelMask;
}

// Level operations
//=================
//Set all pixels in a level
void setLevel(byte level) {
     for (byte column = 0; column < 16; column++) {
         setPixel(column, level);
     }
}

//Clear all pixels in a level
void clearLevel(byte level) {
     for (byte column = 0; column < 16; column++) {
         clearPixel(column, level);
     }
}

//Invert all pixels in a level
void clearPixel(byte level) {
     for (byte column = 0; column < 16; column++) {
         invertPixel(column, level);
     }
}

// Buffer operations
//==================
//Set all pixels
void setBuffer(byte buffer) {
     for (byte column = 0; column < 16; column++) {
         setColumn(column);
     }
}

//Clear all pixels
void clearBuffer(byte buffer) {
     for (byte column = 0; column < 16; column++) {
         clearColumn(column);
     }
}

//Invert all pixels
void clearBuffer(byte buffer) {
     for (byte column = 0; column < 16; column++) {
         invertColumn(column);
     }
}

// Shift operations
//=================
//Shift all pixels one position down
void shiftBufferDown() {
     for (byte column = 0; column < 16; column++) {
         buffer[column] = (buffer[column] >> 1) & drvLevelMask;
     }
}

//Shift all pixels one position up
void shiftBufferDown() {
     for (byte column = 0; column < 16; column++) {
         buffer[column] = (buffer[column] << 1) & drvLevelMask;
     }
}

//Shift all pixels one position front
void shiftBufferFront() {
     for (byte column = 0; column < 16; column++) {
         if ((column & 3) == 3) {
            buffer[column] = 0;
         } else {
            buffer[column] = buffer[column + 1];
         }
     }
}

//Shift all pixels one position back
void shiftBufferBack() {
     for (byte column = 0; column < 16; column++) {
         byte  tmpColumn;
         if ((column & 3) == 0 ) {
            buffer[column] = 0;
         } else {
            buffer[column] = buffer[column - 1];
         }
     }
}

//Shift all pixels one position left
void shiftBufferLeft() {
     for (byte column = 0; column < 16; column++) {
         if ((column & 12) == 12) {
            buffer[column] = 0;
         } else {
            buffer[column] = buffer[column + 4];
         }
     }
}

//Shift all pixels one position right
void shiftBufferRight() {
     for (byte column = 0; column < 16; column++) {
         if ((column & 12) == 0) {
            buffer[column] = 0;
         } else {
            buffer[column] = buffer[column - 4];
         }
     }
}

