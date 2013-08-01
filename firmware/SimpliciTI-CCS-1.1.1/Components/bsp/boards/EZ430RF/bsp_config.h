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

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   Target : Texas Instruments EZ430-RF2500
 *            "MSP430 Wireless Development Tool"
 *   Board configuration file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H


/**************************************************************************************************
 *                                       Configuration                                            *
 **************************************************************************************************
 */

/*
 *  Supported clock speeds : 1, 2, 4, 6, 8, 10, 12, and 16 MHz.
 *  The clock configuration values for 1, 8, 12, 16 MHz are taken from configuration flash
 *  set at the factory and should be the most accurate.
 *
 *  NOTE!  The clock speeds are approximate as they are derived from an internal
 *         digitally controlled oscillator.
 */
#define BSP_CONFIG_CLOCK_MHZ_SELECT     8  /* approximate MHz */

/*
 *  Custom clock configuration is available.  Provide values for BCSCTL1 and DCOCTL as
 *  shown in the example below.  The clock speed itself must be provided too.  Providing
 *  a custom configuration overrides the above selection.
 *
 *  #define BSP_CONFIG_CUSTOM_CLOCK_MHZ         2.1
 *  #define BSP_CONFIG_MSP430_CUSTOM_BCSCTL1    9
 *  #define BSP_CONFIG_MSP430_CUSTOM_DCOCTL     ( 1 << 5 )
 */


/* ------------------------------------------------------------------------------------------------
 *                                Exported Clock Configuration 
 * ------------------------------------------------------------------------------------------------
 */
#if (defined BSP_CONFIG_CUSTOM_CLOCK_MHZ)      || \
    (defined BSP_CONFIG_MSP430_CUSTOM_BCSCTL1) || \
    (defined BSP_CONFIG_MSP430_CUSTOM_DCOCTL)
#define BSP_CONFIG_CLOCK_MHZ        BSP_CONFIG_CUSTOM_CLOCK_MHZ
#define BSP_CONFIG_MSP430_BCSCTL1   BSP_CONFIG_MSP430_CUSTOM_BCSCTL1
#define BSP_CONFIG_MSP430_DCOCTL    BSP_CONFIG_MSP430_CUSTOM_DCOCTL
#else

#if (BSP_CONFIG_CLOCK_MHZ_SELECT == 1)
#define BSP_CONFIG_CLOCK_MHZ        1.0
#define BSP_CONFIG_MSP430_BCSCTL1   CALBC1_1MHZ   /* factory calibrated value from flash */
#define BSP_CONFIG_MSP430_DCOCTL    CALDCO_1MHZ   /* factory calibrated value from flash */
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 2)
#define BSP_CONFIG_CLOCK_MHZ        2.1
#define BSP_CONFIG_MSP430_BCSCTL1   9
#define BSP_CONFIG_MSP430_DCOCTL    ((/* DCO = */ 1 )  << 5 )
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 4)
#define BSP_CONFIG_CLOCK_MHZ        3.9
#define BSP_CONFIG_MSP430_BCSCTL1   10
#define BSP_CONFIG_MSP430_DCOCTL    ((/* DCO = */ 5 )  << 5 )
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 6)
#define BSP_CONFIG_CLOCK_MHZ        6.1
#define BSP_CONFIG_MSP430_BCSCTL1   12
#define BSP_CONFIG_MSP430_DCOCTL    ((/* DCO = */ 4 )  << 5 )
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 8)
#define BSP_CONFIG_CLOCK_MHZ        8.0
#define BSP_CONFIG_MSP430_BCSCTL1   CALBC1_8MHZ   /* factory calibrated value from flash */
#define BSP_CONFIG_MSP430_DCOCTL    CALDCO_8MHZ   /* factory calibrated value from flash */
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 10)
#define BSP_CONFIG_CLOCK_MHZ        10.3
#define BSP_CONFIG_MSP430_BCSCTL1   14
#define BSP_CONFIG_MSP430_DCOCTL    ((/* DCO = */ 2 )  << 5 ) 
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 12)
#define BSP_CONFIG_CLOCK_MHZ        12.0
#define BSP_CONFIG_MSP430_BCSCTL1   CALBC1_12MHZ  /* factory calibrated value from flash */
#define BSP_CONFIG_MSP430_DCOCTL    CALDCO_12MHZ  /* factory calibrated value from flash */
#elif (BSP_CONFIG_CLOCK_MHZ_SELECT == 16)
#define BSP_CONFIG_CLOCK_MHZ        16.0
#define BSP_CONFIG_MSP430_BCSCTL1   CALBC1_16MHZ  /* factory calibrated value from flash */
#define BSP_CONFIG_MSP430_DCOCTL    CALDCO_16MHZ  /* factory calibrated value from flash */
#else
#error "ERROR: Unsupported clock speed.  Custom clock speeds are possible. See comments in code."
#endif

#endif


/**************************************************************************************************
 */
#endif
