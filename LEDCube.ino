//###############################################################################
//# LEDCube - Main                                                              #
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
//#    November 30, 2015                                                        #
//#      - Initial release                                                      #
//###############################################################################

// General definitions
//====================
const int columnCount       = 16;
const int levelCount        =  4;

// Pin definitions
//================
const byte PL3               = 7;        //level 3                   7 (PD7)
const byte PL2               = 6;        //level 2                   6 (PD6)
const byte PL1               = 5;        //level 1                   5 (PD5)
const byte PL0               = 4;        //level 0                   4 (PD4)

const byte PDS               = 3;        //serial data input         3 (PD3)
const byte POE               = 2;        //output enable, active low 2 (PD2)
const byte PST               = 1;        //storage clock             1 (PD1)
const byte PSH               = 0;        //shift clock               0 (PD0)

// Bit definitions
//================
const byte L3                = (1 << PL3);
const byte L2                = (1 << PL2);
const byte L1                = (1 << PL1);
const byte L0                = (1 << PL0);

const byte DS                = (1 << PDS);
const byte OE                = (1 << POE);
const byte ST                = (1 << PST);
const byte SH                = (1 << PSH);

// Variables
//==========
//Skeich buffer
extern byte sketBuffer[];

// Setup routine
//==============
void setup() {
     //Disable interrupts until setup is complete
     noInterrupts();

     //Display routines
     dispSetup();

     //Sketch routines
     //sketSetup();

     //Enable interrupts
     interrupts();
}

// Application loop
//=================
void loop() {

  //Set sketch buffer
  sketBuffer[0]  = 0x01;
  sketBuffer[1]  = 0x23;
  sketBuffer[2]  = 0x45;
  sketBuffer[3]  = 0x67;
  sketBuffer[4]  = 0x89;
  sketBuffer[5]  = 0xAB;
  sketBuffer[6]  = 0xCD;
  sketBuffer[7]  = 0xEF;

  //wait for frame boundary
  dispNextFrame();
}
