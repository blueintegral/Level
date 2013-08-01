
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

  //add P2.0 for GD02 - clock sync to TXDATA
  P2DIR |= BIT0;            // P2.0 output
  P2SEL |= BIT0;            // P2.6 select

  //map P1.0 to GDO1 - high if RSSI above threshold
  P1DIR |= BIT0;            // P1.0 output
  P1SEL |= BIT0;            // P1. select

  PMAPPWD = 0x02D52;             // Get write-access to port mapping regs
  P2MAP6 = PM_RFGDO0;            // Map GDO0 to P2.6   
//  P2MAP0 = PM_RFGDO2;            // Map GDO2 to P2.0   
//  P1MAP0 = PM_RFGDO1;            // Map GDO1 to P1.0   
  PMAPPWD = 0x00;                // Lock Port mapping  

  // Enable timer interrupts to see if input is visible
  TA1CTL = TASSEL__SMCLK + MC_2 + TACLR;
  TA1CCTL0 = CM_3 + CCIS_1 + CAP + CCIE;

  __bis_SR_register(GIE);

  while(1);
#elif defined TEST_ASYNC_TX_MODE

  // Should serve as internal timer output (toggle rate 19.2 kHz)
  // to Radio TX input when IOCFG0 = 0x2E (high-Z)
  // NOTE: SMARTF_CC430 IOCFG0 should = 0x2E so asynchronous signal is taken from
  // the timer and not GDO0


//  TA1CCR0 = 27;
//  TA1CCR1 = 27;

  TA1CCR0 = 26;
  TA1CCR1 = 26;


  TA1CCTL0 = OUTMOD_4;
  TA1CCTL1 = OUTMOD_4;
  TA1CTL = TASSEL__SMCLK + MC_1 + TACLR;

  P2SEL |= BIT2;
  P2DIR |= BIT2;
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP2 = PM_TA1CCR0A;                       // Map CBOUT output to P2.2
  PMAPPWD = 0x00;                           // Lock Port mapping

  Strobe( RF_SIDLE );
  Strobe( RF_STX );

  //Transmit the TX waveform asynchronously
  while(1);


 #endif

  // ************NOT USED IN THIS LAB*****************************
  // Using function generator to generate asynchronous TX signal

  /*

  P2OUT |= BIT0;
  P2DIR |= BIT0;
  P2SEL |= BIT6;

  //add P2.0 for GD02 - clock sync to TXDATA
  P2DIR |= BIT0;            // P2.0 output
  P2SEL |= BIT0;            // P2.6 select


  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P2MAP6 = PM_RFGDO0;                       // Map CBOUT output to P1.6

  P2MAP0 = PM_RFGDO2;            // Map GDO2 to P2.0


  PMAPPWD = 0x00;                           // Lock Port mapping
  // NOTE: SMARTRF_CC430 IOCFG0 shoud = 0x2D so asynchronous signal is taken from
  // GDO0 and not the internal timer connection

  */

}

void InitButtonLeds(void)
{
  // Set up the button as interruptible
  P1DIR &= ~BIT7;
  P1REN |= BIT7;
  P1IES &= BIT7;
  P1IFG = 0;
  P1OUT |= BIT7;
  P1IE  |= BIT7;

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
  //P1OUT ^= BIT0;          // Toggle LED
  P1OUT |= BIT0;          // Toggle LED
}

// Set breakpoint to see RF clock input in synchronous mode
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  switch(__even_in_range(TA0IV,14))
  {
  case 0: break;
  case 2: break;
  case 4: break;
  case 6: break;
  case 8:
    P1OUT ^= BIT0;
    if(TA0CCTL3 & CCI)
      P1OUT |= BIT6;
    else
      P1OUT &= ~BIT6;
    break;
  case 10: break;
  case 12: break;
  case 14: break;
  }
}