/********************************************************
* CC430 RF Code Example - Asynchronous Transmit
* 
* RF link between two devices uses asynchronous mode - no RF clock. 
* This examples shows the Transmit side of the link, by generating
* 19.2 kHZ square wave (half of the 38.4 kBaud raw RF data rate). 
* The output of the timer is internally connected to the GDO0
* signal of the CC430, since IOCFG0 != 0x2D (IOCFG0 = 0x2E). 
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

  // Internal timer output (10.5 kHz) to Radio TX 
  // NOTE: SMARTF_CC430 IOCFG0 should = 0x2E. When IOCFG0 = 0x2D, 
  // asynchronous data into the radio is taken from GDO0 and not the timer. 
  TA1CCR0 = 50;
  TA1CCR1 = 50;

  TA1CCTL0 = OUTMOD_4;
  TA1CCTL1 = OUTMOD_4;
  TA1CTL = TASSEL__SMCLK + MC_1 + TACLR;
  
  // Init P2.2 to output TX signal to the radio 
  P2SEL |= BIT2;
  P2DIR |= BIT2;
  
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP2 = PM_TA1CCR0A;                     // Map TA output to TX to P2.2
  PMAPPWD = 0x00;                           // Lock Port mapping

  //Transmit the TX waveform asynchronously
  Strobe( RF_SIDLE );
  Strobe( RF_STX );

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
