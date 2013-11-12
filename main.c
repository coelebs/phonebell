#include <msp430g2211.h>
#include <stdint.h>

#define red_LED   BIT0
#define grn_LED   BIT6

#define I1	  BIT4
#define I2	  BIT5

#define BTN       BIT3

/**
 * Simple delay function using busy cycles
 * \param[in]   micros  Amount to wait (in kcycles. not microseconds)
 */
void delay(int micro) {
    unsigned int count;
    for (count=0; count<micro * 1000; count++);
}

void main(void) {
    uint8_t flash;
    WDTCTL = WDTPW + WDTHOLD;

    P1OUT = 0;
    P1DIR |= I1 + I2;  //Pins going to L293DNE to outputs, rest remain as is

    for (;;) {
        for (flash=0; flash<7; flash++) {
            P1OUT &= ~I2;   //disable I2
            P1OUT |= I1;    //enable I1
            delay(100);
            P1OUT &= ~I1;   //disable I1
            P1OUT |= I2;    //enable I2
            delay(100);
        }
    }
}

