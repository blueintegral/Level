/*----------------------------------------------------------------------------
 *  Demo Application for SimpliciTI
 *
 *  L. Friedman
 *  Texas Instruments, Inc.
 *---------------------------------------------------------------------------- */

/**********************************************************************************************
  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

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

#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"

#include "app_remap_led.h"

/*         SANITY CHECKS                   */
#if defined( BSP_BOARD_RFUSB )
#error ERROR: RFUSB does not support default channel sniffer build (has only 1 LED)
#endif

#if ( NUM_CONNECTIONS > 0 )
#error ERROR: NUM_CONNECTIONS must be 0 when building channel sniffer
#endif
/*         END SANITY CHECKS                   */

#define SPIN_ABOUT_A_SECOND   NWK_DELAY(1000)
#define SPIN_ABOUT_A_QUARTER_SECOND   NWK_DELAY(250)

static void toggleLED(uint8_t);

void main (void)
{
  ioctlScanChan_t scan;
  freqEntry_t     freq[NWK_FREQ_TBL_SIZE];
  uint8_t         firstTimeThru = 1;

  BSP_Init();

  /* Keep trying to join (a side effect of successful initialization) until
   * successful. Toggle LEDS to indicate that joining has not occurred.
   */
  while (SMPL_SUCCESS != SMPL_Init(0))
  {
    toggleLED(1);
    toggleLED(2);
    SPIN_ABOUT_A_SECOND;
  }

  scan.freq = freq;
  while (1)
  {
    SPIN_ABOUT_A_QUARTER_SECOND;

    SMPL_Ioctl(IOCTL_OBJ_FREQ, IOCTL_ACT_SCAN, &scan);
    if (1 == scan.numChan)
    {
      if (firstTimeThru)
      {
        BSP_TURN_OFF_LED1();
        BSP_TURN_ON_LED2();
        {
          uint8_t i=15;

          while (i--)
          {
            toggleLED(1);
            toggleLED(2);
            SPIN_ABOUT_A_QUARTER_SECOND;
          }
        }
        firstTimeThru = 0;
      }
      switch(freq[0].logicalChan)
      {
        case 0:
          /* GREEN OFF */
          /* RED   OFF */
          BSP_TURN_OFF_LED1();
          BSP_TURN_OFF_LED2();
          break;

        case 1:
          /* GREEN OFF */
          /* RED   ON */
          BSP_TURN_OFF_LED1();
          BSP_TURN_ON_LED2();
          break;

        case 2:
          /* GREEN ON */
          /* RED   OFF */
          BSP_TURN_ON_LED1();
          BSP_TURN_OFF_LED2();
          break;

        case 3:
          /* GREEN ON */
          /* RED   ON */
          BSP_TURN_ON_LED1();
          BSP_TURN_ON_LED2();
          break;

        case 4:
          /* blink them both... */
          BSP_TURN_OFF_LED1();
          BSP_TURN_OFF_LED2();
          SPIN_ABOUT_A_QUARTER_SECOND;
          BSP_TURN_ON_LED1();
          BSP_TURN_ON_LED2();
          SPIN_ABOUT_A_QUARTER_SECOND;
          BSP_TURN_OFF_LED1();
          BSP_TURN_OFF_LED2();
      }
    }
  }
}

static void toggleLED(uint8_t which)
{
  if (1 == which)
  {
    BSP_TOGGLE_LED1();
  }
  else if (2 == which)
  {
    BSP_TOGGLE_LED2();
  }
  return;
}
