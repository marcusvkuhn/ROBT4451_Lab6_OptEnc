#include <msp430.h> 
#include <stdio.h>

#include "quadEncDec.h"

#include "usciUart.h"
#include "usciSpi.h"
#include "timerA0.h"

/**
 * main.c
 *
 * main client file for ROBT4451 Lab 6
 *
 */

void main(void){

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// encoder sim outputs
//    P3DIR |= BIT0 | BIT1;
//    P3OUT = 0;

    // init UART interface
    usciA1UartInit();

	// init encoder decode
	initEncDec();

	// init timerA
	timerA0Init();

	// init SPI interface
	usciB1SpiInit(1,1,0x02,0);


	//testEncoder();
    int clrLenght;

    char posStr[8];

    numEdges = 0;
    char speedStr[8];
    double angSpeed;
    double angSpeedNum;
    double angSpeedDen;

    angSpeedNum = (SMCLK*2*M_PI)*0.01;


    // enable global interrupts (SR.GIE bit)
    __enable_interrupt();

	while(1){

	    __delay_cycles(104800);

	    if (screenWrite == 1){
	        for (clrLenght = 40; clrLenght != 0; --clrLenght)
	            usciA1UartTxChar(' ');                          // this loop clears the console

            usciA1UartTxChar('\r');                         // returns to beginning of line

            angSpeedDen = (pulseDelta*0.01) * 1150;
            angSpeed = angSpeedNum / angSpeedDen;
//          angSpeed = (SMCLK*2*M_PI)/(pulseDelta*P_REV_Q);
            sprintf(speedStr, "posCount = %d, angSpeed = %.2f", posCount, angSpeed);
            usciA1UartTxString(speedStr);
            speedCalc = 0;

            //            usciB1SpiTxBuffer(&posCount, 2);

            screenWrite = 0;                                // reset enabler to write data to screen
        }
	}
}
