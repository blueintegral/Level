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
#include "ADF4351.h"
//#include "radio.h"
//#include <stdio.h>
//#include <stdlib.h>
#include "math.h"
#include <string.h>
#include "RF1A.h"
#include "RfRegSettings.h"

//RF_SETTINGS rfSettings;

RF_SETTINGS rfSettings = {
	0x06,   // FSCTRL1   Frequency synthesizer control.
	0x00,   // FSCTRL0   Frequency synthesizer control.
	0x21,   // FREQ2     Frequency control word, high byte.
	0x62,   // FREQ1     Frequency control word, middle byte.
	0x76,   // FREQ0     Frequency control word, low byte.
	0x2B,   // MDMCFG4   Modem configuration.
	0x3B,   // MDMCFG3   Modem configuration.
	0x75,   // MDMCFG2   Modem configuration.
	0x22,   // MDMCFG1   Modem configuration.
	0xF8,   // MDMCFG0   Modem configuration.
	0x00,   // CHANNR    Channel number.
	0x15,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
	0x56,   // FREND1    Front end RX configuration.
	0x10,   // FREND0    Front end TX configuration.
	0x18,   // MCSM0     Main Radio Control State Machine configuration.
	0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
	0x6C,   // BSCFG     Bit synchronization Configuration.
	0x03,   // AGCCTRL2  AGC control.
	0x40,   // AGCCTRL1  AGC control.
	0x91,   // AGCCTRL0  AGC control.
	0xEA,   // FSCAL3    Frequency synthesizer calibration.
	0x2A,   // FSCAL2    Frequency synthesizer calibration.
	0x00,   // FSCAL1    Frequency synthesizer calibration.
	0x1F,   // FSCAL0    Frequency synthesizer calibration.
	0x59,   // FSTEST    Frequency synthesizer calibration.
	0x81,   // TEST2     Various test settings.
	0x31,   // TEST1     Various test settings.
	0x09,   // TEST0     Various test settings.
	0x47,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
	0x29,   // IOCFG2    GDO2 output pin configuration.
	0x06,   // IOCFG0    GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for detailed pseudo register explanation.
	0x04,   // PKTCTRL1  Packet automation control.
	0x05,   // PKTCTRL0  Packet automation control.
	0x00,   // ADDR      Device address.
	0xFF    // PKTLEN    Packet length.
};



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


//int get_status(void);
//int get_device_id(void);
//int set_message(char *buf, int length);
//int transmit_now(void);
//int get_message(void);
//int set_modulation(char *mod);



