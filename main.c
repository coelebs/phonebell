#include <msp430g2211.h>
#include <stdint.h>

#define red_LED     BIT0
#define grn_LED     BIT6

#define BUTTON      BIT2
#define ENABLE      BIT3
#define I1	        BIT4
#define I2	        BIT5


/**
 * Simple delay function using busy cycles
 * \param[in]   micros  Amount to wait (in kcycles. not microseconds)
 */
void delay(int micro) {
    unsigned int count;
    for (count=0; count<micro * 1000; count++);
}

int main(void) {
    uint8_t flash = 1, input = 0;
    WDTCTL = WDTPW + WDTHOLD;

    P1OUT = 0;

    //Pins going to L293DNE to outputs, rest remain as is
    P1DIR |= I1 + I2 + ENABLE;  
    P1DIR &= ~BUTTON;
    P1OUT &= ~BUTTON;
    P1REN |= BUTTON;
     
    while(1) {
        input = P1IN & BUTTON;

        if(input) {
            P1OUT |= ENABLE;
        } else {
            P1OUT &= ~ENABLE;
        }

        P1OUT &= ~I2;   //disable I2
        P1OUT |= I1;    //enable I1
        delay(5);
        P1OUT &= ~I1;   //disable I1
        P1OUT |= I2;    //enable I2
        delay(5);

    }
    return 1;
}
