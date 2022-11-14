/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
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
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trice.h"
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
    serveUs();
    microSecond += 1000;
    milliSecond++;
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */
#if defined( TRICE_UARTA ) && TRICE_DEFERRED_OUT // buffered out to UART
    triceTriggerTransmitUartA();
#endif
#if defined( TRICE_UARTB ) && TRICE_DEFERRED_OUT // buffered out to UART
    triceTriggerTransmitUartB();
#endif
    serveUs();    
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
    serveUs();
  /* USER CODE BEGIN USART1_IRQn 0 */
#if defined( TRICE_UARTB )
    if (LL_USART_IsActiveFlag_RXNE(TRICE_UARTB) ) { // Read Data Register Not Empty Flag 
        static char rxBuf[TRICE_COMMAND_SIZE_MAX]; // with terminating 0
        static int index = 0;
        uint8_t v;
        if( LL_USART_IsActiveFlag_ORE(TRICE_UARTB) ){
            TRICE( Id( 1583), "WARNING:USART1 OverRun Error Flag is set!\n" );
        }
        v = LL_USART_ReceiveData8(TRICE_UARTB); // implicit clears the flag
        rxBuf[index] = (char)v;
        index += index < TRICE_COMMAND_SIZE_MAX ? 1 : 0; 
        if( v == 0 ){ // command end
            TRICE( id( 6877), "inf:rx:USART1 rx:" );
            TRICE_B( id( 4546), " %02x", rxBuf, index );
            TRICE( id( 7104), "\n" );
            strcpy(triceCommand, rxBuf );
            triceCommandFlag = 1;
            index = 0;
        }
        serveUs();
        return;
    }
#endif // #if defined( TRICE_UARTB )
    // If both flags active and only one was served, the IRQHandler gets activated again.

#if defined( TRICE_UARTB ) && TRICE_DEFERRED_OUT // buffered out to UARTA
    if( LL_USART_IsActiveFlag_TXE(TRICE_UARTB) ){ // Transmit Data Register Empty Flag
        triceServeTransmitUartB();
        serveUs();
        return;
    }
#endif

    /*
    if( LL_USART_IsActiveFlag_PE(TRICE_UARTB) ){
        TRICE( Id( 6905), "err:TRICE_UARTB Parity Error Flag is set\n" );
        LL_USART_ClearFlag_PE(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_FE(TRICE_UARTB) ){
        TRICE( Id( 5074), "err:TRICE_UARTB Framing Error Flag is set\n" );
        LL_USART_ClearFlag_FE(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_NE(TRICE_UARTB) ){
        TRICE( Id( 5496), "err:TRICE_UARTB Noise error detected Flag is set\n" );
        LL_USART_ClearFlag_NE(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_ORE(TRICE_UARTB) ){
        //TRICE( Id( 6115), "err:TRICE_UARTB OverRun Error Flag is set\n" );
        LL_USART_ClearFlag_ORE(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_IDLE(TRICE_UARTB) ){
        TRICE( Id( 4465), "wrn:TRICE_UARTB IDLE line detected Flag is set\n" );
        LL_USART_ClearFlag_IDLE(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_TC(TRICE_UARTB) ){
        //TRICE0( Id( 5199), "wr:TRICE_UARTB Transmission Complete Flag is set\n" );
        LL_USART_ClearFlag_TC(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_nCTS(TRICE_UARTB) ){
        TRICE( Id( 1037), "wrn:TRICE_UARTB CTS interrupt Flag is set\n" );
        LL_USART_ClearFlag_nCTS(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_CTS(TRICE_UARTB) ){
        TRICE( Id( 4927), "wrn:TRICE_UARTB CTS Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RTO(TRICE_UARTB) ){
        TRICE( Id( 1079), "err:TRICE_UARTB Receiver Time Out Flag is set\n" );
        LL_USART_ClearFlag_RTO(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_ABRE(TRICE_UARTB) ){
        TRICE0( Id( 4325), "err:TRICE_UARTB Auto-Baud Rate Error Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_ABR(TRICE_UARTB) ){
        TRICE( Id( 7490), "wrn:TRICE_UARTB Auto-Baud Rate Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_BUSY(TRICE_UARTB) ){
        TRICE( Id( 4290), "inf:TRICE_UARTB Busy Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_CM(TRICE_UARTB) ){
        TRICE( Id( 4181), "wrn:TRICE_UARTB Character Match Flag is set\n" );
        LL_USART_ClearFlag_CM(TRICE_UARTB);
    }
    if( LL_USART_IsActiveFlag_SBK(TRICE_UARTB) ){
        TRICE( Id( 3302), "wrn:TRICE_UARTB Send Break Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RWU(TRICE_UARTB) ){
        TRICE( Id( 3652), "wrn:TRICE_UARTB Receive Wake Up from mute mode Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_TEACK(TRICE_UARTB) ){
        //TRICE( Id( 5009), "wr:TRICE_UARTB Transmit Enable Acknowledge Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_REACK(TRICE_UARTB) ){
        //TRICE( Id( 7453), "rd:TRICE_UARTB Receive Enable Acknowledge Flag is set\n" );
    }
*/
  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */
    serveUs();
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
    serveUs();
  /* USER CODE BEGIN USART2_IRQn 0 */
#if defined( TRICE_UARTA )
    if (LL_USART_IsActiveFlag_RXNE(TRICE_UARTA) ) { // Read Data Register Not Empty Flag 
        static char rxBuf[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0
        static int index = 0;
        uint8_t v;
        if( LL_USART_IsActiveFlag_ORE(TRICE_UARTA) ){
            TRICE( Id( 7108), "WARNING:USARTq OverRun Error Flag is set!\n" );
        }
        v = LL_USART_ReceiveData8(TRICE_UARTA); // implicit clears the flag
        rxBuf[index] = (char)v;
        index += index < TRICE_COMMAND_SIZE_MAX ? 1 : 0; 
        if( v == 0 ){ // command end
            TRICE( id( 1219), "inf:rx:USART2 rx:" );
            TRICE_B( id( 4436), " %02x ", rxBuf, index );
            TRICE( id( 5258), "\n" );
            strcpy(triceCommand, rxBuf );
            triceCommandFlag = 1;
            index = 0;
        }
        serveUs();
        return;
    }
#endif // #if defined( TRICE_UARTA )
    // If both flags active and only one was served, the IRQHandler gets activated again.

#if defined( TRICE_UARTA ) && TRICE_DEFERRED_OUT // buffered out to UARTA
    if( LL_USART_IsActiveFlag_TXE(TRICE_UARTA) ){ // Transmit Data Register Empty Flag
        triceServeTransmitUartA();
        serveUs();
        return;
    }
#endif

    /*
    if( LL_USART_IsActiveFlag_PE(TRICE_UARTA) ){
        TRICE( Id( 5529), "err:TRICE_UARTA Parity Error Flag is set\n" );
        LL_USART_ClearFlag_PE(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_FE(TRICE_UARTA) ){
        TRICE( Id( 7172), "err:TRICE_UARTA Framing Error Flag is set\n" );
        LL_USART_ClearFlag_FE(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_NE(TRICE_UARTA) ){
        TRICE( Id( 3102), "err:TRICE_UARTA Noise error detected Flag is set\n" );
        LL_USART_ClearFlag_NE(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_ORE(TRICE_UARTA) ){
        //TRICE( Id( 4872), "err:TRICE_UARTA OverRun Error Flag is set\n" );
        LL_USART_ClearFlag_ORE(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_IDLE(TRICE_UARTA) ){
        TRICE( Id( 6534), "wrn:TRICE_UARTA IDLE line detected Flag is set\n" );
        LL_USART_ClearFlag_IDLE(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_TC(TRICE_UARTA) ){
        //TRICE0( Id( 2824), "wr:TRICE_UARTA Transmission Complete Flag is set\n" );
        LL_USART_ClearFlag_TC(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_nCTS(TRICE_UARTA) ){
        TRICE( Id( 2797), "wrn:TRICE_UARTA CTS interrupt Flag is set\n" );
        LL_USART_ClearFlag_nCTS(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_CTS(TRICE_UARTA) ){
        TRICE( Id( 4643), "wrn:TRICE_UARTA CTS Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RTO(TRICE_UARTA) ){
        TRICE( Id( 2215), "err:TRICE_UARTA Receiver Time Out Flag is set\n" );
        LL_USART_ClearFlag_RTO(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_ABRE(TRICE_UARTA) ){
        TRICE0( Id( 2766), "err:TRICE_UARTA Auto-Baud Rate Error Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_ABR(TRICE_UARTA) ){
        TRICE( Id( 5656), "wrn:TRICE_UARTA Auto-Baud Rate Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_BUSY(TRICE_UARTA) ){
        TRICE( Id( 7474), "inf:TRICE_UARTA Busy Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_CM(TRICE_UARTA) ){
        TRICE( Id( 2270), "wrn:TRICE_UARTA Character Match Flag is set\n" );
        LL_USART_ClearFlag_CM(TRICE_UARTA);
    }
    if( LL_USART_IsActiveFlag_SBK(TRICE_UARTA) ){
        TRICE( Id( 7928), "wrn:TRICE_UARTA Send Break Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_RWU(TRICE_UARTA) ){
        TRICE( Id( 5225), "wrn:TRICE_UARTA Receive Wake Up from mute mode Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_TEACK(TRICE_UARTA) ){
        //TRICE( Id( 2024), "wr:TRICE_UARTA Transmit Enable Acknowledge Flag is set\n" );
    }
    if( LL_USART_IsActiveFlag_REACK(TRICE_UARTA) ){
        //TRICE( Id( 6192), "rd:TRICE_UARTA Receive Enable Acknowledge Flag is set\n" );
    }
*/
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
    serveUs();
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

