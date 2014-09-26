/*
 * HAL.c
 *
 *  Created on: Sep 21, 2014
 *      Author: hunter
 */
#include "HAL.h"
//#include <msp430.h>
#include "cc430x513x.h"
//#include "IO.H"
#include "ADF4351.H"
//#include "radio.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include <strings.h>
#include "RF1A.h"
#include "RfRegSettings.h"

RF_SETTINGS rfSettings;

#define fosc 26000000 //26 MHz crystal oscillator used in this design for the CC430

int set_datarate(float rate){
	//The argument for this function, rate, is in kilobaud

	unsigned char DRATE_E;
	unsigned char DRATE_M;
	//unsigned char

	if ((rate < 0.8) || (rate > 406.3)){
		//The CC430 can't achieve the requested datarate
		return 1;
	}

	//set 0x10 MDMCFG4.DRATE_E at bits 3:0
	//exponent
	DRATE_E = floor(log2((rate*pow(2,20))/fosc));
	rfSettings.mdmcfg4 |= DRATE_E;

	//set 0x11 MDMCFG3.DRATE_M at bits 7:0
	//mantissa
	DRATE_M = floor((rate*pow(2,28))/(fosc*pow(2,DRATE_E)))-256;
	rfSettings.mdmcfg3 = DRATE_M;

	WriteRfSettings(&rfSettings);

	return 0;
}


int set_frequency(int freq){
	//Frequency is in Hz.
	//The frequency of the CC430 is fixed at 915 MHz, so frequency changes happen through the ADF4351

	if ((freq > 4400000000) || (freq < 30000000)){
		return 1;
	}

	PLL_set_frq(freq);

	return 0;
}

int set_power(int power){
	//This doesn't do anything yet, since I haven't picked an AGC amp yet.
	//This is set to a constant 125 mW, since that's the Minicircuits amp being used.
	return 0;
}


int set_mode(char * mode){

	if(strcmp(mode, "tx'")){

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

	} else if (strcmp(mode, "rx")){

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

	} else {
		return 1;
	}

	return 0;

}


struct status get_status(void){

	struct status s; //status struct we're gonna populate

	s.partnum = ReadSingleReg(PARTNUM); //part number
	s.version = ReadSingleReg(VERSION); //current version number
	s.freqest = ReadSingleReg(FREQEST); //frequency offset estimate
	s.lqi = ReadSingleReg(LQI); //demodulator estimate for link quality
	s.rssi = ReadSingleReg(RSSI); // received signal strength indication
	s.marcstate = ReadSingleReg(MARCSTATE); //control state machine state
	s.wortime1 = ReadSingleReg(WORTIME1); //high bytes of WOR timer
	s.wortime0 = ReadSingleReg(WORTIME0); //low byte of WOR timer
	s.ptkstatus = ReadSingleReg(PKTSTATUS); //current GDOx status and packet status
	s.vco_vc_dac = ReadSingleReg(VCO_VC_DAC); //current setting from PLL calibration module
	s.txbytes = ReadSingleReg(TXBYTES); //underflow and number of bytes in the TX FIFO
	s.rxbytes = ReadSingleReg(RXBYTES); //overflow and number of bytes in the RX FIFO

	return s;
}


int get_device_id(void){

	return ReadSingleReg(PARTNUM);

}


int transmit_now(void){
	unsigned char txBytesLeft = PACKET_LENGTH;           // +1 for length byte
	unsigned char packetTransmit;
	unsigned char transmitting = 0;
	unsigned char txPosition = 0;

	//disable receiving
	RF1AIE &= ~BIT9;                          // Disable RX interrupts
	RF1AIFG &= ~BIT9;                         // Clear pending IFG
	RF1AIES &= ~BIT9;                         // Switch back to to sync word

	// It is possible that ReceiveOff is called while radio is receiving a packet.
	// Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe
	// such that the RXFIFO is empty prior to receiving a packet.
	Strobe(RF_SIDLE);
	Strobe(RF_SFRX);

	//Set RF switches to stun. I mean, transmit.
	P1OUT |= BIT1; //Set P1.1 high (GDO2)

	//begin transmitting
	P3OUT |= BIT6;                        // Pulse LED during Transmit

	txBytesLeft = PACKET_LENGTH;
	txPosition = 0;
	packetTransmit = 0;
	transmitting = 1;

	Strobe( RF_STX );                         // Strobe STX

	TA0CCR1   = TX_TIMER_PERIOD;              // x cycles * 1/32768 = y us
	TA0CCTL1 |= CCIE;
	TA0CTL |= MC_2 + TACLR;                   // put the timer in continuous mode

	__bis_SR_register(LPM3_bits + GIE);
	__no_operation();

	TA0CCR1 = TX_TIMER_PERIOD;             // x cycles * 1/32768 = y us
	TA0CCTL1 &= ~(CCIE);
	TA0CTL &= ~(MC_3);                  // Turn off timer

	P3OUT &= ~BIT6;                     // Turn off LED after Transmit

	return 0;
}

int get_message(void){

	unsigned char packetReceived;
	unsigned char rxBytesLeft = PACKET_LENGTH+2; // +2 for status bytes
	unsigned char rxPosition = 0;


	//Enable RX
	 RF1AIES &= ~BIT9;
	 RF1AIFG = 0;                              // Clear pending RFIFG interrupts
	 RF1AIE  |= BIT9;                          // Enable the sync word received interrupt

	 // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
	 Strobe( RF_SRX );
	 __no_operation();

	//Start receiving
	 rxBytesLeft = PACKET_LENGTH + 2;// Set maximum packet leng + 2 for appended bytes
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

	return 0;
}





