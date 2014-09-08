/*
 * radio.h
 *
 *  Created on: Sep 7, 2014
 *      Author: hunter
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "cc430x513x.h"
#include "RF1A.h"
#include "pmm.h"
#include <stdio.h>
#include <string.h>


/*******************
 * Function Definition
 */
void Transmit(unsigned char *buffer, unsigned char length);
void ReceiveOn(void);
void ReceiveOff(void);

void InitButtonLeds(void);
void InitRadio(void);


/////////////////////////
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


//void take_fft(int freq, int bw){
//	//Takes in a center frequency and bandwidth to do a fixed point fft on
//	int i, scale;
//    unsigned diff;
//	short x[n], fx[n];
//
//		for (i=0; i<n; i++){
//			x[i] = AMPLITUDE*cos(i*FREQUENCY*(2*3.1415926535)/n);
//			if (i & 0x01)
//				fx[(n+i)>>1] = x[i];
//			else
//				fx[i>>1] = x[i];
//	#if DEBUG
//	printf("%d %d\n", i, x[i]);
//	#endif
//		}
//	puts("");
//
//		fix_fftr(fx, log2n, 0);
//	#if SPECTRUM
//	for (i=0; i<n/2; i++) printf("%d %d\n", i, fx[i]);
//	return 0;
//	#endif
//		scale = fix_fftr(fx, log2n, 1);
//	fprintf(stderr, "scale = %d\n", scale);
//
//		for (i=0,diff=0; i<n; i++) {
//			int sample;
//			if (i & 0x01)
//				sample = fx[(n+i)>>1] << scale;
//			else
//				sample = fx[i>>1] << scale;
//	#if DEBUG
//	printf("%d %d\n", i, sample);
//	#endif
//			diff += abs(x[i]-sample);
//		}
//		fprintf(stderr, "sum(abs(diffs)))/n = %g\n", diff/(double)n);
//
//		return 0;
//}



#endif /* RADIO_H_ */
