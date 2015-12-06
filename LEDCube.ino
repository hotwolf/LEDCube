//###############################################################################
//# LEDCube - Main                                                              #
//###############################################################################
//#    Copyright 2015 Dirk Heisswolf                                            #
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
//#    git                #
//#   Y                                                                         #
//#  /         C3---C7---C11--C15    Anode connections:                         #
//#  --X       /    /    /    / |    C0-C15: Q outputs of 74HC595s              #
//# |        C2---C6---C10--C14 @                                               #
//# Z    L0  /    /    /    / |/|    74HC595 control:                           #
//#        C1---C5---C9---C13 @ @    DS (serial data input):         3 (PD3)    #
//#        /    /    /    / |/|/|    OE (output enable, active low): 2 (PD2)    #
//#      C0---C4---C8---C12 @ @ @    ST (storage clock):             1 (PD1)    #
//#       |    |    |    | /|/|/     SH (shift clock):               0 (PD0)    #
//#  L1  C0---C4---C8---C12 @ @                                                 #
//#       |    |    |    | /|/       Cathode connections:                       #
//#  L2  C0---C4---C8---C12 @        L0: 4 (PD4)                                #
//#       |    |    |    | /         L1: 5 (PD5)                                #
//#  L3  C0---C4---C8---C12          L2: 6 (PD6)                                #
//#                                  L3: 7 (PD7)                                #
//#                                                                             #
//#    Buffer format:                                                           #                                                                             #
//# Column:  C15 C14 C13 C12 C11 C10 C9  C8  C7  C6  C5  C4  C3  C2  C1  C0     #                                                                   #
//# Level:  3210321032103210321032103210321032103210321032103210321032103210    #                                                                   #                                                                             #
//# Index:  |15||14||13||12||11||10|| 9|| 8|| 7|| 6|| 5|| 4|| 3|| 2|| 1|| 0|    #                                                                  #
//#                                                                             #
//###############################################################################
//# Version History:                                                            #
//#    November30, 2015                                                         #
//#      - Initial release                                                      #
//###############################################################################

// Setup routine
void setup() {
}

// Application loop
void loop() {
}
