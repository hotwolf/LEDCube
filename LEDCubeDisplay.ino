//###############################################################################
//# LEDCube - Display Driver                                                    #
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

#include <avr/sleep.h>
#include <avr/power.h>
#include "LEDCube.h"

// General definitions
//====================
//Framerate
#define SUBFRAMES       60/FRAMERATE                  //subframes per frame
                                            
//FIFI depth                                
#define FIFODEPTH       16                  //number of FIFO entries
                                            
// Variables                                
//==========                                
//FIFO                                      
ledState        fifo[FIFODEPTH];            //FIFO buffer
byte            fifoIn   = 0;               //index of next free cell in FIFO buffer
byte            fifoOut  = 0;               //index of oldest entry in FIFO buffer
                                            // FIFO is empty if (fifoIn == fifoOut)
//Display state
byte            columnCount   = 0;          //current column (display inactive if (column < 0)
byte            subframeCount = SUBFRAMES;  //current subframe

//Temporary buffers
ledState        currentFrame = 0x0123456789ABCDEF;           //current frame
byte            nextPortD    = 0;           //column buffer 

// Setup routine
//==============
void dispSetup() {
  //Initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  
  //Ports and shift registers
  PORTD =                OE;                //disable shift register outputs
  DDRD  = L3|L2|L1|L0|DS|OE|ST|SH;          //set entire port to output
  for (byte i=0; i < 16; i++) {             //shift in 16 zeros
    PORTD = OE|SH;                          //toggle shift clock
    PORTD = OE;
  }
  PORTD = DS|ST;                            //toggle storage clock, enable outputs, and assert shift input
  PORTD = SH;                               //toggle shift clock
  PORTD = 0;

  //Timer 2
  TCCR2A = (1 << WGM21);                    //CTC mode, no pin toggling
  TCCR2B = (1 << CS22) |                    //set timer clock to 15.625kHz
           (1 << CS21) |
           (1 << CS20);
  OCR2A  = 15625/(FRAMERATE*SUBFRAMES);     //set interrupt frequency
  TIMSK2 = (1 << OCIE2A);                   //enable output compare A interrupt

  //Power modes
  power_timer2_enable();                    //make sure timer2 is powered up
  set_sleep_mode(SLEEP_MODE_IDLE);          //use idle mode
  sleep_enable();                           //enable sleep instruction
}

// Queue frame
//============
void dispQueueFrame(ledState frame) {
  //Copy new frame into FIFO buffer
  fifo[fifoIn] = frame;

  //Start of atomic sequence
  noInterrupts();                           //disable interrupts

  //Wait until there is room in the queue
  while(((fifoOut-fifoIn)%FIFODEPTH) == 1) {//repeat as long as fifo is empty
     wait_for_interrupt();                 //wait for anything to happen
     noInterrupts();                      //disable interrupts
  }

  //Advance in-index
  fifoIn = (fifoIn+1)%FIFODEPTH;            //increment and wrap index

  //End of atomic sequence
  interrupts();                             //enable interrupts
}

// Interrupt Service Routine
//==========================
ISR(TIMER2_COMPA_vect){                     //timer2 output compare A interrupt

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  //Local Variables
  byte    tmpOut;

  //Drive precalculated column pattern
  tmpOut = PORTD;                           //shift column
  tmpOut = (tmpOut & 0xF8) | SH;            // determine shift register inputs first
  PORTD  = tmpOut;                          // then dreive all outputs at once
  PORTD  = nextPortD;                       //drive precalculated column pattern

  //Switch to next column
  columnCount++;                            //increment column counter  
  if (columnCount < COLUMNS) {              //check for end of subframe
    //Determine next column pattern in sub-frame
    nextPortD = ((~(currentFrame >> (4*(columnCount-1))) & 0xF0) | ST);//update column buffer
  } else {       
    //Subframe is complete
    columnCount = 0;                        //reset column counter
    subframeCount--;                        //decrement subframe count

    //Update frame
    if (fifoIn != fifoOut) {                //check if a new frame is in the queue
      if (subframeCount == 1) {             //check for next to last subframe
        //Fade frames on next to last subframe
        currentFrame |= fifo[fifoOut];      //merge current and next frame
      } else if (subframeCount == 0) {      //check for last subframe
        //Switch to next frame
        subframeCount = SUBFRAMES;          //reset subframe counter
        currentFrame = fifo[fifoOut];       //get next frame
        fifoOut = ((fifoOut+1)%FIFODEPTH);  //remove frame from FIFO
      }
    }

    //Determine next column pattern in sub-frame
    nextPortD = ((~(currentFrame << 4) & 0xF0) | ST);//update column buffer

    //assert shift register input
    PORTD |= DS;                              //assert DS
  }

  digitalWrite(13, LOW);   // turn the LED off (LOW is the voltage level)

}
