#include <msp430.h>

#define BUTTON      BIT2
#define ENABLE      BIT3
#define I1          BIT4
#define I2          BIT5

/**
 * Simple delay function using busy cycles
 * \param[in]   micros  Amount to wait (in kcycles. not microseconds)
 */
void delay(int micro) {
    unsigned int count;
    for (count=0; count<micro * 1000u; count++);
}

/**
 * Simple interrupt function to toggle the ENABLE input of the 
 * L293DNE.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P1OUT ^= ENABLE;            //Toggle enable port of the L293DNE
    TACCTL0 ^= CCIE;              //Toggle CCIE bit in the CCTL0 register ???
    P1IFG &= ~BUTTON;           //Clear the Interrupt FlaG
}

/**
 * Simple interrupt function to toggle input pins of the L293DNE
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= I1 + I2;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    //Pins going to L293DNE to outputs, rest remain as is
    P1DIR |= I1 + I2 + ENABLE;
    P1DIR &= ~BUTTON;           //Set direction to INPUT for button
    P1REN |= BUTTON;            //Enable pullup/pulldown for button
    P1OUT |= BUTTON;            //Select pullup 

    P1IE |= BUTTON;             //Enable interrupt for button
    P1IES |= BUTTON;            //Select falling edge
    P1IFG &= ~BUTTON;           //Clear the Interupt FlaG

    TACTL |= TASSEL_2;          //Select SMCLK (tassel_2)
    TACTL |= MC_2;              //Select continous, count until 0xFFFF
    TACTL |= ID_2;              //Select a clock divider of 4
    //TACTL |= MC_1;            //Select up, count until TACCR0
    //TACCR0 = 30000;           //SMCLK/65535 =~= 15.25hz 

    //Initial state of pins (inverse of each other) allows easy toggling
    P1OUT |= I1;                //enable I1
    P1OUT &= ~I2;               //disable I2

    _BIS_SR(GIE);               //Enable Global Interrupt Enable;

    while(1) { }

    return 1;
}
