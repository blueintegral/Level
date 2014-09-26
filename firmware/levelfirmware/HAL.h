/*
 * HAL.h
 *
 *  Created on: Sep 21, 2014
 *      Author: hunter
 */

#ifndef HAL_H_
#define HAL_H_

#define PACKET_LENGTH (100)
#define  TX_TIMER_PERIOD (40)
#define  RX_TIMER_PERIOD (85)


//Function prototypes
int set_datarate(float rate);
int set_frequency(int freq);
int set_power(int power);
int set_mode(char * mode);
struct status get_status(void);
int get_device_id(void);
int transmit_now(void);
int get_message(void);

struct status {
		int partnum; //part number
		int version; //current version number
		int freqest; //frequency offset estimate
		int lqi; //demodulator estimate for link quality
		int rssi; // received signal strength indication
		int marcstate; //control state machine state
		int wortime1; //high bytes of WOR timer
		int wortime0; //low byte of WOR timer
		int ptkstatus; //current GDOx status and packet status
		int vco_vc_dac; //current setting from PLL calibration module
		int txbytes; //underflow and number of bytes in the TX FIFO
		int rxbytes; //overflow and number of bytes in the RX FIFO
	};


#endif /* HAL_H_ */
