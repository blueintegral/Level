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

#ifndef DCO_calibrate
#define DCO_calibrate

extern char TI_SetDCO( int delta );

#define TI_DCO_NO_ERROR 0
#define TI_DCO_SET_TO_SLOWEST 1
#define TI_DCO_SET_TO_FASTEST 2
#define TI_DCO_TIMEOUT_ERROR 0xFF

//Note: all values rounded DOWN in order to ensure Vcc specifications
#define TI_DCO_1MHZ      244                       // Target DCO = DELTA*(4096) ~1MHz
#define TI_DCO_2MHZ      488                       // Target DCO = DELTA*(4096) ~2MHz
#define TI_DCO_3MHZ      732                       // Target DCO = DELTA*(4096) ~3MHz
#define TI_DCO_4MHZ      977                       // Target DCO = DELTA*(4096) ~4MHz
#define TI_DCO_5MHZ      1221                      // Target DCO = DELTA*(4096) ~5MHz
#define TI_DCO_6MHZ      1465                      // Target DCO = DELTA*(4096) ~6MHz
#define TI_DCO_7MHZ      1709                      // Target DCO = DELTA*(4096) ~7MHz
#define TI_DCO_8MHZ      1953                      // Target DCO = DELTA*(4096) ~8MHz
#define TI_DCO_9MHZ      2197                      // Target DCO = DELTA*(4096) ~9MHz
#define TI_DCO_10MHZ     2441                      // Target DCO = DELTA*(4096) ~10MHz
#define TI_DCO_11MHZ     2686                      // Target DCO = DELTA*(4096) ~11MHz
#define TI_DCO_12MHZ     2930                      // Target DCO = DELTA*(4096) ~12MHz
#define TI_DCO_13MHZ     3174                      // Target DCO = DELTA*(4096) ~13MHz
#define TI_DCO_14MHZ     3418                      // Target DCO = DELTA*(4096) ~14MHz
#define TI_DCO_15MHZ     3662                      // Target DCO = DELTA*(4096) ~15MHz
#define TI_DCO_16MHZ     3906                      // Target DCO = DELTA*(4096) ~16MHz

#endif
