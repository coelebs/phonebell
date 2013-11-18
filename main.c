#include <msp430g2553.h>
#include <stdint.h>
#include <stdbool.h>

#define red_LED     BIT0
#define grn_LED     BIT6

#define DELAY       200

#define BUTTON      BIT2
#define ENABLE      BIT3
#define I1	        BIT4
#define I2	        BIT5

bool input = true;

/**
 * Simple delay function using busy cycles
 * \param[in]   micros  Amount to wait (in kcycles. not microseconds)
 */
void delay(int micro) {
    unsigned int count;
    for (count=0; count<micro * 1000; count++);
}

/**
 * Simple interrupt function to toggle the ENABLE input of the 
 * L293DNE.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P1OUT ^= ENABLE;            //Toggle enable port of the L293DNE
    CCTL0 ^= CCIE;              //Toggle CCIE bit in the CCTL0 register ???
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

    P1OUT &= 0;

    //Pins going to L293DNE to outputs, rest remain as is
    P1DIR |= grn_LED + I1 + I2 + ENABLE;
    P1DIR &= ~BUTTON;           //Set direction to INPUT for button
    P1REN |= BUTTON;            //Enable pullup/pulldown for button
    P1OUT |= BUTTON;            //Select pullup 

    P1IE |= BUTTON;             //Enable interrupt for button
    P1IES |= BUTTON;            //Select falling edge
    P1IFG &= ~BUTTON;           //Clear the Interupt FlaG

    //Select SMCLK (tassel_2)
    //Select UP mode, so the number counts up (MC_1)
    TACTL = TASSEL_2 + MC_1;
    CCR0 = 65535;               //SMCLK/65535 =~= 15.25hz 

    //Initial state of pins (inverse of each other) allows easy toggling
    P1OUT |= I1;                //enable I1
    P1OUT &= ~I2;               //disable I2

    _BIS_SR(GIE);               //Enable Global Interrupt Enable;

    while(1) {
    }

    return 1;
}
