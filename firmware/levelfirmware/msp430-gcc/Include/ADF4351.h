/*
 * ADF4351.h
 *
 *  Created on: Sep 7, 2014
 *      Author: hunter
 */

#ifndef ADF4351_H_
#define ADF4351_H_

void delay(int length);
void WriteToADF4350(unsigned char count, unsigned char *buf);
void ReadFromADF4350(unsigned char count, unsigned char *buf);
void PLL_set_frq (int freq);




#endif /* ADF4351_H_ */
