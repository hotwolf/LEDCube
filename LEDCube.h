#ifndef LEDCUBE_H
#define LEDCUBE_H
//###############################################################################
//# LEDCube - Header file                                                       #
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
//#    March 3, 2016                                                            #
//#      - Initial release                                                      #
//###############################################################################

// Type definitions
//=================
typedef unsigned long long ledState;

// General definitions
//====================
#define LEVELS             4
#define COLUMNS            (LEVELS * LEVELS)

// Pin definitions
//================
#define PL3                7    //level 3                   7 (PD7)
#define PL2                6    //level 2                   6 (PD6)
#define PL1                5    //level 1                   5 (PD5)
#define PL0                4    //level 0                   4 (PD4)
                                
#define PDS                3    //serial data input         3 (PD3)
#define POE                2    //output enable, active low 2 (PD2)
#define PST                1    //storage clock             1 (PD1)
#define PSH                0    //shift clock               0 (PD0)

// Bit definitions
//================
#define L3                  (1 << PL3)
#define L2                  (1 << PL2)
#define L1                  (1 << PL1)
#define L0                  (1 << PL0)
                           
#define DS                  (1 << PDS)
#define OE                  (1 << POE)
#define ST                  (1 << PST)
#define SH                  (1 << PSH)

// LED patterns
//=============
#define LED_STATE_MASK      ((1 << (LEVELS * COLUMNS)) - 1)
#define LED_STATE_ALL_ON    LED_STATE_MASK 
#define LED_STATE_ALL_OFF   0
#define LED_STATE_LEVEL_0   (LED_STATE_MASK & (((((((((((((((((((((((((((((((1  << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS) |\
                                                                             1) << LEVELS))
#define LED_STATE_LEVEL_1   (LED_STATE_LEVEL_0 << 1)
#define LED_STATE_LEVEL_2   (LED_STATE_LEVEL_0 << 2)
#define LED_STATE_LEVEL_3   (LED_STATE_LEVEL_0 << 3)
#define LED_STATE_SLICE_Y0  (LED_STATE_MASK & ((((((((((((1 << LEVELS) - 1)  << (LEVELS * LEVELS)) | \
                                                         (1 << LEVELS) - 1)) << (LEVELS * LEVELS)) | \
                                                         (1 << LEVELS) - 1)) << (LEVELS * LEVELS)) | \
                                                         (1 << LEVELS) - 1)) << (LEVELS * LEVELS)))


// Macro definitions
//==================
//Wait for any interrupt
#define wait_for_interrupt()                     \
do {                                             \
  __asm__ __volatile__ ( "sei" "\n\t" :: );      \
  __asm__ __volatile__ ( "sleep" "\n\t" :: );    \
} while(0)

#endif

