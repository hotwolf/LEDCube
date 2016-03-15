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
//#    November 30, 2015                                                        #
//#      - Initial release                                                      #
//###############################################################################

#include "LEDCube.h"

// Pixel operations
//=================
//Set a single pixel
ledState sketSetPx(ledState frame, byte column, byte level) {
  return (frame | (1 << ((LEVELS * column) + level)));
}

//Clear a single pixel
ledState sketClrPx(ledState frame, byte column, byte level) {
  return (frame & ~(1 << ((LEVELS * column) + level)));
}

//Invert a single pixel
ledState sketInvPx(ledState frame, byte column, byte level) {
  return (frame ^ (1 << ((LEVELS * column) + level)));
}

//Test if a single pixel is set
boolean sketTstPx(ledState frame, byte column, byte level) {
  return (frame & (1 << ((LEVELS * column) + level)));
}

// Column operations
//==================
//Set all pixels in a column
ledState sketSetCol(ledState frame, byte column) {
  return (frame | (((1 << LEVELS) - 1) << (LEVELS * column))); // ((1 << LEVELS) - 1) = 0xF
}

//Clear all pixels in a column
ledState sketClrCol(ledState frame, byte column) {
  return (frame & ~(((1 << LEVELS) - 1) << (LEVELS * column)));
}

//Invert all pixels in a column
ledState sketInvCol(ledState frame, byte column) {
  return (frame ^ (((1 << LEVELS) << (LEVELS * column))));
}

//Insert a column
ledState sketInsCol(ledState frame, byte column, byte pattern) {
  return (frame &
          (~(((1 << LEVELS)            << (LEVELS * column))) |
            (((1 << LEVELS) & pattern) << (LEVELS * column))));
}

//Test if any pixel in a column is set
boolean sketTstCol(byte column) {
  return (frame & (((1 << LEVELS) << (LEVELS * column))));
}

// Level operations
//=================
//Set all pixels in a level
ledState sketSetLev(ledState frame, byte level) {
  return (frame | (LED_STATE_LEVEL_0 << level));
}

//Clear all pixels in a level
ledState sketClrLev(ledState frame, byte level) {
  return (frame & ~(LED_STATE_LEVEL_0 << level));
}
  
//Invert all pixels in a level
ledState sketInvLev(ledState frame, byte level) {
  return (frame ^ (LED_STATE_LEVEL_0 << level));
}

//Test if any pixel in a level is set
boolean sketTstLev(ledState frame, byte level) {
  return (frame & (LED_STATE_LEVEL_0 << level));
}

// Buffer operations
//==================
//Set all pixels
ledState sketSetBuf() {
  return LED_STATE_ALL_ON;
}

//Clear all pixels
ledState sketClrBuf() {
  return LED_STATE_ALL_OFF;
}

//Invert all pixels
ledState sketInvBuf(ledState frame) {
  return ~frame;
}

//Test if any pixel in the buffer is set
boolean sketTstBuf(ledState frame) {
  return frame;
}

// Shift operations
//=================
//Shift all pixels one position in X direction
ledState  sketShiftX(ledState frame) {
  return (frame << (LEVELS*LEVELS));
}

//Shift all pixels one position in opposite X direction
ledState  sketUnshiftX(ledState frame) {
  return (frame >> (LEVELS*LEVELS));
}

//Shift all pixels one position in Y direction
ledState  sketShiftY(ledState frame) {
  return ((frame << LEVELS) & ~LED_STATE_SLICE_Y0);
}

//Shift all pixels one position in opposite Y direction
ledState  sketUnshiftY(ledState frame) {
  return ((frame & ~LED_STATE_LEVEL_0) >> 1);
 }

//Shift all pixels one position in Z direction
ledState  sketShiftZ(ledState frame) {
  return ((frame << 1) & ~LED_STATE_LEVEL_0);
}

//Shift all pixels one position in opposite Z direction
ledState  sketUnshiftZ(ledState frame) {
  return ((frame & ~LED_STATE_LEVEL_0) >> 1);
}
