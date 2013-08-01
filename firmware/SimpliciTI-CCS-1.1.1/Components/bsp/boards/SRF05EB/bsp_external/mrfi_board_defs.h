/**************************************************************************************************
  Revised:        $Date: 2007-07-06 11:19:00 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Copyright 2007 Texas Instruments Incorporated.  All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS”
  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
 *   MRFI (Minimal RF Interface)
 *   Board definition file.
 *   Target : Texas Instruments SmartRF05
 *   Radios : CC2520
 * ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
 */

#ifndef MRFI_BOARD_DEFS_H
#define MRFI_BOARD_DEFS_H

/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp.h"

/* ------------------------------------------------------------------------------------------------
 *                                       Global Variables
 * ------------------------------------------------------------------------------------------------
 */

#ifdef MRFI_PA_LNA_ENABLED
extern uint8_t mrfiLnaHighGainMode;
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Defines
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                        Radio Selection
 * ------------------------------------------------------------------------------------------------
 */
#if (!defined MRFI_CC2520)
  #error "ERROR: A compatible radio must be specified for the SmartRF05 board."
/*
 *  Since the SmartRF05 board can support several different radios, the installed
 *  radio must be specified with a #define.  It is best to do this at the
 *  project level.  However, if only one radio will ever be used, a #define
 *  could be placed here, above this error check.
 */
#endif


/* ------------------------------------------------------------------------------------------------
 *                                 VREG_EN Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define MRFI_VREG_EN_GPIO_BIT                0
#define MRFI_DRIVE_VREG_EN_PIN_HIGH()        st( P1OUT |=  BV(MRFI_VREG_EN_GPIO_BIT); ) /* atomic operation */
#define MRFI_DRIVE_VREG_EN_PIN_LOW()         st( P1OUT &= ~BV(MRFI_VREG_EN_GPIO_BIT); ) /* atomic operation */
#define MRFI_CONFIG_VREG_EN_PIN_AS_OUTPUT()  st( P1DIR |=  BV(MRFI_VREG_EN_GPIO_BIT); )


/* ------------------------------------------------------------------------------------------------
 *                                  RESETN Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define MRFI_RESETN_GPIO_BIT                 7
#define MRFI_DRIVE_RESETN_PIN_HIGH()         st( P5OUT |=  BV(MRFI_RESETN_GPIO_BIT); ) /* atomic operation */
#define MRFI_DRIVE_RESETN_PIN_LOW()          st( P5OUT &= ~BV(MRFI_RESETN_GPIO_BIT); ) /* atomic operation */
#define MRFI_CONFIG_RESETN_PIN_AS_OUTPUT()   st( P5DIR |=  BV(MRFI_RESETN_GPIO_BIT); )


/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* CSn Pin Configuration */
#define __mrfi_SPI_CSN_GPIO_BIT__             0
#define MRFI_SPI_CONFIG_CSN_PIN_AS_OUTPUT()   st( P5DIR |=  BV(__mrfi_SPI_CSN_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_CSN_HIGH()             st( P5OUT |=  BV(__mrfi_SPI_CSN_GPIO_BIT__); ) /* atomic operation */
#define MRFI_SPI_DRIVE_CSN_LOW()              st( P5OUT &= ~BV(__mrfi_SPI_CSN_GPIO_BIT__); ) /* atomic operation */
#define MRFI_SPI_CSN_IS_HIGH()                 (  P5OUT &   BV(__mrfi_SPI_CSN_GPIO_BIT__) )

#define MRFI_SPI_SET_CHIP_SELECT_OFF()    MRFI_SPI_DRIVE_CSN_HIGH()
#define MRFI_SPI_SET_CHIP_SELECT_ON()     MRFI_SPI_DRIVE_CSN_LOW()
#define MRFI_SPI_CHIP_SELECT_IS_OFF()     MRFI_SPI_CSN_IS_HIGH()

/* SCLK Pin Configuration */
#define __mrfi_SPI_SCLK_GPIO_BIT__            3
#define MRFI_SPI_CONFIG_SCLK_PIN_AS_OUTPUT()  st( P5DIR |=  BV(__mrfi_SPI_SCLK_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SCLK_HIGH()            st( P5OUT |=  BV(__mrfi_SPI_SCLK_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SCLK_LOW()             st( P5OUT &= ~BV(__mrfi_SPI_SCLK_GPIO_BIT__); )

/* SI Pin Configuration */
#define __mrfi_SPI_SI_GPIO_BIT__              1
#define MRFI_SPI_CONFIG_SI_PIN_AS_OUTPUT()    st( P5DIR |=  BV(__mrfi_SPI_SI_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SI_HIGH()              st( P5OUT |=  BV(__mrfi_SPI_SI_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SI_LOW()               st( P5OUT &= ~BV(__mrfi_SPI_SI_GPIO_BIT__); )

/* SO Pin Configuration */
#define __mrfi_SPI_SO_GPIO_BIT__              2
#define MRFI_SPI_CONFIG_SO_PIN_AS_INPUT()     /* nothing to required */
#define MRFI_SPI_SO_IS_HIGH()                 ( P5IN & BV(__mrfi_SPI_SO_GPIO_BIT__) )


/* SPI Port Configuration */
#define MRFI_SPI_CONFIG_PORT()                st( P5SEL |= BV(__mrfi_SPI_SCLK_GPIO_BIT__) |  \
                                                           BV(__mrfi_SPI_SI_GPIO_BIT__)   |  \
                                                           BV(__mrfi_SPI_SO_GPIO_BIT__); )

/* read/write macros */
#define MRFI_SPI_WRITE_BYTE(x)                st( UC1IFG &= ~UCB1RXIFG;  UCB1TXBUF = x; )
#define MRFI_SPI_READ_BYTE()                  UCB1RXBUF
#define MRFI_SPI_WAIT_DONE()                  while(!(UC1IFG & UCB1RXIFG));

/* SPI critical section macros */
typedef bspIState_t mrfiSpiIState_t;
#define MRFI_SPI_ENTER_CRITICAL_SECTION(x)    BSP_ENTER_CRITICAL_SECTION(x)
#define MRFI_SPI_EXIT_CRITICAL_SECTION(x)     BSP_EXIT_CRITICAL_SECTION(x)



/*
  GPIO_0 <---- TX_FRM_DONE -----> P1.3
  GPIO_1 <----    FIFO     -----> P1.5
  GPIO_2 <----    FIFOP    -----> P1.6

#ifdef MRFI_PA_LNA_ENABLED
  GPIO_3 <-----------> HGM
  GPIO_4 <-----------> PA
  GPIO_5 <-----------> PAEN
#else
  GPIO_3 <-----------> P1.1
  GPIO_4 <-----------> P1.2
  GPIO_5 <-----------> P1.7
#endif
*/

#define MRFI_BOARD_CONFIG_RADIO_GPIO() st(                                     \
    /* Program the radio GPIO_0 to output TX_FRM_DONE Exception */             \
    mrfiSpiWriteReg(GPIOCTRL0, 0x02);                                          \
    );

#ifdef MRFI_PA_LNA_ENABLED

#define MRFI_BOARD_PA_LNA_CONFIG_PORTS() st(                                   \
    /* HGM of CC2591 is connected to GPIO_3. Set it to HIGH (HGM) */           \
    mrfiSpiWriteReg(GPIOCTRL3, 0x7F);                                          \
 /* The following settings for LNA_PD/PA_PD are not documented in datasheet.*/ \
    /* EN of CC2591 is connected to GPIO_4. 0x46 == LNA_PD. */                 \
    mrfiSpiWriteReg(GPIOCTRL4, 0x46);                                          \
    /* PAEN of CC2591 is connected to GPIO_5. 0x47 == PA_PD. */                \
    mrfiSpiWriteReg(GPIOCTRL5, 0x47);                                          \
    /* Invert polarity of GPIO 4 and 5 */                                      \
    mrfiSpiWriteReg(GPIOPOLARITY, 0x0F);                                       \
    );

#define  MRFI_BOARD_PA_LNA_LGM()  st( mrfiLnaHighGainMode = 0; mrfiSpiWriteReg(GPIOCTRL3, 0x7E););
#define  MRFI_BOARD_PA_LNA_HGM()  st( mrfiLnaHighGainMode = 1; mrfiSpiWriteReg(GPIOCTRL3, 0x7F););

#endif

/* Port bits */
#define __mrfi_TX_FRM_DONE_BIT   3
#define __mrfi_FIFO_BIT          5
#define __mrfi_FIFOP_BIT         6

/* Port status */
#define MRFI_TX_DONE_STATUS()           (P1IN & BV(__mrfi_TX_FRM_DONE_BIT))
#define MRFI_FIFOP_STATUS()             (P1IN & BV(__mrfi_FIFOP_BIT))
#define MRFI_FIFO_STATUS()              (P1IN & BV(__mrfi_FIFO_BIT))

/* Port interrupt flags */
#define MRFI_FIFOP_INTERRUPT_FLAG()     (P1IFG & BV(__mrfi_FIFOP_BIT))


/* Port configure macros */
#define MRFI_CONFIG_TX_FRAME_DONE_AS_INPUT()  st( P1SEL &= ~BV(__mrfi_TX_FRM_DONE_BIT); \
                                                  P1DIR &= ~BV(__mrfi_TX_FRM_DONE_BIT); )
#define MRFI_CONFIG_FIFO_AS_INPUT()           st( P1SEL &= ~BV(__mrfi_FIFO_BIT); \
                                                  P1DIR &= ~BV(__mrfi_FIFO_BIT); )
#define MRFI_CONFIG_FIFOP_AS_INPUT()          st( P1SEL &= ~BV(__mrfi_FIFOP_BIT); \
                                                  P1DIR &= ~BV(__mrfi_FIFOP_BIT); \
                                                  P1IES &= ~BV(__mrfi_FIFOP_BIT); )

/* Port control macros */
#define MRFI_ENABLE_RX_INTERRUPT()     (P1IE  |=  BV(__mrfi_FIFOP_BIT));
#define MRFI_DISABLE_RX_INTERRUPT()    (P1IE  &= ~BV(__mrfi_FIFOP_BIT));
#define MRFI_CLEAR_RX_INTERRUPT_FLAG() (P1IFG &= ~BV(__mrfi_FIFOP_BIT));


/*
 *  Radio SPI Specifications
 * -----------------------------------------------
 *    Max SPI Clock   :  10 MHz
 *    Data Order      :  MSB transmitted first
 *    Clock Polarity  :  low when idle
 *    Clock Phase     :  sample leading edge
 */

/*
 *  Chipcon CC2520 SPI Specifications
 * -----------------------------------------------
 *    Max SPI Clock   :  10 MHz
 *    Data Order      :  MSB transmitted first
 *    Clock Polarity  :  low when idle
 *    Clock Phase     :  sample leading edge
 */

/* initialization macro */
#define MRFI_SPI_INIT() \
st ( \
  UCB1CTL1 = UCSSEL1 | UCSWRST;                 \
  UCB1CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC ;  \
  UCB1BR0  = 0;                                 \
  UCB1BR1  = 0;                                 \
  MRFI_SPI_CONFIG_PORT();                       \
  UCB1CTL1 &= ~UCSWRST;                         \
)


/**************************************************************************************************
 *                                  Compile Time Integrity Checks
 **************************************************************************************************
 */
#ifndef BSP_BOARD_SRF05EB
  #error "ERROR: Mismatch between specified board and MRFI configuration."
#endif


/**************************************************************************************************
 */
#endif
