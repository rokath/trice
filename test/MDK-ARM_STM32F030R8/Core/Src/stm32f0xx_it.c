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
//#define TRICE_FILE Id(11311)
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
  for(;;)
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
  for(;;)
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
#if defined( TRICE_UART )
    if (LL_USART_IsActiveFlag_RXNE(TRICE_UART) ) { // Read Data Register Not Empty Flag 
        static char rxBuf[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0
        static int index = 0;
        uint8_t v;
        if( LL_USART_IsActiveFlag_ORE(USART2) ){
            TRICE( Id(11614), "WARNING:USARTq OverRun Error Flag is set!\n" );
        }
        v = LL_USART_ReceiveData8(TRICE_UART); // implicit clears the flag
        rxBuf[index] = (char)v;
        index += index < TRICE_COMMAND_SIZE_MAX ? 1 : 0; 
        if( v == 0 ){ // command end
            TRICE_S( Id(11045), "rx:received command:%s\n", rxBuf );
            strcpy(triceCommand, rxBuf );
            triceCommandFlag = 1;
            index = 0;
        }
        return;
    }
#endif // #if defined( TRICE_UART )
    // If both flags active and only one was served, the IRQHandler gets activated again.

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
    if( LL_USART_IsActiveFlag_TXE(TRICE_UART) ){ // Transmit Data Register Empty Flag
        triceServeTransmit();
        return;
    }
#endif

    /*
    if( LL_USART_IsActiveFlag_PE(TRICE_UART) ){
        TRICE( Id(14104), "err:TRICE_UART Parity Error Flag is set\n" );
        LL_USART_ClearFlag_PE(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_FE(TRICE_UART) ){
        TRICE( Id(13315), "err:TRICE_UART Framing Error Flag is set\n" );
        LL_USART_ClearFlag_FE(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_NE(TRICE_UART) ){
        TRICE( Id(10430), "err:TRICE_UART Noise error detected Flag is set\n" );
        LL_USART_ClearFlag_NE(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_ORE(TRICE_UART) ){
        //TRICE( Id(10816), "err:TRICE_UART OverRun Error Flag is set\n" );
        LL_USART_ClearFlag_ORE(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_IDLE(TRICE_UART) ){
        TRICE( Id(11121), "wrn:TRICE_UART IDLE line detected Flag is set\n" );
        LL_USART_ClearFlag_IDLE(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_TC(TRICE_UART) ){
        //TRICE0( Id(11689), "wr:TRICE_UART Transmission Complete Flag is set\n" );
        LL_USART_ClearFlag_TC(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_nCTS(TRICE_UART) ){
        TRICE( Id(15914), "wrn:TRICE_UART CTS interrupt Flag is set\n" );
        LL_USART_ClearFlag_nCTS(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_CTS(TRICE_UART) ){
        TRICE( Id(13861), "wrn:TRICE_UART CTS Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RTO(TRICE_UART) ){
        TRICE( Id(11097), "err:TRICE_UART Receiver Time Out Flag is set\n" );
        LL_USART_ClearFlag_RTO(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_ABRE(TRICE_UART) ){
        TRICE0( Id(10611), "err:TRICE_UART Auto-Baud Rate Error Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_ABR(TRICE_UART) ){
        TRICE( Id(12161), "wrn:TRICE_UART Auto-Baud Rate Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_BUSY(TRICE_UART) ){
        TRICE( Id(12213), "inf:TRICE_UART Busy Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_CM(TRICE_UART) ){
        TRICE( Id(13978), "wrn:TRICE_UART Character Match Flag is set\n" );
        LL_USART_ClearFlag_CM(TRICE_UART);
    }
    if( LL_USART_IsActiveFlag_SBK(TRICE_UART) ){
        TRICE( Id(15466), "wrn:TRICE_UART Send Break Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RWU(TRICE_UART) ){
        TRICE( Id(13989), "wrn:TRICE_UART Receive Wake Up from mute mode Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_TEACK(TRICE_UART) ){
        //TRICE( Id(11751), "wr:TRICE_UART Transmit Enable Acknowledge Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_REACK(TRICE_UART) ){
        //TRICE( Id(10572), "rd:TRICE_UART Receive Enable Acknowledge Flag is set\n" );
    }
*/
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
