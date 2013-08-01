/**************************************************************************************************
  Filename:       bsp_board.c
  Revised:        $Date: 2009-10-11 16:48:20 -0700 (Sun, 11 Oct 2009) $
  Revision:       $Revision: 20896 $

  Copyright 2009 Texas Instruments Incorporated.  All rights reserved.

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
 *   Target : Texas Instruments MSP430F2618
 *            "SmartRF05 Board"
 *   Top-level board code file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp.h"
#include "bsp_config.h"
#include "DCO_calibrate.h"

/* ------------------------------------------------------------------------------------------------
 *                                            Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */

/* Timer clk */
#define BSP_TIMER_CLK_MHZ       BSP_CONFIG_CLOCK_MHZ
#define BSP_DELAY_MAX_USEC      (0xFFFF/BSP_TIMER_CLK_MHZ)


#define BSP_CLOCK_TYPE_DCO         0
#define BSP_CLOCK_TYPE_CRYSTAL     1


#define BSP_CLOCK_INIT(x)                            \
{                                                    \
  /* The DCO library does not handle the stack */    \
  /* frame well. As a result, this macro must */     \
  /* be the first thing that runs after main. */     \
  /* Disable watchdog timer */                       \
  WDTCTL = WDTPW | WDTHOLD;                          \
                                                     \
  if (x == BSP_CLOCK_TYPE_CRYSTAL)                   \
  {                                                  \
    /* Configure external crystal */                 \
    /* Turn on XT2 */                                \
    BCSCTL1 &= ~(XT2OFF);                            \
                                                     \
    /* Select SMCLK = XT2 - MCLK = XT2  */           \
    BCSCTL2 |= (SELS | SELM1);                       \
                                                     \
    /* XT2 range select 3 to 16MHz, 10pf */          \
    BCSCTL3 |= (XCAP0 | XCAP1 | XT2S1);              \
                                                     \
    /* wait for oscillator to stabilize */           \
    for (;;)                                         \
    {                                                \
      uint8_t i;                                     \
      uint8_t fault;                                 \
                                                     \
      /* clear oscillator fault flag */              \
      IFG1 &= ~OFIFG;                                \
      fault = 0;                                     \
                                                     \
      /* once fault flag is clear */                 \
      /* several times in a row, continue. */        \
      for (i=0; i<10; i++)                           \
      {                                              \
        fault = fault + (IFG1 & OFIFG);              \
      }                                              \
      if (fault == 0) break;                         \
    }                                                \
  }                                                  \
  else if (x == BSP_CLOCK_TYPE_DCO)                  \
  {                                                  \
    /* Setup the GPIOs for DCO calibration */        \
    /* P1.1 and P1.4 outputs. */                     \
    P1DIR |= 0x12;                                   \
                                                     \
    /* P1.4 SMCLK output */                          \
    P1SEL |= 0x10;                                   \
                                                     \
    /* P2.0 output */                                \
    P2DIR |= 0x01;                                   \
                                                     \
    /* P2.0 ACLK output */                           \
    P2SEL |= 0x01;                                   \
                                                     \
    /* delay for ACLK startup */                     \
    {                                                \
      uint16_t i;                                    \
      for (i=0; i<0xFFFF; i++) {};                   \
    }                                                \
                                                     \
    /* Set DCO at 6MHz */                            \
    BSP_ASSERT(TI_SetDCO(TI_DCO_6MHZ) == 0);         \
  }                                                  \
  else                                               \
  {                                                  \
    /* Unknown clock type */                         \
    BSP_ASSERT(0);                                   \
  }                                                  \
                                                     \
  /* Turn on the timer clock; keep 32KHz running */  \
  __bis_SR_register(SCG0);                           \
}

/* ------------------------------------------------------------------------------------------------
 *                                            Local Variables
 * ------------------------------------------------------------------------------------------------
 */
#if defined(SW_TIMER)
static uint8_t sIterationsPerUsec = 0;
#endif

/**************************************************************************************************
 * @fn          BSP_EARLY_INIT
 *
 * @brief       This function is called by start-up code before doing the normal initialization
 *              of data segments. If the return value is zero, initialization is not performed.
 *              The global macro label "BSP_EARLY_INIT" gets #defined in the bsp_msp430_defs.h
 *              file, according to the specific compiler environment (CCE or IAR). In the CCE
 *              environment this macro invokes "_system_pre_init()" and in the IAR environment
 *              this macro invokes "__low_level_init()".
 *
 * @param       None
 *
 * @return      0 - don't intialize data segments / 1 - do initialization
 **************************************************************************************************
*/
BSP_EARLY_INIT(void)
{
  /* Disable watchdog timer */
  WDTCTL = WDTPW | WDTHOLD;

  /* Return 1 - run seg_init */
  return (1);
}

/**************************************************************************************************
 * @fn          BSP_InitBoard
 *
 * @brief       Initialize the board.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
void BSP_InitBoard(void)
{
  /* MCU clk is driven by internal Digitally Controlled Oscillator (DCO).
   * The DCO needs to be calibrated before use.
   */
  BSP_CLOCK_INIT(BSP_CLOCK_TYPE_DCO);

  /* Configure TimerA for use by the delay function */

  /* Reset the timer */
  TACTL |= TACLR; /* Set the TACLR */

  /* Clear all settings */
  TACTL = 0x0;

  /* Select the clk source to be - SMCLK (Sub-Main CLK)*/
  TACTL |= TASSEL_2;

#if defined(SW_TIMER)
#define MHZ_CLOCKS_PER_USEC      BSP_CLOCK_MHZ
#define MHZ_CLOCKS_PER_ITERATION 10

  sIterationsPerUsec = (uint8_t)(((MHZ_CLOCKS_PER_USEC)/(MHZ_CLOCKS_PER_ITERATION))+.5);

  if (!sIterationsPerUsec)
  {
    sIterationsPerUsec = 1;
  }
#endif   /* SW_TIMER */
}

/**************************************************************************************************
 * @fn          BSP_Delay
 *
 * @brief       Sleep for the requested amount of time.
 *
 * @param       # of microseconds to sleep.
 *
 * @return      none
 **************************************************************************************************
 */
void BSP_Delay(uint16_t usec)
#if !defined(SW_TIMER)
{
  BSP_ASSERT(usec < BSP_DELAY_MAX_USEC);

  TAR = 0; /* initial count */
  TACCR0 = BSP_TIMER_CLK_MHZ*usec; /* compare count. (delay in ticks) */

  /* Start the timer in UP mode */
  TACTL |= MC_1;

  /* Loop till compare interrupt flag is set */
  while(!(TACCTL0 & CCIFG));

  /* Stop the timer */
  TACTL &= ~(MC_1);

  /* Clear the interrupt flag */
   TACCTL0 &= ~CCIFG;
}
#else  /* !SW_TIMER */
{
  /* Declared 'volatile' in case User optimizes for speed. This will
   * prevent the optimizer from eliminating the loop completely. But
   * it also generates more code...
   */
  volatile uint16_t repeatCount = sIterationsPerUsec*usec;

  while (repeatCount--) ;

  return;
}
#endif  /* !SW_TIMER */

/**************************************************************************************************
*/




