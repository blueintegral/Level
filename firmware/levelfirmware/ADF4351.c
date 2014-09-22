/*
 * ADF4351.c
 *
 *  Created on: Sep 21, 2014
 *      Author: hunter
 */


//#include "ADF4350_IO.h"
#include "cc430x513x.h"

#define PLL_SET_LE()		P1OUT |= BIT0	//P2.0->LE for dev board, P1.0->LE for our board
#define PLL_CLR_LE()		P1OUT &=~BIT0

#define	PLL_SET_SCL()		P1OUT |= BIT1	//P2.2->SCL for dev board, P1.1->SCL for our board
#define	PLL_CLR_SCL()		P1OUT &=~BIT1

#define	PLL_SET_CE()		P1OUT |= BIT1	//P2.2->SCL for dev board, P1.1->SCL for our board
#define	PLL_CLR_CE()		P1OUT &=~BIT1

#define PLL_SET_DATA()		P1OUT |= BIT6	//P2.4->DATA for dev board, P1.6->DATA for our board
#define PLL_CLR_DATA()		P1OUT &=~BIT6

#define PLL_SET_DATA_OUT()	P5DIR |= BIT1
#define PLL_SET_DATA_IN()	P5DIR &=~BIT1

#define PLL_PowUp()			P5OUT |= BIT6	//P5.6->PD (RF mute)
#define PLL_PowDown()		P5OUT &=~BIT6

#define PLL_DATA_IN 	(P5IN & BIT1)
#define PLL_LD_IN 		(P5IN & BIT5)

#define SWITCH_TRANSMIT()	P1OUT |= BIT0; //RF switch
#define SWITCH_RECEIVE()	P1OUT &= ~BIT0;

/*-------------------------------------400MHz---------------------------------------------
Reference frequency: 30MHz;Output frequency: 400MHz;VCO frequency: 3200MHz;Prescaler: 8/9;
Output divider: 8;VCO channel spacing frequency: 800KHz;PFD frequency: 30MHz;
INT: 106;FRAC: 0;MOD: 3;R: 1;Clk Div: 240;Phase: 1
----------------------------------------------------------------------------------------*/
/*
unsigned char PLL_Reg_buf0[4]	={0x00,0x35,0x00,0x10};   //400 MHz
unsigned char PLL_Reg_buf1[4]	={0x08,0x00,0x80,0x19};
unsigned char PLL_Reg_buf2[4]	={0x14,0x00,0x56,0xC2};
unsigned char PLL_Reg_buf3[4]	={0x00,0x00,0x04,0xB3};
unsigned char PLL_Reg_buf4[4]	={0x00,0xBF,0x00,0x3C};
unsigned char PLL_Reg_buf5[4]	={0x00,0x40,0x00,0x05};
*/

unsigned char PLL_Reg_buf0[4]	={0x00,0x38,0x00,0x00};   //350 MHz @ +5 dBm
unsigned char PLL_Reg_buf1[4]	={0x08,0x00,0x80,0x09};
unsigned char PLL_Reg_buf2[4]	={0x00,0x00,0x4E,0x42};
unsigned char PLL_Reg_buf3[4]	={0x00,0x00,0x04,0xB3};
unsigned char PLL_Reg_buf4[4]	={0x00,0xBC,0x80,0x3C};
unsigned char PLL_Reg_buf5[4]	={0x00,0x58,0x00,0x05};


/*
unsigned char PLL_Reg_buf0[4]	={0x00,0x35,0x00,0x10};		//800 MHz
unsigned char PLL_Reg_buf1[4]	={0x08,0x00,0x80,0x19};
unsigned char PLL_Reg_buf2[4]	={0x16,0x00,0x96,0xC2};
unsigned char PLL_Reg_buf3[4]	={0x00,0x00,0x04,0xB3};
unsigned char PLL_Reg_buf4[4]	={0x00,0xAF,0x00,0x3C};
unsigned char PLL_Reg_buf5[4]	={0x00,0x40,0x00,0x05};
*/

void delay (int length)
{
	while (length >0)
    	length--;
}

//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port.
//--------------------------------------------------------------------------------
void WriteToADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;

	PLL_SET_DATA_OUT();

	delay(1);
	PLL_CLR_SCL();
	PLL_CLR_LE(); //bring LE low
	delay(1);

	for(i=0;i<count;i++)
 	{
	 	ValueToWrite = *(buf + i);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				PLL_SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				PLL_CLR_DATA();	  //Send zero to SDO pin
			}
			delay(1);
			PLL_SET_SCL();
			delay(1);
			ValueToWrite <<= 1;	//Rotate data
			PLL_CLR_SCL();
		}
	}
	PLL_CLR_DATA();
	delay(1);
	PLL_SET_LE(); //bring LE high again once this register is set.
	delay(1);
//	PLL_CLR_LE();
}


//---------------------------------
//void ReadFromADF4350(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the ADF4350 via the SPI port.
//--------------------------------------------------------------------------------
void ReadFromADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	int  	iTemp = 0;
	unsigned	char  	RotateData = 0;

	PLL_SET_DATA_IN();

	delay(1);
	PLL_CLR_SCL();
	PLL_CLR_LE();
	delay(1);

	for(j=0; j<count; j++)
	{
		for(i=0; i<8; i++)
		{
			RotateData <<= 1;		//Rotate data
			delay(1);
			iTemp = PLL_DATA_IN;			//Read DATA off ADF4350
			PLL_SET_SCL();
			if(iTemp)
			{
				RotateData |= 1;
			}
			delay(1);
			PLL_CLR_SCL();
		}
		*(buf + j)= RotateData;
	}
	delay(1);
	PLL_SET_LE();
	delay(1);
	PLL_CLR_LE();
}

void PLL_set_frq (int freq)
{
	PLL_SET_CE(); //take clock high
	WriteToADF4350(4,PLL_Reg_buf5);
	WriteToADF4350(4,PLL_Reg_buf4);		//(DB23-14:1H)R counter is 1
	WriteToADF4350(4,PLL_Reg_buf3);		//(DB14-3:96H)clock divider value is 150.
	WriteToADF4350(4,PLL_Reg_buf2);
	WriteToADF4350(4,PLL_Reg_buf1);		//(DB27=1)prescaler value is 8/9
	WriteToADF4350(4,PLL_Reg_buf0);		//(DB30-15:F0H)INT value is 240;
	PLL_PowUp();
}



