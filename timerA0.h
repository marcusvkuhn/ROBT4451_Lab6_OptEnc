/*
 * timerA0.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Marcus Kuhn
 */

#ifndef TIMERA0_H_
#define TIMERA0_H_

#define SMCLK 1048000
#define P_REV_Q 4600
#define P_REV_NQ 1150
#define M_PI 3.14159
#define N_GB 11.5

void timerA0Init(void);

long int speedCalc,
    numEdges,
    pulseStart,
    pulseDelta;


#endif /* TIMERA0_H_ */
