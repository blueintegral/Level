//Level firmware
//Author: Hunter Scott
//License: GPL v3

#include <msp430.h> 
#include "cc430x513x.h"
#include "IO.H"
#include "ADF4351.H"
#include "radio.h"
//#include "fix_fft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//*****************************************************
//Function definitions
//*****************************************************
void InitSys()
{
	//Set everything up

	//radio stuff
	//P4DIR |= BIT0; // P4.0 output
	//P4OUT = BIT0; // P4.0 High

	// initialize clock module
	P1DIR |= 0x01; // P1.0 output
	UCSCTL3 |= SELREF__REFOCLK;
	UCSCTL4 |= SELA__REFOCLK;

	// initialize Timer_A module
	TA1CCTL0 = CCIE; // CCR0 interrupt enabled
	TA1CCR0 = 32768;
	TA1CTL = TASSEL__ACLK + MC__UP + TACLR; // ACLK, up mode, clear TAR

	#ifdef UART_PRINTF
	// initialize USCI module
	P1SEL |= BIT5 + BIT6 + BIT7; // P1.5,6 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST; // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL__ACLK; // AMCLK
	UCA0BR0 = 27; // 32,768kHz 1200 (see User's Guide)
	UCA0BR1 = 0; // 32,768kHz 1200
	UCA0MCTL = UCBRS_2; // 32,768kHz 1200
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    #endif

}



int main(void) {
	//Set things up
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	MCU_IO_set();
    InitSys();
    delay (20);
    // PLL_set_frq(); //Program VCO

    //Get radio ready for use/////
    SetVCore(2);  // Increase PMMCOREV level to 2 for proper radio operation
    ResetRadioCore();
    InitRadio();
    InitButtonLeds();
    // ReceiveOn();

    /*
     * This section sets up the CC430 as an SPI slave and echos out whatever it recieves from the SPI master device.
     * It uses USCI for this. USCI RX ISR is used to handle communication, CPU normally in LMP4. Before the first data
     * exchange, the master pulses the slave RST for complete reset.
     * ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz
     *
     * If the slave (aka level) is in debug mode, the reset signal from the master will conflict with the slave's
     * JTAG. To work around, use IAR's "Release JTAG on Go" on the slave device. If breakpoints are set in the slave RX
     * ISR, the master must be stopped to avoice overrunning the slave RXBUF.
     *
     */

    //                   CC430F5137
    //                 -----------------
    //            /|\ |                 |
    //             |  |                 |
    //    Master---+->|RST          P1.0|-> LED
    //                |                 |
    //                |             P2.0|-> Data Out (UCA0SIMO)
    //                |                 |
    //                |             P2.2|<- Data In (UCA0SOMI)
    //                |                 |
    //                |             P2.4|-> Serial Clock Out (UCA0CLK)

    while(!(P2IN&BIT4));                      // If clock sig from mstr stays low,
                                               // it is not yet in SPI mode

     PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
     P2MAP0 = PM_UCA0SIMO;                     // Map UCA0SIMO output to P2.0
     P2MAP2 = PM_UCA0SOMI;                     // Map UCA0SOMI output to P2.2
     P2MAP4 = PM_UCA0CLK;                      // Map UCA0CLK output to P2.4
     PMAPPWD = 0;                              // Lock port mapping registers

     P2DIR |= BIT0 + BIT2 + BIT4;              // ACLK, MCLK, SMCLK set out to pins
     P2SEL |= BIT0 + BIT2 + BIT4;              // P2.0,2,4 for debugging purposes

     UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
     UCA0CTL0 |= UCSYNC+UCCKPL+UCMSB;          // 3-pin, 8-bit SPI slave,
                                               // Clock polarity high, MSB
     UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
     UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

     __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts





    //Set RF switches to stun. I mean, transmit.
    //   P1OUT |= BIT1; //Set P1.1 high (GDO2)



	return 0;
}


// Echo character
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = UCA0RXBUF;
      break;
    case 4:break;                             // Vector 4 - TXIFG
    default: break;
  }
}

