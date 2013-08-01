/******************************************************************************
* CC430 RF Code Example - TX and RX (variable packet length =< FIFO size)
*
* Simple RF Link to Toggle Receiver's LED by pressing Transmitter's Button    
* Warning: This RF code example is setup to operate at either 868 or 915 MHz, 
* which might be out of allowable range of operation in certain countries.
* The frequency of operation is selectable as an active build configuration
* in the project menu. 
* 
* Please refer to the appropriate legal sources before performing tests with 
* this code example. 
* 
* This code example can be loaded to 2 CC430 devices. Each device will transmit 
* a small packet upon a button pressed. Each device will also toggle its LED 
* upon receiving the packet. 
* 
* The RF packet engine settings specify variable-length-mode with CRC check 
* enabled. The RX packet also appends 2 status bytes regarding CRC check, RSSI 
* and LQI info. For specific register settings please refer to the comments for 
* each register in RfRegSettings.c, the CC430x613x User's Guide, and/or
* SmartRF Studio.
* 
* G. Larmore
* Texas Instruments Inc.
* June 2012
* Built with IAR v5.40.1 and CCS v5.2
******************************************************************************/

#include "RF_Toggle_LED_Demo.h"

#define  PACKET_LEN         (0x10)	    // PACKET_LEN <= 61
#define  RSSI_IDX           (PACKET_LEN+1)  // Index of appended RSSI 
#define  CRC_LQI_IDX        (PACKET_LEN+2)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit 
#define  PATABLE_VAL        (0x51)          // 0 dBm output 

extern RF_SETTINGS rfSettings;

unsigned char packetReceived;
unsigned char packetTransmit; 

unsigned char RxBuffer[64];
unsigned char RxBufferLength = 0;
const unsigned char TxBuffer[11]= {PACKET_LEN, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x42, 0x6F, 0x62};
unsigned char buttonPressed = 0;
unsigned int i = 0; 

unsigned char transmitting = 0; 
unsigned char receiving = 0;

unsigned int counter = 0;

//#define UART_PRINTF

#ifdef UART_PRINTF
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);
#endif

void main( void )
{  
  // Stop watchdog timer to prevent time out reset 
  WDTCTL = WDTPW + WDTHOLD;

  P4DIR |= BIT0;							// P4.0 output
  P4OUT = BIT0;								// P4.0 High

  // initialize clock module
  P1DIR |= 0x01;                            // P1.0 output
  UCSCTL3 |= SELREF__REFOCLK;
  UCSCTL4 |= SELA__REFOCLK;

  // initialize Timer_A module
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 32768;
  TA1CTL = TASSEL__ACLK + MC__UP + TACLR;   // ACLK, up mode, clear TAR

#ifdef UART_PRINTF
  // initialize USCI module
  P1SEL |= BIT5 + BIT6 + BIT7;              // P1.5,6 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL__ACLK;                 // AMCLK
  UCA0BR0 = 27;                             // 32,768kHz 1200 (see User's Guide)
  UCA0BR1 = 0;                              // 32,768kHz 1200
  UCA0MCTL = UCBRS_2;                       // 32,768kHz 1200
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
#endif

  // Increase PMMCOREV level to 2 for proper radio operation
  SetVCore(2);                            
  
  ResetRadioCore();     
  InitRadio();
  InitButtonLeds();
    
  ReceiveOn(); 
  receiving = 1; 
    
  while (1)
  { 
    __bis_SR_register( LPM3_bits + GIE );   
    __no_operation(); 
    
    if (buttonPressed)                      // Process a button press->transmit 
    {
      P3OUT |= BIT6;                        // Pulse LED during Transmit                          
      buttonPressed = 0; 
      P1IFG = 0; 
      
      ReceiveOff();
      receiving = 0; 
      Transmit( (unsigned char*)TxBuffer, sizeof TxBuffer);         
      transmitting = 1;
       
      P1IE |= BIT7;                         // Re-enable button press  
    }
    else if(!transmitting)
    {
      ReceiveOn();      
      receiving = 1; 
    }
  }
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

void Transmit(unsigned char *buffer, unsigned char length)
{
  RF1AIES |= BIT9;                          
  RF1AIFG &= ~BIT9;                         // Clear pending interrupts
  RF1AIE |= BIT9;                           // Enable TX end-of-packet interrupt
  
  WriteBurstReg(RF_TXFIFOWR, buffer, length);     
  
  Strobe( RF_STX );                         // Strobe STX   
}

void ReceiveOn(void)
{  
  RF1AIES |= BIT9;                          // Falling edge of RFIFG9
  RF1AIFG &= ~BIT9;                         // Clear a pending interrupt
  RF1AIE  |= BIT9;                          // Enable the interrupt 
  
  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe( RF_SRX );                      
}

void ReceiveOff(void)
{
  RF1AIE &= ~BIT9;                          // Disable RX interrupts
  RF1AIFG &= ~BIT9;                         // Clear pending IFG

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
  // such that the RXFIFO is empty prior to receiving a packet.
  Strobe( RF_SIDLE );
  Strobe( RF_SFRX  );                       
}

#pragma vector=CC1101_VECTOR
__interrupt void CC1101_ISR(void)
{
  switch(__even_in_range(RF1AIV,32))        // Prioritizing Radio Core Interrupt 
  {
    case  0: break;                         // No RF core interrupt pending                                            
    case  2: break;                         // RFIFG0 
    case  4: break;                         // RFIFG1
    case  6: break;                         // RFIFG2
    case  8: break;                         // RFIFG3
    case 10: break;                         // RFIFG4
    case 12: break;                         // RFIFG5
    case 14: break;                         // RFIFG6          
    case 16: break;                         // RFIFG7
    case 18: break;                         // RFIFG8
    case 20:                                // RFIFG9
      if(receiving)			    // RX end of packet
      {
        // Read the length byte from the FIFO       
        RxBufferLength = ReadSingleReg( RXBYTES );               
        ReadBurstReg(RF_RXFIFORD, RxBuffer, RxBufferLength);
        
        // Stop here to see contents of RxBuffer
        __no_operation();
        
        int i = 0;
        for(i = 0; i < RxBufferLength; i++)
        	printf("%c ", RxBuffer[i]);
        printf("\n");

        // Check the CRC results
        if(RxBuffer[CRC_LQI_IDX] & CRC_OK)
          P1OUT ^= BIT0;                    // Toggle LED1
        else
          printf("Wrong CRC, check Register contents");
      }
      else if(transmitting)		    // TX end of packet
      {
        RF1AIE &= ~BIT9;                    // Disable TX end-of-packet interrupt
        P3OUT &= ~BIT6;                     // Turn off LED after Transmit               
        transmitting = 0; 
      }
      else while(1); 			    // trap 
      break;
    case 22: break;                         // RFIFG10
    case 24: break;                         // RFIFG11
    case 26: break;                         // RFIFG12
    case 28: break;                         // RFIFG13
    case 30: break;                         // RFIFG14
    case 32: break;                         // RFIFG15
  }  
  __bic_SR_register_on_exit(LPM3_bits);     
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  switch(__even_in_range(P1IV, 16))
  {
    case  0: break;
    case  2: break;                         // P1.0 IFG
    case  4: break;                         // P1.1 IFG
    case  6: break;                         // P1.2 IFG
    case  8: break;                         // P1.3 IFG
    case 10: break;                         // P1.4 IFG
    case 12: break;                         // P1.5 IFG
    case 14: break;                         // P1.6 IFG
    case 16:                                // P1.7 IFG
      P1IE = 0;                             // Debounce by disabling buttons
      buttonPressed = 1;
      __bic_SR_register_on_exit(LPM3_bits); // Exit active    
      break;
  }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
  printf("Hello world %d!\r\n", counter++);
  __bic_SR_register_on_exit(LPM3_bits);
}

#ifdef UART_PRINTF
int fputc(int _c, register FILE *_fp)
{
  while(!(UCA0IFG & UCTXIFG));
  UCA0TXBUF = (unsigned char) _c;

  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    while(!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = (unsigned char) _ptr[i];
  }

  return len;
}
#endif
