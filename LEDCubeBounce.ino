//###############################################################################
//# LEDCube - "Bounce" Animation                                                #
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

// Animation
//==========
ledState bounceAnimation(ledState frame) {
  //Variables
  int     frameCount = 100;                           //150 frames ~10s
  boolean xDirection = true;                          //True = positive direction
  boolean yDirection = true;                          //True = positive direction
  boolean zDirection = true;                          //True = positive direction
  ledState head  = 0x0000000000400000;
  ledState tail0 = 0x0000000000000000;
   
  while (head|tail0) {                                //loop as long as there is anything to display

    //Bounce off outside walls
    if (frameCount--) {                               //stop bouncing when animation is over
      if (head & 0x000000000000FFFF) {xDirection = true;}   //bounce off left wall
      if (head & 0xFFFF000000000000) {xDirection = false;}  //bounce off right wall

      if (head & 0x000F000F0000000F) {yDirection = true;}   //bounce off front wall
      if (head & 0xF000F000F000F000) {yDirection = false;}  //bounce off back wall
      
      if (head & 0x1111111111111111) {zDirection = true;}   //bounce off ceiling
      if (head & 0x8888888888888888) {zDirection = false;}  //bounce off floor
    }
    
    //Update tail
     tail0 = head;

    //Update head
    if (xDirection) {head = sketShiftX(head);}
    else            {head = sketUnshiftX(head);}
      
    if (yDirection) {head = sketShiftY(head);}
    else            {head = sketUnshiftY(head);}
     
    if (zDirection) {head = sketShiftZ(head);}
    else            {head = sketUnshiftZ(head);}
       
    //Draw new frame
    dispQueueFrame(head|tail0);
  }

  //Draw last (empty) frame
  dispQueueFrame(LED_STATE_ALL_OFF);
   
  return (frame);
}
