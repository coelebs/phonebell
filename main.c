#include <msp430g2553.h>
#include <stdint.h>
#include <stdbool.h>

#define BUTTON      BIT2
#define ENABLE      BIT3
#define I1          BIT4

#define LED1        BIT0
#define LED2        BIT6

static uint32_t seconds;
static bool half;

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
    TA0CCTL0 ^= CCIE;            //Toggle CCIE bit in the CCTL0 register ???

    P1IFG &= ~BUTTON;           //Clear the Interrupt FlaG
}

/**
 * Simple interrupt function to toggle input pins of the L293DNE
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= I1;

    TA0CCTL0 &= ~CCIFG;
}

/**
 * Simple interrupt function to toggle input pins of the L293DNE
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void IncrementClock(void) {
    seconds++;

    TA1CCTL0 &= ~CCIFG;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    seconds = 0;
    half = false;

    //Pins going to L293DNE to outputs, rest remain as is
    P1DIR |= I1 + ENABLE;

    P1DIR &= ~BUTTON;           //Set direction to INPUT for button
    P1REN |= BUTTON;            //Enable pullup/pulldown for button
    P1OUT |= BUTTON;            //Select pullup 

    P1IE |= BUTTON;             //Enable interrupt for button
    P1IES |= BUTTON;            //Select falling edge
    P1IFG &= ~BUTTON;           //Clear the Interupt FlaG

    TA0CTL |= TASSEL_2;         //Select SMCLK (tassel_2)
    TA0CTL |= MC_2;             //Select continous, count until TACCRx
    TA0CTL |= ID_1;             //Select a clock divider of 8

    TA1CTL |= TASSEL_2;         //Select SMCLK (tassel_2)
    TA1CTL |= MC_1;             //Select continous, count until TACCRx
    TA1CTL |= ID_1;             //Select a clock divider of 4

    TA1CCR0= 50000;

    TA0CCTL0 |= OUTMOD_7;       //Select OUTMODE? and don't enable interrupt yet
    TA1CCTL0 |= CCIE + OUTMOD_7;//Select OUTMODE? and start counting immediate  

    //Initial state of pins (inverse of each other) allows easy toggling
    P1OUT |= I1;                //enable I1
    P1OUT &= ~ENABLE;

    _BIS_SR(GIE);               //Enable Global Interrupt Enable;

    while(1) { }

    return 1;
}
