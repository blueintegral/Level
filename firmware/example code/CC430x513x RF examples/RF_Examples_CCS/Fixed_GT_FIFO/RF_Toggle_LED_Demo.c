/******************************************************************************
* CC430 RF Code Example - TX and RX (fixed-size packet length > FIFO size)
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
* a 100 byte packet upon a button pressed. Each device will also toggle its LED 
* upon receiving the packet(s). 
* 
* The RF packet engine settings specify variable-length-mode with CRC check 
* enabled. The RX packet also appends 2 status bytes regarding CRC check, RSSI 
* and LQI info. For specific register settings please refer to the comments for 
* each register in RfRegSettings.c, the CC430x513x User's Guide, and/or 
* SmartRF Studio.
* 
* G. Larmore
* Texas Instruments Inc.
* June 2012
* Built with IAR v5.40.1 and CCS v5.2
******************************************************************************/

#include "RF_Toggle_LED_Demo.h"

extern RF_SETTINGS rfSettings;

unsigned char packetReceived;
unsigned char packetTransmit; 

unsigned char txBytesLeft = PACKET_LEN;           // +1 for length byte 
unsigned char txPosition = 0; 
unsigned char rxBytesLeft = PACKET_LEN+2;         // +2 for status bytes
unsigned char rxPosition = 0;
unsigned char lengthByteRead = 0; 

unsigned char RxBufferLength = 0;
unsigned char TxBufferLength = 0; 
unsigned char * _p_Buffer = 0; 
unsigned char buttonPressed = 0;
unsigned int i = 0;                 

unsigned char transmitting = 0; 
unsigned char receiving = 0; 

unsigned char RxBuffer[PACKET_LEN+2] = {0};
//unsigned char TxBuffer[PACKET_LEN]= {
//    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
//    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
//    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
//    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
//    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
//    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
//    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
//    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
//    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
//    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
//};

unsigned char TxBuffer[PACKET_LEN]= {
		0x15, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x42, 0x6F, 0x62,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


void main( void )
{  
  // Stop watchdog timer to prevent time out reset 
  WDTCTL = WDTPW + WDTHOLD; 
  // Increase PMMCOREV level to 2 for proper radio operation
  SetVCore(2);                               
  
  ResetRadioCore();
  InitButtonLeds();
  InitTimer(); 
    
  // Clean out the RX Buffer 
  rxPosition = PACKET_LEN+2;
  while(rxPosition--)
  {
    RxBuffer[rxPosition] = 0; 
  }

  InitRadio();  
  ReceiveOn(); 
//Check RSSI here
  while (1)
  { 
    P1IE |= BIT7;                           // Enable button interrupt
    
    __bis_SR_register( LPM3_bits + GIE );   
    __no_operation(); 
    
    if (buttonPressed)                      // Process a button press->transmit
    {
      ReceiveOff();                         // Button means TX, stop RX
      receiving = 0;                         

      TransmitPacket();                 

      buttonPressed = 0;                    // Re-enable button press                           
    }
    if(receiving)
    {
      ReceivePacket(); 
      __no_operation(); 
      // Check CRC
      if(RxBuffer[CRC_LQI_IDX] & CRC_OK)
      //Got it!
      P1OUT ^= BIT0;                    // Toggle LED1 in celebration

    }
    if(!transmitting)
    {
      ReceiveOn(); 
    }
  }


}

void ReceivePacket(void)
{
  rxBytesLeft = PACKET_LEN + 2;// Set maximum packet leng + 2 for appended bytes
  rxPosition = 0; 
  packetReceived = 0; 
  
  __delay_cycles(2800);                     // Wait for bytes to fill in RX FIFO

  TA0CCR1   = RX_TIMER_PERIOD;              // x cycles * 1/32768 = y us
  TA0CCTL1 |= CCIE;            
  TA0CTL   |= MC_2 + TACLR;                 // Start the timer- continuous mode

  __bis_SR_register(LPM3_bits + GIE); 
  __no_operation(); 
  
  TA0CCR1 = RX_TIMER_PERIOD;             
  TA0CCTL1 &= ~(CCIE);
  TA0CTL &= ~(MC_3);                  // Turn off timer 
  
  __no_operation(); 
}

void TransmitPacket(void)
{  
  P3OUT |= BIT6;                        // Pulse LED during Transmit 

  txBytesLeft = PACKET_LEN;   
  txPosition = 0; 
  packetTransmit = 0; 
  transmitting = 1;       
  
  Strobe( RF_STX );                         // Strobe STX   
  
  TA0CCR1   = TX_TIMER_PERIOD;              // x cycles * 1/32768 = y us
  TA0CCTL1 |= CCIE; 
  TA0CTL |= MC_2 + TACLR;                   // Start the timer- continuous mode   

  __bis_SR_register(LPM3_bits + GIE); 
  __no_operation(); 
  
  TA0CCR1 = TX_TIMER_PERIOD;             // x cycles * 1/32768 = y us
  TA0CCTL1 &= ~(CCIE); 
  TA0CTL &= ~(MC_3);                  // Turn off timer         

  P3OUT &= ~BIT6;                     // Turn off LED after Transmit               
}

void ReceiveOn(void)
{  
  RF1AIES &= ~BIT9; 
  RF1AIFG = 0;                              // Clear pending RFIFG interrupts
  RF1AIE  |= BIT9;                          // Enable the sync word received interrupt
  
  // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
  Strobe( RF_SRX );        
  
  __no_operation(); 
}

void ReceiveOff(void)
{
  RF1AIE &= ~BIT9;                          // Disable RX interrupts
  RF1AIFG &= ~BIT9;                         // Clear pending IFG
  RF1AIES &= ~BIT9;                         // Switch back to to sync word

  // It is possible that ReceiveOff is called while radio is receiving a packet.
  // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
  // such that the RXFIFO is empty prior to receiving a packet.
  Strobe(RF_SIDLE); 
  Strobe(RF_SFRX);                       
}

//------------------------------------------------------------------------------
//  void pktRxHandler(void)
//
//  DESCRIPTION:
//      This function is called every time a timer interrupt occurs. The 
//      function starts by retreiving the status byte. Every time the status 
//      byte indicates that there are available bytes in the RX FIFO, bytes are 
//      read from the RX FIFO and written to RxBuffer. This is done until the 
//      whole packet is received. If the status byte indicates that there has 
//      been an RX FIFO overflow the RX FIFO is flushed. Please see the 
//      EM430F5137RF900 RF Examples User Manual for a flow chart describing this 
//      function.
//------------------------------------------------------------------------------
void pktRxHandler(void) {
  unsigned char RxStatus;
  unsigned char bytesInFifo;
  
  // Which state?
  RxStatus = Strobe(RF_SNOP);
  
  switch(RxStatus & CC430_STATE_MASK)
  {
    case CC430_STATE_RX:
      // If there's anything in the RX FIFO....
      if (bytesInFifo = MIN(rxBytesLeft, RxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK))
      {         
        // Update how many bytes are left to be received
        rxBytesLeft -= bytesInFifo;
  
        // Read from RX FIFO and store the data in rxBuffer
        while (bytesInFifo--) {
          RxBuffer[rxPosition] = ReadSingleReg(RXFIFO);
          rxPosition++; 
        }  
        if (!rxBytesLeft){           
            packetReceived = 1; 
            receiving = 0;
            lengthByteRead = 0; 
            ReceiveOff();
            
            P1OUT ^= BIT0;                    // Toggle LED1             
        }      
      } 
      break;
    default:
      if(!packetReceived)
      {
        packetReceived = 1; 
      }
      
      rxBytesLeft = 0;
      receiving = 0; 
      ReceiveOff();

      break;  
  }
} // pktRxHandler

//------------------------------------------------------------------------------
//  void pktTxHandler(void)
//
//  DESCRIPTION:
//      This function is called every time a timer interrupt occurs. The function starts
//      by getting the status byte. Every time the status byte indicates that there 
//      is free space in the TX FIFO, bytes are taken from txBuffer and written to 
//      the TX FIFO until the whole packet is written or the TXFIFO has underflowed. 
//      See the EM430F5137RF900 RF Examples User Manual for a flow chart describing 
//      this function.
//------------------------------------------------------------------------------
void pktTxHandler(void) {
    unsigned char freeSpaceInFifo;
    unsigned char TxStatus;  
    
    // Which state?
    TxStatus = Strobe(RF_SNOP);

    switch (TxStatus & CC430_STATE_MASK) {
        case CC430_STATE_TX:
            // If there's anything to transfer..
            if (freeSpaceInFifo = MIN(txBytesLeft, TxStatus & CC430_FIFO_BYTES_AVAILABLE_MASK)) 
            {
              txBytesLeft -= freeSpaceInFifo;

              while(freeSpaceInFifo--)
              {
                WriteSingleReg(TXFIFO, TxBuffer[txPosition]);
                txPosition++; 
              }
              
              if(!txBytesLeft)
              {                
                RF1AIES |= BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9
                while(!(RF1AIFG & BIT9)); // poll RFIFG9 for TX end-of-packet  
                RF1AIES &= ~BIT9;      // End-of-packet TX interrupt
                RF1AIFG &= ~BIT9;     // clear RFIFG9
                transmitting = 0; 
                packetTransmit = 1; 
              }
            }
            break;

        case CC430_STATE_TX_UNDERFLOW:
            Strobe(RF_SFTX);  // Flush the TX FIFO
            
            __no_operation(); 
            // No break here!
        default:
            if(!packetTransmit) 
              packetTransmit = 1; 
            
            if (transmitting) {
                if ((TxStatus & CC430_STATE_MASK) == CC430_STATE_IDLE) {
    	          transmitting = 0; 
                }
            }
        break;
    }
} // pktTxHandler

void InitTimer(void)
{
  P5SEL |= 0x03;                            // Set xtal pins  
 // LFXT_Start(XT1DRIVE_0); //This seems to be for CC430F6137, the 5137 has no external 32 kHz clock.
  
  TA0CCR1  = RX_TIMER_PERIOD;               // x cycles * 1/32768 = y us
  TA0CCTL1 = CCIE;                          // Enable interrupts
  TA0CTL   = TASSEL__ACLK + TACLR;          // ACLK source
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

/**************************************
* Interrupt Service Routines
**************************************/

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  switch(__even_in_range(TA0IV,14))
  {
    case 0:  break;                  
    case 2:  
      if(receiving)
      {
        TA0CCR1 += RX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

        pktRxHandler();  
        
        if(packetReceived)
          __bic_SR_register_on_exit(LPM3_bits); 
      }
      else if(transmitting)
      {
        TA0CCR1 += TX_TIMER_PERIOD;                  // 16 cycles * 1/32768 = ~500 us

        pktTxHandler(); 
        
        if(packetTransmit)
          __bic_SR_register_on_exit(LPM3_bits); 
      }       
      break;
    case 4:  break;                         // CCR2 not used
    case 6:  break;                         // Reserved not used
    case 8:  break;                         // Reserved not used
    case 10: break;                         // Reserved not used
    case 12: break;                         // Reserved not used
    case 14: break;                         // Overflow not used
  }
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
      if(!(RF1AIES & BIT9))                 // RX sync word received 
      {
        receiving = 1;         
        __bic_SR_register_on_exit(LPM3_bits); // Exit active    
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
      __delay_cycles(1000);                 // debounce delay 
      buttonPressed = 1;
      P1IE = 0;                             // Debounce by disabling buttons
      P1IFG = 0; 
      __bic_SR_register_on_exit(LPM3_bits); // Exit active    
      break;
  }
}
