/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trice.h"
#define TRICE_FILE Id(63911)
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

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
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

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
    extern uint64_t microSecond;
    extern uint32_t milliSecond;
    microSecond += 1000;
    milliSecond++;
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */
#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
    triceTriggerTransmit();
#endif
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
    if (LL_USART_IsActiveFlag_RXNE(TRICE_UART) ) { //  && LL_USART_IsEnabledIT_RXNE(TRICE_UART)) {
        static char command[5];
        static int index = 0;
        uint8_t v;
        if( LL_USART_IsActiveFlag_ORE(USART2) ){
            TRICE( Id(59297), "WARNING:USARTq OverRun Error Flag is set!\n" );
        }
        v = LL_USART_ReceiveData8(TRICE_UART);
        command[index] = v;
        index += index < sizeof(command)-1 ? 1 : 0; 
        if( v == 0 ){ // command end
            TRICE_S( Id(58565), "rx:received command:%s\n", command );
            TRICE_S( Id(39158), "tx:received command:%s\n", command );
            index = 0;
        }
        return;
    } 
/*
    if( LL_USART_IsActiveFlag_PE(USART2) ){
        TRICE( Id( 57921), "err:USART2 Parity Error Flag is set\n" );
        LL_USART_ClearFlag_PE(USART2);
    }
    if( LL_USART_IsActiveFlag_FE(USART2) ){
        TRICE( Id( 46955), "err:USART2 Framing Error Flag is set\n" );
        LL_USART_ClearFlag_FE(USART2);
    }
    if( LL_USART_IsActiveFlag_NE(USART2) ){
        TRICE( Id( 33152), "err:USART2 Noise error detected Flag is set\n" );
        LL_USART_ClearFlag_NE(USART2);
    }
    if( LL_USART_IsActiveFlag_ORE(USART2) ){
        //TRICE( Id( 48567), "err:USART2 OverRun Error Flag is set\n" );
        LL_USART_ClearFlag_ORE(USART2);
    }
    if( LL_USART_IsActiveFlag_IDLE(USART2) ){
        TRICE( Id( 49827), "wrn:USART2 IDLE line detected Flag is set\n" );
        LL_USART_ClearFlag_IDLE(USART2);
    }
    if( LL_USART_IsActiveFlag_TC(USART2) ){
        //TRICE0( Id( 43272), "wr:USART2 Transmission Complete Flag is set\n" );
        LL_USART_ClearFlag_TC(USART2);
    }
    if( LL_USART_IsActiveFlag_nCTS(USART2) ){
        TRICE( Id( 63283), "wrn:USART2 CTS interrupt Flag is set\n" );
        LL_USART_ClearFlag_nCTS(USART2);
    }
    if( LL_USART_IsActiveFlag_CTS(USART2) ){
        TRICE( Id( 44666), "wrn:USART2 CTS Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RTO(USART2) ){
        TRICE( Id( 45795), "err:USART2 Receiver Time Out Flag is set\n" );
        LL_USART_ClearFlag_RTO(USART2);
    }
    if( LL_USART_IsActiveFlag_ABRE(USART2) ){
        TRICE0( Id( 58499), "err:USART2 Auto-Baud Rate Error Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_ABR(USART2) ){
        TRICE( Id( 40821), "wrn:USART2 Auto-Baud Rate Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_BUSY(USART2) ){
        TRICE( Id( 47278), "inf:USART2 Busy Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_CM(USART2) ){
        TRICE( Id( 59432), "wrn:USART2 Character Match Flag is set\n" );
        LL_USART_ClearFlag_CM(USART2);
    }
    if( LL_USART_IsActiveFlag_SBK(USART2) ){
        TRICE( Id( 33033), "wrn:USART2 Send Break Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RWU(USART2) ){
        TRICE( Id( 40309), "wrn:USART2 Receive Wake Up from mute mode Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_TEACK(USART2) ){
        //TRICE( Id( 36833), "wr:USART2 Transmit Enable Acknowledge Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_REACK(USART2) ){
        //TRICE( Id( 47372), "rd:USART2 Receive Enable Acknowledge Flag is set\n" );
    }
*/
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
    triceServeTransmit();
#endif
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
