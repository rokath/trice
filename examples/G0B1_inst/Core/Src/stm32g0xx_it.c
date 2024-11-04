/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trice.h"
#include "triceUart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim17;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM17, FDCAN1_IT1 and FDCAN2_IT1 Interrupt.
  */
void TIM17_FDCAN_IT1_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_FDCAN_IT1_IRQn 0 */

  /* USER CODE END TIM17_FDCAN_IT1_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM17_FDCAN_IT1_IRQn 1 */
#if !TRICE_OFF
  static int i = 0;
  if( i++>25000 ){
    trice("isr:TIM17_FDCAN_IT1_IRQHandler! (%u ms)\n", i );
    i = 0;
  }
#endif
  /* USER CODE END TIM17_FDCAN_IT1_IRQn 1 */
}

/**
  * @brief This function handles USART2 + LPUART2 Interrupt.
  */
void USART2_LPUART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_LPUART2_IRQn 0 */

  /* USER CODE END USART2_LPUART2_IRQn 0 */

  /* USER CODE BEGIN USART2_LPUART2_IRQn 1 */
#if !TRICE_OFF && defined( TRICE_UARTA ) && ((TRICE_BUFFER == TRICE_DOUBLE_BUFFER) || (TRICE_BUFFER == TRICE_RING_BUFFER) ) // buffered out to UARTA
    if( LL_USART_IsActiveFlag_TXE(TRICE_UARTA) ){ // Transmit Data Register Empty Flag
        triceServeTransmitUartA();
    }
#endif
  /* USER CODE END USART2_LPUART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
