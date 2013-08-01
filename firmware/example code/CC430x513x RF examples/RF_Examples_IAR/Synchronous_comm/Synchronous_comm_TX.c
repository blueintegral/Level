/********************************************************
* CC430 RF Code Example - Synchronous Transmit
* 
* RF link between two devices uses synchronous mode.               
* This examples shows the Transmit side of the link, by generating 
* 19.2 kHZ square wave (half of the 38.4 kBaud raw RF data rate).  
* The square wave is output to pin P2.4, which is jumpered to the GDO0 
* radio input on P2.6. Since IOCFG0 == 0x2D, data is taken from the GDO0
* pin and modulated for the transmission. 
* 
* G. Larmore
* Texas Instruments
* June 2012
* Compiled using IAR v5.40.1 and CCS v5.2
********************************************************/

#include "Synchronous_comm.h"

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

  // Generate timer output (19.2 kHz) for P2.4 to be input back into GDO0. 
  // NOTE: SMARTF_CC430 IOCFG0 should = 0x2D. When IOCFG0 = 0x2E, 
  // synchronous data into the radio is taken from the timer and not GDO0.
  TA1CCR0 = 27;
  TA1CCR1 = 27;

  TA1CCTL1 = OUTMOD_4;
  TA1CTL = TASSEL__SMCLK + MC_1 + TACLR;

  P2SEL |= BIT4+BIT6;                       // P2.4 (TA1 out), P2.6 (GDO0 input)
  P2DIR |= BIT4;                            // P2.4 TA1CCR1 output        
  
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP4 = PM_TA1CCR1A;                     // Map TA1CCR0A output to P2.4
  P2MAP6 = PM_RFGDO0;                       // Map GDO0 as an input on P2.6
  PMAPPWD = 0x00;                           // Lock Port mapping

  Strobe( RF_SIDLE );
  Strobe( RF_STX );

  //Transmit the TX waveform asynchronously
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