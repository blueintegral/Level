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
 *   Target : Texas Instruments MSP-EXP430FG4618
 *            "MSP430FG4618/F2013 Experimenter Board"
 *   Radios : CC2500, CC1100, CC1101
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
 *                                           Defines
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                        Radio Selection
 * ------------------------------------------------------------------------------------------------
 */
#if (!defined MRFI_CC1100) && \
    (!defined MRFI_CC1101) && \
    (!defined MRFI_CC1100E_470) && \
    (!defined MRFI_CC1100E_950) && \
    (!defined MRFI_CC2500) && \
    (!defined MRFI_CC2420)
#error "ERROR: A compatible radio must be specified for the EXP461x board."
/*
 *  Since the EXP461x board can support several different radios, the installed
 *  radio must be specified with a #define.  It is best to do this at the
 *  project level.  However, if only one radio will ever be used, a #define
 *  could be placed here, above this error check.
 */
#endif


/* ------------------------------------------------------------------------------------------------
 *                                      GDO0 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define __mrfi_GDO0_BIT__                     2
#define MRFI_CONFIG_GDO0_PIN_AS_INPUT()       /* nothing required */
#define MRFI_GDO0_PIN_IS_HIGH()               (P1IN & BV(__mrfi_GDO0_BIT__))

#define MRFI_GDO0_INT_VECTOR                  PORT1_VECTOR
#define MRFI_ENABLE_GDO0_INT()                st( P1IE  |=  BV(__mrfi_GDO0_BIT__); ) /* atomic operation */
#define MRFI_DISABLE_GDO0_INT()               st( P1IE  &= ~BV(__mrfi_GDO0_BIT__); ) /* atomic operation */
#define MRFI_GDO0_INT_IS_ENABLED()             (  P1IE  &   BV(__mrfi_GDO0_BIT__) )
#define MRFI_CLEAR_GDO0_INT_FLAG()            st( P1IFG &= ~BV(__mrfi_GDO0_BIT__); ) /* atomic operation */
#define MRFI_GDO0_INT_FLAG_IS_SET()            (  P1IFG &   BV(__mrfi_GDO0_BIT__) )
#define MRFI_CONFIG_GDO0_RISING_EDGE_INT()    st( P1IES &= ~BV(__mrfi_GDO0_BIT__); ) /* atomic operation */
#define MRFI_CONFIG_GDO0_FALLING_EDGE_INT()   st( P1IES |=  BV(__mrfi_GDO0_BIT__); ) /* atomic operation */


/* ------------------------------------------------------------------------------------------------
 *                                      GDO2 Pin Configuration
 * ------------------------------------------------------------------------------------------------
 */
#define __mrfi_GDO2_BIT__                     3
#define MRFI_CONFIG_GDO2_PIN_AS_INPUT()       /* nothing required */
#define MRFI_GDO2_PIN_IS_HIGH()               (P1IN & BV(__mrfi_GDO2_BIT__))

#define MRFI_GDO2_INT_VECTOR                  PORT1_VECTOR
#define MRFI_ENABLE_GDO2_INT()                st( P1IE  |=  BV(__mrfi_GDO2_BIT__); ) /* atomic operation */
#define MRFI_DISABLE_GDO2_INT()               st( P1IE  &= ~BV(__mrfi_GDO2_BIT__); ) /* atomic operation */
#define MRFI_GDO2_INT_IS_ENABLED()             (  P1IE  &   BV(__mrfi_GDO2_BIT__) )
#define MRFI_CLEAR_GDO2_INT_FLAG()            st( P1IFG &= ~BV(__mrfi_GDO2_BIT__); ) /* atomic operation */
#define MRFI_GDO2_INT_FLAG_IS_SET()            (  P1IFG &   BV(__mrfi_GDO2_BIT__) )
#define MRFI_CONFIG_GDO2_RISING_EDGE_INT()    st( P1IES &= ~BV(__mrfi_GDO2_BIT__); ) /* atomic operation */
#define MRFI_CONFIG_GDO2_FALLING_EDGE_INT()   st( P1IES |=  BV(__mrfi_GDO2_BIT__); ) /* atomic operation */


/* ------------------------------------------------------------------------------------------------
 *                                      SPI Configuration
 * ------------------------------------------------------------------------------------------------
 */

/* CSn Pin Configuration */
#define __mrfi_SPI_CSN_GPIO_BIT__             2
#define MRFI_SPI_CONFIG_CSN_PIN_AS_OUTPUT()   st( P4DIR |=  BV(__mrfi_SPI_CSN_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_CSN_HIGH()             st( P4OUT |=  BV(__mrfi_SPI_CSN_GPIO_BIT__); ) /* atomic operation */
#define MRFI_SPI_DRIVE_CSN_LOW()              st( P4OUT &= ~BV(__mrfi_SPI_CSN_GPIO_BIT__); ) /* atomic operation */
#define MRFI_SPI_CSN_IS_HIGH()                 (  P4OUT &   BV(__mrfi_SPI_CSN_GPIO_BIT__) )

/* SCLK Pin Configuration */
#define __mrfi_SPI_SCLK_GPIO_BIT__            5
#define MRFI_SPI_CONFIG_SCLK_PIN_AS_OUTPUT()  st( P4DIR |=  BV(__mrfi_SPI_SCLK_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SCLK_HIGH()            st( P4OUT |=  BV(__mrfi_SPI_SCLK_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SCLK_LOW()             st( P4OUT &= ~BV(__mrfi_SPI_SCLK_GPIO_BIT__); )

/* SI Pin Configuration */
#define __mrfi_SPI_SI_GPIO_BIT__              3
#define MRFI_SPI_CONFIG_SI_PIN_AS_OUTPUT()    st( P4DIR |=  BV(__mrfi_SPI_SI_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SI_HIGH()              st( P4OUT |=  BV(__mrfi_SPI_SI_GPIO_BIT__); )
#define MRFI_SPI_DRIVE_SI_LOW()               st( P4OUT &= ~BV(__mrfi_SPI_SI_GPIO_BIT__); )

/* SO Pin Configuration */
#define __mrfi_SPI_SO_GPIO_BIT__              4
#define MRFI_SPI_CONFIG_SO_PIN_AS_INPUT()     /* nothing to required */
#define MRFI_SPI_SO_IS_HIGH()                 ( P4IN & BV(__mrfi_SPI_SO_GPIO_BIT__) )

/* SPI Port Configuration */
#define MRFI_SPI_CONFIG_PORT()                st( P4SEL |= BV(__mrfi_SPI_SCLK_GPIO_BIT__) |  \
                                                           BV(__mrfi_SPI_SI_GPIO_BIT__)   |  \
                                                           BV(__mrfi_SPI_SO_GPIO_BIT__); )

/* read/write macros */
#define MRFI_SPI_WRITE_BYTE(x)                st( IFG2 &= ~URXIFG1;  U1TXBUF = x; )
#define MRFI_SPI_READ_BYTE()                  U1RXBUF
#define MRFI_SPI_WAIT_DONE()                  while(!(IFG2 & URXIFG1));

/* SPI critical section macros */
typedef bspIState_t mrfiSpiIState_t;
#define MRFI_SPI_ENTER_CRITICAL_SECTION(x)    BSP_ENTER_CRITICAL_SECTION(x)
#define MRFI_SPI_EXIT_CRITICAL_SECTION(x)     BSP_EXIT_CRITICAL_SECTION(x)


/*
 *  Radio SPI Specifications
 * -----------------------------------------------
 *    Max SPI Clock   :  10 MHz
 *    Data Order      :  MSB transmitted first
 *    Clock Polarity  :  low when idle
 *    Clock Phase     :  sample leading edge
 */

/* initialization macro */
#define MRFI_SPI_INIT() \
st ( \
  U1CTL  = SWRST;                     \
  U1CTL  = SWRST | MM | SYNC | CHAR;  \
  U1TCTL = CKPH | STC | SSEL1;        \
  U1BR0  = 2;                         \
  U1BR1  = 0;                         \
  U1MCTL = 0;                         \
  ME2 |= USPIE1;                      \
  MRFI_SPI_CONFIG_PORT();             \
  U1CTL &= ~SWRST;                    \
)

#define MRFI_SPI_IS_INITIALIZED()     (U1TCTL & CKPH)


/**************************************************************************************************
 *                                  Compile Time Integrity Checks
 **************************************************************************************************
 */
#ifndef BSP_BOARD_EXP461x
#error "ERROR: Mismatch between specified board and MRFI configuration."
#endif


/**************************************************************************************************
 */
#endif
