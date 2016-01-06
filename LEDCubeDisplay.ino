//###############################################################################
//# LEDCube - Display Routines                                                  #
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

// General definitions
//====================
const int subframes          =  4; //number of subframes per frame
const int subframeIntervall  = 10; //delay between subframes [multiples of 64us]

// Variables
//==========
//Display buffer
byte      dispBuffer[8]      = {0, 0, 0, 0, 0, 0, 0, 0};
//Skeich buffer
extern byte sketBuffer[];

//Display iterator            7   6   5   4   3   2   1   0
// The display iterator keeps track of the column that is to be shown next
// in the current subframe, as well as the next subframe within the current
// frame:
//        +---+---+---+---+---+---+---+---+
//        | subframe count| column index  |
//        +---+---+---+---+---+---+---+---+
byte       dispIterator      = subframes * columnCount;

//Display handshake flag
//Handshake mechanism:
//    Display driver:                    Application code:
//    ~~~~~~~~~~~~~~~                    ~~~~~~~~~~~~~~~~~
// +-----------+                                  +-----------------+
// |           |                                  |                 |
// |           V                                  V                 |
// |  The content of dispBuffer          New sketBuffer content     |
// |  displayed for the length           is calculated              |
// |  of one full frame                           |                 |
// |           |                                  |                 |
// |           V                                  V                 |
// |  Further subframes of the<--.    .->The application code       |
// |  dispBuffer content are      \  /   waits until distHandshake  |
// |  displayed, until             \/    is TRUE  |                 |
// |  dispHandshake is FALSE       /\             |                 |
// |           |                  /  \            |                 |
// |           V                 /    \           V                 |
// |  The display driver copies_/      \_distHandshake is set to    |
// |  the content of sketBuffer          FALSE    |                 |
// |  into dispBuffer and                         |                 |
// |  acknowledges the buffer                     +-----------------+
// |  update by setting
// |  distHandshake to TRUE
// |           |
// +-----------+
boolean    dispHandshake     = true;

// Setup routine
//==============
void dispSetup() {
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
  OCR2A  = subframeIntervall;               //set interrupt frequency to 1562.5Hz
  TIMSK2 = (1 << OCIE2A);                   //enable output compare A interrupt
}

// Synchronization Routines
//=========================
void dispNextFrame() {
  noInterrupts();                           //disable interrupts
  while (dispHandshake) {                   //wait for next frame
    interrupts();                           //enable interrupts
    noInterrupts();                         //disable interrupts
  }
  dispHandshake  = false;                   //clear frame complete indicator
  interrupts();                             //enable interrupts
}

// Interrupt Service Routine
//==========================
ISR(IMER2_COMPA_vect){                      //timer2 output compare A interrupt
  //Local Variables
  byte    output;

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
