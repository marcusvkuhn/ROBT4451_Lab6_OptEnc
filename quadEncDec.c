#include <msp430.h>
#include <stdio.h>

#include "quadEncDec.h"
#include "usciUart.h"
#include "usciSpi.h"
#include "timerA0.h"

/*
 * qEncDec.c
 *
 *  Created on: Mar 25, 2021
 *      Author: A01064898
 */

/************************************************************************************
  * Function: initEncDec
  * - Initializes port 2 for encoder channels signal capture.
  * - Configures initial edge detection and enables port interrupts
  *
  * Arguments: none
  *
  * return: none
  *
  * Author: Marcus Kuhn
  * Date: 3/29/2021
  * Modified: <date of any mods> usually taken care of by rev control
  ***********************************************************************************/
void initEncDec(void){

    P2DIR &= ~(CH_A | CH_B | CH_I | RST);    // set P2.4/5/0 as input

    P2OUT |= RST;                            // choose pull-up resistor
    P2REN |= RST;                            // enable resistor

    currState = (P2IN & (CH_A + CH_B));      // read enc channels A and B

    P2IES = currState + RST;                 // the next edge setup is always equal to the current state, keep RST edge as H-->L

    P2IE |= (CH_A | CH_B | RST);             // enable interrupts on both channels and rest pushbutton

    P2IFG = 0;                               // clear interrupt flags
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {

    switch(__even_in_range(P2IV,0xF)) {
        case 0x04: posCount = 0;            // P2.1 Interrupt, reset posCount
            break;
        case 0x0A:                          // P2.4 Interrupt, no break here, case A and C share the same code
        case 0x0C:                          // P2.5 Interrupt

            if (posCount > 4600 || posCount < -4600)
                    posCount = 0;
            //update previous state
            prevState = currState;
            //update current state, right shift
            currState = (P2IN & (CH_A + CH_B)) >> 4;
            // update previous CH_A state by anding with BIT1 of prevState
            prevA = (prevState & A_BIT) >> 1;
            // update new CH_B state by anding with BIT0 of currState,
            currB = (currState & B_BIT);

            // XOR previous A with current B
            dir = prevA ^ currB;

            if (dir)  // if the result is true, motor is moving in the CCW direction
                posCount--;
            else if (!dir)                // if not, its moving CW
                posCount++;

            // the next edge setup is always equal to the current state, keep RST edge as H-->L
            P2IES = (currState << 4) + RST;
            break;
        default: break;
    }
    // enable screen write
    screenWrite = 1;
    P2IFG = 0; // clear interrupt flag after its serviced
}

void testEncoder(void){

    char qEncStateCW[8] = {0x2, 0x3, 0x1, 0x0, 0x2, 0x3, 0x1, 0x0}; //CW
    char qEncStateCCW[8] = {0x1, 0x3, 0x2, 0x0, 0x1, 0x3, 0x2, 0x0}; //CWW

    unsigned int var;
    for (var = 0; var < 8; ++var)
        P3OUT = qEncStateCW[var];
    for (var = 0; var < 8; ++var)
        P3OUT = qEncStateCCW[var];
}
