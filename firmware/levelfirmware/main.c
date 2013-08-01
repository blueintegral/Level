//Add support for RS232 or SPI over the pins that are broken out. So it can be controlled over either.

#define DEBUG 1
#define SPECTRUM 0

#include "cc430x513x.h"
#include "IO.H"
#include "ADF4350.H"
#include "radio.h"
#include "fix_fft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/////FFT settings////////
#define FFT_SIZE  128
#define log2FFT   7
#define N         (2 * FFT_SIZE)
#define log2N     (log2FFT + 1)
#define FREQUENCY 5
#define AMPLITUDE 12288
/////////////////////////


void InitSys()
{
	//Set everything up

	////radio/////
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

}

int use_channel(const unsigned char *buffer){
	//Read the packet we got and configure the radio to operate how the base station stipulated
	return 1;
}

int connect_to_basestation(void){
		//Use FFT for this eventually for more accuracy
		int set = 0;
		int camped = 0;
		const unsigned char response[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Base station response packet is prepended with this sequence to guarantee that it's from our base station.
		int threshold = 500; //Threshold for deciding if a channel is open. Should be calculated more intelligently.
		const unsigned char RequestBeacon[11]= {PACKET_LEN, 0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //request beacon
		//The packet consists of a preamble, sync bit, length, the payload, and a CRC. You only need to provide the length
		//and payload, and everything else is taken care of automatically when you stick that in the transmit buffer.
		char power;
		int freq = 1780; //PLL frequency in hundreds of kHz (to avoid truncation)
		while(!set && (freq < 3980)){ //Start at the bottom of the spectrum and scan for open 100 kHz channels in 100 kHz increments.
			PLL_set_frq(freq); //Set PLL to LO frequency
			ReceiveOn();
			receiving = 1;
			SWITCH_RECEIVE(); //Set RF switches to receive
			power = ReadSingleReg(RSSI);
				if(power < threshold){
					//We found an open channel, so start transmitting a request beacon
					 set = 1;
					 while(!camped){
						 P1IFG = 0;
						 ReceiveOff();
						 receiving = 0;
						 SWITCH_TRANSMIT(); //Set RF switches to transmit
						 Transmit( (unsigned char*)RequestBeacon, sizeof RequestBeacon);
						 transmitting = 1;
						 //Now start listening for a response
						 SWITCH_RECEIVE();
						 ReceiveOn();
						 receiving = 1;
						 if(RxBuffer[CRC_LQI_IDX] & CRC_OK){
							 ReceiveOff();
							 receiving = 0;
						 	 //CRC checked out, so we got a packet. Let's make sure it's from our basestation
							 if(memcmp(RxBuffer, response, sizeof(response))){
								 camped = 1;
								 use_channel(RxBuffer);
								 return 1; //We're now operating on our assigned channel and bandwidth
						 	 }
						 }
					 }
				} else {
					freq = freq + 1; //Increase scan freq by 100 kHz
				}
			}
		return 0; //Failed to find an open channel.
}



int main(void){
		/////Set up system/////
		WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
        MCU_IO_set();
        InitSys();
        delay (20);
       // PLL_set_frq(); //Program VCO
        ///////////////////////

        /////Get radio ready for use/////
         SetVCore(2);  // Increase PMMCOREV level to 2 for proper radio operation
         ResetRadioCore();
         InitRadio();
         InitButtonLeds();
         ////////////////////////////////

        // ReceiveOn();
        // receiving = 1;

         //Set RF switches to stun. I mean, transmit.
      //   P1OUT |= BIT1; //Set P1.1 high (GDO2)

         while (1)
         {

           __bis_SR_register( LPM3_bits + GIE );
           __no_operation();

           connect_to_basestation();



           //  P3OUT |= BIT6;                        // Pulse LED during Transmit
           //  buttonPressed = 0;
             P1IFG = 0;

             ReceiveOff();
             receiving = 0;
             Transmit( (unsigned char*)TxBuffer, sizeof TxBuffer);
             transmitting = 1;

             P1IE |= BIT7;                         // Re-enable button press

         }

}
