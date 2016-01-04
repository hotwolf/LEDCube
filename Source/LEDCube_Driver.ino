//###############################################################################
//# LEDCube - Driver                                                            #
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

// Pin definitions
//================
const byte L3                = 7;        //level 3                   7 (PD7)
const byte L2                = 6;        //level 2                   6 (PD6)
const byte L1                = 5;        //level 1                   5 (PD5)
const byte L0                = 4;        //level 0                   4 (PD4)

const byte DS                = 3;        //serial data input         3 (PD3)
const byte OE                = 2;        //output enable, active low 2 (PD2)
const byte ST                = 1;        //storage clock             1 (PD1)
const byte SH                = 0;        //shift clock               0 (PD0)

const byte drvLevelMask      = (1 << L3) | (1 << L2) | (1 << L1) | (1 << L0);


// Variables
//==========
//Subframe counter              7   6   5   4   3   2   1   0
//  For every frame, each    +---+---+---+---+---+---+---+---+
//  column must be shown     |iteration count| column index  |
//  4 times.                 +---+---+---+---+---+---+---+---+
byte       drvSubframeCount  = 0;
const byte drvColumnMask     = (1 << 4) - 1;

//Display buffer
byte       drvBuffer[16]     = {0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0};

//Frame complete indicator
boolean    drvFrameComplete  =  false;

// Setup Routines
//===============
void setupDriver() {
     //Ports and Shift registers
     PORTD = (1 << OE);                  //disable shift register outputs
     DDRD  = -1;                         //set entire port to output
     for (byte i=0; i < 16; i++) {       //shift in 16 zeros
       PORTD = (1 << OE) | (1 << SH);    //toggle shift clock
       PORTD = (1 << OE);
    }
    PORTD = (1 << ST)| (1 << DS);        //toggle storage clock, enable outputs, and assert shift input
    PORTD = (1 << SH);                   //toggle shift clock
    PORTD = 0;

    // Variables
    drvSubframeCount  = 0;               //subframe counter
    drvBuffer[]       = {0, 0, 0, 0,     //display buffer
                         0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};
    drvFrameComplete  = false;          //frame complete indicator

    //Timer 2
     TCCR2A = (1 << WGM21);              //CTC mode, no pin toggling
     TCCR2B = (1 << CS22) |              //set timer clock to 15.625kHz
              (1 << CS21) |
              (1 << CS20);
     OCR2A  = 10;                        //set interrupt frequency to 1562.5Hz
     TIMSK2 = (1 << OCIE2A);             //enable output compare A interrupt
}

// Synchronization Routines
//=========================
void nextFrame() {
     noInterrupts();                     //disable interrupts
     while (drvFrameComplete) {          //wait for next frame
           interrupts();                 //enable interrupts
           noInterrupts();               //disable interrupts
     }
     drvFrameComplete  = false;          //clear frame complete indicator
     interrupts();                       //enable interrupts
}

// Interrupt Service Routine
//==========================
ISR(IMER2_COMPA_vect){                  //timer2 output compare A interrupt
{
        //Local Variables
        byte    outputPattern;

        //Drive column pattern
        outputPattern  = drvBuffer[drvSubframeCount & drvColumnMask] & drvLevelMask); //set column pattrern
        outputPattern |= (1 << ST);                                                   //toggle storage clock
        PORTD          = outputPattern;                                               //drive output
        outputPattern &= drvLevelMask;                                                //clear storage clock

        //Increment and check subframe counter
        drvSubframeCount++;

        if (! (drvSubframeCount & drvColumnMask)) {
           //Restart shift sequence
           outputPattern    |= (1 << DS);                                             //asert shift input
           PORTD             = outputPattern;                                         //drive output

           if (drvSubframeCount > 3*16) {                                             //all iterations complete
              if {drvFrameComplete} (                                                 //next buffer is not ready
                 drvSubframeCount -= 16                                               //repeat last iteration
              } else {
                 //Reset subframe counter
                 drvSubframeCount  = 0;

                 //Copy buffer content
                 for (byte i = 0; i < 16; i++) {
                     drvBuffer[i] = buffer[i];
                 }

                 //Set frame complete indicator
                 drvFrameComplete  = true;                                            //mark frame complete
              }
           }
        }

        //Advance shift registers
        outputPattern |= (1 << SH);                                                   //toggle shift clock clock
        PORTD          = outputPattern;                                               //drive output
}

