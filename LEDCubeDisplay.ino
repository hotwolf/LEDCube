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
//# LED pattern format (unsigned 64-bit integer):                               #
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
#define FRAMERATE	15                  //frames per second 	
#define SUBFRAMES	 4                  //subframes per frame
			                    
//FIFI depth		                    
#define FIFIDEPTH	16                  //number of FIFO entries
				            
// Variables			            
//==========			            
//FIFO				            
ledState	fifo[FIFODEPTH];            //FIFO buffer
unsigned char	fifoIn   = 0;               //index of next free cell in FIFO buffer
unsigned char   fifoOut  = 0;               //index of oldest entry in FIFO buffer
                                            // FIFO is empty if (fifoIn == fifoOut)
//Display state
signed char	column   = -1;              //current column (display inactive if (column < 0)
signed char	subframe = SUBFRAMES;       //current subframe

//Column buffer
unsigned char   colbuf   = 0;               //column buffer 

// Setup routine
//==============
void displaySetup() {
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
  power_timer2_disable();                   //make sure timer2 is powered up
  set_sleep_mode(SLEEP_MODE_IDLE);          //use idle mode
  sleep_enable();                           //enable sleep instruction
}

// Queue frame
//============
void queueFrame(ledState frame) {
  //Copy new frame into FIFO buffer
  fifo[fifoIn] = frame;

  //Start of atomic sequence
  noInterrupts();                           //disable interrupts

  //Wait until there is room in the queue
  while(((fifoOut-fifoIn)%FIFIDEPTH) == 1) {//repeat as long as fifo is empty
     wait_for_interrupts();                 //wait for anything to happen
     noInterrrupts();                       //disable interrupts
  }

  //Advance in-index
  fifoIn = (fifoIn+1)%FIFIDEPTH;            //increment and wrap index

  //End of atomic sequence
  interrupts();                             //enable interrupts
}

// Interrupt Service Routine
//==========================
ISR(TIMER2_COMPA_vect){                     //timer2 output compare A interrupt
  //Local Variables
  byte    output;

  if (column >= 0) {                        //check if display is active
     //Display is active



  } else {
    //Display is inactive
     if (fifoIn != fifoOut) {               //check if frame is queued 
       //Activate display
       subframe = SUBFRAMES;                //reset subframe count
       column   = 0;                        //reset column counter                 
       colbuf   = (fifo[fifoOut]&0x0F)<<4   //prepare columnbuffer










  }






  
  //Drive column pattern
  if (dispIterator & 1) {
    //Odd column
    output = dispBuffer[(dispIterator & (columnCount - 1)) >> 1] << 6;   //set column pattrern
  } else {
    //Even column
    output = dispBuffer[(dispIterator & (columnCount - 1)) >> 1] & 0xF0; //set column pattrern
  }
  output |= ST;                                                          //toggle storage clock
  PORTD   = output;                                                      //drive output
  output &= L3|L2|L1|L0;                                                 //clear storage clock

  //Advance iterator
  dispIterator++;
  if (dispIterator == 0) {
    dispIterator = -columnCount;                                         //saturate subframe count
  }

  //Restart shift sequence after subframe
  if (!(dispIterator & (columnCount - 1))) {
    //Restart shift sequence
    output |= DS;                                                        //assert shift input
    PORTD   = output;                                                    //drive output

    if ((!dispHandshake) &&                                              //sketBuffer is ready
        (dispIterator > (subframes * columnCount ))) {                   //frame is complete

      //Copy buffer content
      dispBuffer[0] = sketBuffer[0];
      dispBuffer[1] = sketBuffer[1];
      dispBuffer[2] = sketBuffer[2];
      dispBuffer[3] = sketBuffer[3];
      dispBuffer[4] = sketBuffer[4];
      dispBuffer[5] = sketBuffer[5];
      dispBuffer[6] = sketBuffer[6];
      dispBuffer[7] = sketBuffer[7];

      //Signal frame completion
      dispHandshake  = true;                                             //mark frame complete
    }
  }

  //Advance shift registers
  output |= SH;                                                          //toggle shift clock clock
  PORTD   = output;                                                      //drive output
}
