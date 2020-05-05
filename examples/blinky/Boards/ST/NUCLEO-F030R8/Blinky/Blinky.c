/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"
#include "Board_LED.h"                  /* ::Board Support:LED */
#include "Board_Buttons.h"              /* ::Board Support:Buttons */

#include "RTE_Components.h"             /* Component selection */
#include "stm32f0xx_hal.h"              /* Keil::Device:STM32Cube HAL:Common */

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (512U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

static volatile uint32_t delay_val = 500U;

static osThreadId_t tid_thrLED;                /* Thread id of thread: LED */
static osThreadId_t tid_thrBUT;                /* Thread id of thread: BUT */


/*----------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrLED(void *argument) {
  uint32_t led_max    = LED_GetCount();
  uint32_t led_num    = 0U;

  (void)argument;

  for (;;) {
    osThreadFlagsWait(0x0001U, osFlagsWaitAny ,osWaitForever);
    LED_On(led_num);                                           /* Turn specified LED on */
    osThreadFlagsWait(0x0001U, osFlagsWaitAny ,osWaitForever);
    LED_Off(led_num);                                          /* Turn specified LED off */

    led_num++;                                                 /* Change LED number */
    if (led_num >= led_max) {
      led_num = 0U;                                            /* Restart with first LED */
    }
  }

}

/*----------------------------------------------------------------------------
  thrBUT: check button state
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrBUT(void *argument) {
  uint32_t button_msk = (1U << Buttons_GetCount()) - 1U;

  (void)argument;

  for (;;) {
    osDelay(delay_val);                                        /* Wait */
    while (Buttons_GetState() & (button_msk));                 /* Wait while holding USER button */
    osThreadFlagsSet(tid_thrLED, 0x0001U);
  }

}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *argument) {

  (void)argument;

#ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize(0U, 1U);
  EventRecorderEnable (EventRecordError, 0xF0U, 0xF8U);     /* RTOS Events */
  EventRecorderEnable (EventRecordAll, 0xF2U, 0xF2U);       /* Thread Events */
#endif

  LED_Initialize();                                         /* initalize LEDs */
  Buttons_Initialize();                                     /* initalize Buttons */

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);            /* create BUT thread */
  if (tid_thrBUT == NULL) { /* add error handling */ }

  tid_thrLED = osThreadNew (thrLED, NULL, NULL);            /* create LED thread */
  if (tid_thrLED == NULL) { /* add error handling */ }

  for (;;) {}
}
