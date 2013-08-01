/********************************************************
* CC430 RF Code Example - Asynchronous Receive
* 
* RF link between two devices uses asynchronous mode. 
* This examples shows the Receive side of the link, by outputting the 
* demodulated RF signal on GDO0 to P2.6 for analysis. In this example, the 
* signal is a 19.2 kHZ square wave (half of the 38.4 kBaud raw RF data rate). 
* 
* G. Larmore
* Texas Instruments
* June 2012
* Compiled using IAR v5.40.1 and CCS v5.2
********************************************************/

#include "Asynchronous_comm.h"

#define  PATABLE_VAL        (0x51)          // 0 dBm output 

extern RF_SETTINGS rfSettings;

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  // Increase PMMCOREV level to 2 in order to avoid low voltage error
  // when the RF core is enabled
  SetVCore(2);

  InitRadio();
  InitButtonLeds();
  
  Strobe( RF_SIDLE );
  Strobe(RF_SRX); 

  P2DIR |= BIT6;            // P2.6 output
  P2SEL |= BIT6;            // P2.6 select

  PMAPPWD = 0x02D52;             // Get write-access to port mapping regs
  P2MAP6 = PM_RFGDO0;            // Map GDO0 to P2.6   
  PMAPPWD = 0x00;                // Lock Port mapping  

  // Enable timer interrupts to see if input is visible
  TA1CTL = TASSEL__SMCLK + MC_2 + TACLR;
  TA1CCTL0 = CM_3 + CCIS_1 + CAP + CCIE;

  __bis_SR_register(GIE);

  while(1);
}

void InitButtonLeds(void)
{
  // Initialize Port J
  PJOUT = 0x00;
  PJDIR = 0xFF;

  // Set up LEDs
  P1OUT &= ~BIT0;
  P1DIR |= BIT0;
  P3OUT &= ~BIT6;
  P3DIR |= BIT6;
}

void InitRadio(void)
{
  // Set the High-Power Mode Request Enable bit so LPM3 can be entered
  // with active radio enabled 
  PMMCTL0_H = 0xA5;
  PMMCTL0_L |= PMMHPMRE_L; 
  PMMCTL0_H = 0x00; 
  
  WriteRfSettings(&rfSettings);
  
  WriteSinglePATable(PATABLE_VAL);
}

// Set breakpoint sense asynchronous data
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  // Will see the f_GDO on the LED1 
  P1OUT ^= BIT0;          // Toggle LED
}