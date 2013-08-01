/*----------------------------------------------------------------------------
 *  Demo Application for SimpliciTI
 *
 *  L. Friedman
 *  Texas Instruments, Inc.
 *---------------------------------------------------------------------------- */

/**********************************************************************************************
  Copyright 2004-2007 Texas Instruments Incorporated. All rights reserved.

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
#include "bsp_leds.h"
#include "nwk_types.h"
#include "nwk_api.h"

#include "app_remap_led.h"

#define SPIN_ABOUT_A_SECOND   NWK_DELAY(1000)

void toggleLED(uint8_t);

void main (void)
{
  BSP_Init();

  /* If an on-the-fly device address is generated it must be done before the
   * call to SMPL_Init(). If the address is set here the ROM value will not
   * be used. If SMPL_Init() runs before this IOCTL is used the IOCTL call
   * will not take effect. One shot only. The IOCTL call below is conformal.
   */
#ifdef I_WANT_TO_CHANGE_DEFAULT_ROM_DEVICE_ADDRESS_PSEUDO_CODE
  {
    addr_t lAddr;

    createRandomAddress(&lAddr);
    SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);
  }
#endif /* I_WANT_TO_CHANGE_DEFAULT_ROM_DEVICE_ADDRESS_PSEUDO_CODE */

  while (SMPL_SUCCESS != SMPL_Init((uint8_t (*)(linkID_t))0))
  {
    toggleLED(1);
    toggleLED(2);
    SPIN_ABOUT_A_SECOND;
  }

  toggleLED(1);
  toggleLED(2);

#ifdef ACCESS_POINT
  /* This code example changes the Link token to be distributed to those who
   * Join. For the example here this should be done before anyone joins so
   * the Join context is defaulted to OFF for this scenario. See the
   * smpl_config.dat file. After the link token is set the Join context must
   * be enabled.
   *
   * NOTE that this is done after initialization. For APs the init sequence
   * consists of a step in which a link token is generated. The sequence here
   * overrides that setting. It can be used to distribute different link tokens
   * to different devices. The sequence here is a simple example of how to use
   * the IOCTL interface to set the Link token for subsequent Joiners.
   *
   * You might want to be careful about following this particular example if you
   * are restoring from NV unless you are setting a fixed value as is done here.
   * Unconditionally setting a random value will make it esentially impossible
   * for newly joining devices to link to devices that joined before the AP was
   * reset since they will have different link tokens.
   */
  {
    ioctlToken_t t;

    t.tokenType       = TT_LINK;
    t.token.linkToken = 0x78563412;

    SMPL_Ioctl(IOCTL_OBJ_TOKEN, IOCTL_ACT_SET, &t);

    /* enable join context */
    SMPL_Ioctl(IOCTL_OBJ_AP_JOIN, IOCTL_ACT_ON, 0);
  }
#endif
  while (1) ;
}

void toggleLED(uint8_t which)
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

