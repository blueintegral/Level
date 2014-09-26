//Level firmware
//Author: Hunter Scott
//License: MIT

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

	unsigned char TxBuffer[PACKET_LEN]= {
	    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
	    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
	};

	unsigned char RxBuffer[PACKET_LEN+2] = {0};


	//Set things up
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	MCU_IO_set();
    InitSys();
    delay (20);
    // PLL_set_frq(); //Program VCO

    //Get radio ready for use
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
     * JTAG. To work around, use IAR's "Release JTAG on Go" or CCS's "Run Free" on the slave device. If breakpoints are set in the slave RX
     * ISR, the master must be stopped to avoice overrunning the slave RXBUF.
     *
     */

    //                   CC430F5137
    //                 -----------------
    //            /|\ |                 |
    //             |  |                 |
    //    Master---+->|RST          P2.2|-> LED
    //                |                 |
    //                |             P1.4|-> Data Out (UCA0SIMO)
    //                |                 |
    //                |             P2.7|<- Data In (UCA0SOMI)
    //                |                 |
    //                |             P1.3|-> Serial Clock Out (UCA0CLK)

    while(!(P1IN&BIT3));                      // If clock sig from mstr stays low,
                                               // it is not yet in SPI mode

     PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
     P1MAP4 = PM_UCA0SIMO;                     // Map UCA0SIMO output to P1.4
     P2MAP7 = PM_UCA0SOMI;                     // Map UCA0SOMI output to P2.7
     P1MAP3 = PM_UCA0CLK;                      // Map UCA0CLK output to P1.3
     PMAPPWD = 0;                              // Lock port mapping registers

     P3DIR |= BIT0 + BIT1 + BIT2;              // ACLK, MCLK, SMCLK set out to pins
     P3SEL |= BIT0 + BIT1 + BIT2;              // P3.0,1,2 for debugging purposes

     UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
     UCA0CTL0 |= UCSYNC+UCCKPL+UCMSB;          // 3-pin, 8-bit SPI slave,
                                               // Clock polarity high, MSB
     UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
     UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

     __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts


     //Transmit some junk
     set_frequency(512000000); //Transmit at 512 MHz
     transmit_now();

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
  printf("Got an interrupt\r\n");
  switch(__even_in_range(UCA0IV,4))
  {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
      while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
      UCA0TXBUF = UCA0RXBUF;
      printf("Sent a byte\r\n");
      break;
    case 4:break;                             // Vector 4 - TXIFG
    default: break;
  }
}


