#include <msp430g2211.h>
#include <stdint.h>

#define red_LED   BIT0
#define grn_LED   BIT6

#define I1	  BIT4
#define I2	  BIT5

#define BTN       BIT3

void delay(int microsecs);

void main(void) {
    uint8_t flash;
    WDTCTL = WDTPW + WDTHOLD;

    P1OUT = 0;
    P1DIR |= I1 + I2;  // LED pins to outputs, BTN is

    for (;;) {

        for (flash=0; flash<7; flash++) {
            P1OUT &= ~I2;
            P1OUT |= I1;    // red LED on
            delay(1000);             // call delay function
            P1OUT &= ~I1;   // red LED off
            P1OUT |= I2;    // red LED on
            delay(1000);             // delay again
        }
    }
} // main

void delay(int micro) {
    unsigned int count;
    for (count=0; count<micro * 100; count++);
} // delay
