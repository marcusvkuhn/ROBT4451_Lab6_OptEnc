/*
 * qEncDec.h
 *
 *  Created on: Mar 25, 2021
 *      Author: A01064898
 */

#ifndef QUADENCDEC_H_
#define QUADENCDEC_H_

//-- func prototypes
void initEncDec(void);
void testEncoder(void);

//-- macros
#define     CH_A        BIT4
#define     CH_B        BIT5
#define     RST         BIT1
#define     CH_I        BIT0

#define     A_BIT       BIT1
#define     B_BIT       BIT0

//-- global vars
int posCount;

volatile unsigned char prevState,
                       currState,
                       prevA,
                       currB,
                       dir;

int  screenWrite;

#endif /* QUADENCDEC_H_ */
