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

static void linkTo(void);
static void linkFrom(void);

void toggleLED(uint8_t);

static uint8_t sTid = 0;

#define SPIN_ABOUT_A_SECOND   NWK_DELAY(1000)
#define SPIN_ABOUT_3_SECONDS  NWK_DELAY(3000)

void main (void)
{
  uint8_t button = 0;

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

  /* Keep trying to join (a side effect of successful initialization) until
   * successful. Toggle LEDS to indicate that joining has not occurred.
   */
  while (SMPL_SUCCESS != SMPL_Init((uint8_t (*)(linkID_t))0))
  {
    toggleLED(1);
    toggleLED(2);
    SPIN_ABOUT_A_SECOND;
  }


  /* LEDs on solid to indicate successful join. */
  BSP_TURN_ON_LED1();
  BSP_TURN_ON_LED2();

#if !defined( BSP_BOARD_EZ430RF ) && !defined( BSP_BOARD_RFUSB ) && !defined( BSP_BOARD_CC430EM )
  /* watch for button press to select context */
  do {
    if (BSP_BUTTON1())
    {
      button = 1;
    }
    else if (BSP_BUTTON2())
    {
      button = 2;
    }
  } while (!button);
#else
  /* only one button on the eZ430, RFUSB, and CC430EM boards. */
  button = 1;
  while (!BSP_BUTTON1()) ;

  SPIN_ABOUT_3_SECONDS;

  if (!BSP_BUTTON1())
  {
    /* switch button was released. emulate Button 2 */
    button = 2;
  }
#endif
  switch(button)
  {
    case 1:
      /* I'm the linker */
      toggleLED(1);
      linkTo();
      break;
    case 2:
      /* I'm the linkee */
      toggleLED(2);
      linkFrom();
      break;
    default:
      break;
  }

  while (1) ;
}

static void linkFrom()
{
   linkID_t            linkID1;
   uint8_t             msg[2], len, ltid;

   /* listen for link forever... */
   while (1)
   {
     if (SMPL_SUCCESS == SMPL_LinkListen(&linkID1))
     {
       break;
     }
     /* Implement fail-to-link policy here. Otherwise, listen again. */
   }

   toggleLED(1);


   while (1)
   {
     /* sleeping... */
     SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);
     SPIN_ABOUT_A_SECOND;  /* emulate MCU sleeping */
     SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);

     /* Any meesages "received"?
      *
      *   The receive call results in polling the Access Point. The success
      *   case occurs when a payload is actually returned. When there is no
      *   frame waiting for the device a frame with no payload is returned by
      *   the Access Point. Note that this loop will retrieve any and all
      *   frames that are waiting for this device on the specified link ID.
      *   This call will also return frames that were received directly. It is
      *   possible to get frames that were repeated either from the initial
      *   transmission from the peer or via a Range Extender. This is why we
      *   implement the TID check.
      */

     while ((SMPL_SUCCESS == SMPL_Receive(linkID1, msg, &len)) && len)
     {
       /* Check the application sequence number to detect
        * late or missing frames...
        */
       if ((ltid=(*(msg+1))))
       {
         /* If the current TID is non-zero and the last one we saw was
          * less than this one assume we've received the 'next' one.
          */
         if (sTid < ltid)
         {
           /* 'next' frame. may have missed some but we don't care. */
           if ((*msg == 1) || (*msg == 2))
           {
             /* we're good. toggle LED */
             toggleLED(*msg);
           }
           /* remember last TID. */
           sTid = ltid;
         }
         /* If current TID non-zero and less than or equal to the last one we saw
          * assume we received a duplicate. Just ignaore it.
          */
       }
       else
       {
         /* Current TID zero. count wrapped (or we just started). let's just accept
          * it and start over.
          */
         if ((*msg == 1) || (*msg == 2))
         {
           /* we're good. toggle LED */
           toggleLED(*msg);
         }
         /* remember last TID. */
         sTid = ltid;
       }
     }
   }
}

static void linkTo()
{
  linkID_t linkID1;
  uint8_t  msg[2], wrap = 0;

  toggleLED(2);

  /* keep trying to link... */
  while (SMPL_SUCCESS != SMPL_Link(&linkID1))
  {
    toggleLED(1);
    toggleLED(2);
    SPIN_ABOUT_A_SECOND;
  }

  if (BSP_LED2_IS_ON())
  {
    toggleLED(2);
  }

#ifdef FREQUENCY_AGILITY
  /* Radio comes up with Rx off. If Frequency Agility is enabled we need to
   * be on so we don't miss a channel change broadcast. We need to hear this
   * since this application has no ack so the broadcast from the AP is the only
   * way to find out about a channel change.
   */
  SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0);
#endif

  /* Set up initial message */
  msg[0] = 2;
  msg[1] = ++sTid;

  while (1)
  {
    /* Send a message every 5 seconds. This could be emulating a sleep. */
#ifndef FREQUENCY_AGILITY
    /* If no FA we don't need to listen for the broadcast channel change command */
    SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);
#endif
    SPIN_ABOUT_A_SECOND;
    SPIN_ABOUT_A_SECOND;
    SPIN_ABOUT_A_SECOND;
    SPIN_ABOUT_A_SECOND;
    SPIN_ABOUT_A_SECOND;
#ifndef FREQUENCY_AGILITY
    /* See comment above...If Frequency Agility is not enabled we never listen
     * so it is OK if we just awaken leaving Rx off.
     */
    SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);
#endif

    if (SMPL_SUCCESS == SMPL_Send(linkID1, msg, sizeof(msg)))
    {
      /* toggle LED 1 to indicate a send */
      toggleLED(1);

      /* Set up next message. Every 8th message toggle LED 1 instead of LED 2 on the peer. */
      msg[0] = (++wrap & 0x7) ? 2 : 1;
      msg[1] = ++sTid;
    }
  }
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

