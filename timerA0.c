#include <msp430.h>

#include "timerA0.h"
/*
 * timerA0.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Marcus Kuhn
 */

/************************************************************************************
  * Function: timerA0Init
  * - Initializes timer A0.
  *
  * Arguments: none
  *
  * return: none
  *
  * Author: Marcus Kuhn
  * Date: 1/6/2021
  * Modified: <date of any mods> usually taken care of by rev control
  ***********************************************************************************/
void timerA0Init(void){

        // clk select: SMCLK | input divider: 1 | mode control: continuous | clear clk | enable interrupts
        TA0CTL = TASSEL_2 | ID_0 | MC_2 | TACLR;
        // expansion clk divider to 1
        TA0EX0 |= TAIDEX_0;
        // Capture on L->H & H->L: 11b | capture mode: 1b | Sync to SMCLK | choose CCI1A
        TA0CCTL1 |= CM0 | CAP | SCS | CCIE;

        // set TA0.0CCI1A to be captured @ P1.2
        P1DIR &= ~BIT2;
        P1SEL |= BIT2;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMERA0_ISR(void){

    numEdges++;
    if(numEdges == 1)
        pulseStart = TA0CCR1;
    else if(numEdges == 2){
//        speedCalc = 1;
        pulseDelta = TA0CCR1 - pulseStart;
        TA0CTL |= TACLR;
        numEdges = 0;
    }
//    if(TA0CCTL1 & COV){
//        pulseDelta = pulseStart - TA0CCR1;
//        TA0CCTL1 &= ~COV;
//    }

    // clear interrupt flags
    TA0CTL &= ~TAIFG;
    TA0CCTL1 &= ~CCIFG;
    TA0IV = 0;
}
