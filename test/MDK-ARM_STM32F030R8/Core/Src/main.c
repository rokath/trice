/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trice.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t milliSecond = 0;
uint64_t microSecond = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//! ReadUs64 reads the 1us tick in the assumption of an 48MHz systick clock using the microSecond variable and current systick value.
//! ATTENTION: This is a quick and dirty implementation working well only if this function is called in intervals smaller than 1 ms.
//! :-( Because the STM32F030 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
//! I see no way to find out if the systick ISR was already active shortly after a systick counter wrap, despite calling this
//! function in intervals smaller than 1 ms if not using hardware timers. To make it clear: You can use ReadUs64 to measure long
//! intervals up to 584542 years, but the "OS" needs to call ReadUs64 internally regularely in <1ms intervals.
//! \retval us count since last reset
static inline uint64_t ReadUs64( void ){
    static uint64_t us_1 = 0; // result of last call 
    uint64_t us = microSecond + (((SysTick->LOAD - SysTick->VAL) * 87381LL) >> 22); // Divide 48MHz clock by 48,0001831 to get us part.
    if( us < us_1){ // Possible very close to systick ISR, when milliSecond was not incremented yet, but the systic wrapped already.
        us += 1000; // Time cannot go backwards, so correct the 1ms error in the assumption last call is not longer than 1ms back.
    }
    us_1 = us; // keep result for next call
    return us;
}

//! ReadUs32 reads the 1us tick in the assumption of an 48MHz systick clock using the microSecond variable and current systick value.
//! ATTENTION: This is a quick and dirty implementation working well only if this function is called in intervals smaller than 1 ms.
//! :-( Because the STM32F030 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
//! I see no way to find out if the systick ISR was already active shortly after a systick counter wrap, despite calling this
//! function in intervals smaller than 1 ms if not using hardware timers. To make it clear: You can use ReadUs32 to measure long
//! intervals up to over 1 hour (4294 seconds), but the "OS" needs to call ReadUs32  internally regularely in <1ms intervals.
//! \retval us count since last reset modulo 2^32
static inline uint32_t ReadUs32( void ){
    static uint32_t us_1 = 0; // result of last call
    uint32_t us = ((uint32_t)microSecond) + (((SysTick->LOAD - SysTick->VAL) * 87381LL) >> 22); // Divide clock by 48,0001831 to get us.
    if( us < us_1){ // Possible very close to systick ISR, when milliSecond was not incremented yet, but the systic wrapped already.
        us += 1000; // Time cannot go backwards, so correct the 1ms error in the assumption last call is not longer than 1ms back.
    }
    us_1 = us; // keep result for next call
    return us;
}

uint16_t TriceStamp16( void ){
    return (uint16_t)(ReadUs32()%10000); // This implies division and is therefore slow!
}

uint32_t TriceStamp32( void ){
    return ReadUs32();
}

//! serveUs should be called in intervals secure smaller than 1ms.
static void serveUs( void ){
    static uint64_t st64_1 = 0;
    static uint32_t st32_1 = 0;
    uint64_t st64 = ReadUs64();
    uint32_t st32 = ReadUs32();
    if( st64 < st64_1 || st32 < st32_1 ){
        for(;;){} // stop, timing error
    }
    st64_1 = st64;
    st32_1 = st32;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//lint -e835
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */
//lint +e835
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // enable SysTick interrupt
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    #ifdef TRICE_ENCRYPT
        XTEAInitTable();
    #endif
    #ifdef TRICE_UARTA
    LL_USART_EnableIT_RXNE(TRICE_UARTA); 
    #endif
    #ifdef TRICE_UARTB
    LL_USART_EnableIT_RXNE(TRICE_UARTB); 
    #endif
    TRICE_HEADLINE;
    {
        float a = (float)5.934;
        float b = a + ((a > 0) ? 0.0005f : -0.0005f);
        int c = (int)b;
        int d = (int)(b * 1000) % 1000;
        int e = (int)(1000 * (float)(a - c)); 
        TRICE( Id(10762), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); //lint !e666
        TRICE( Id(12531), "1/11 = %g\n", aFloat( 1.0/11 ) ); //lint !e666
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    for(;;){
        if( triceCommandFlag ){
            triceCommandFlag = 0;
            TRICE_S( Id(11673), "att:Executing command %s ...\n", triceCommand );
            // do
            TRICE( Id(12106), "att:...done\n" );
        }

        // serve every few ms
        #if TRICE_DEFERRED_OUT
        static unsigned lastMs = 0;
        if( milliSecond >= lastMs + TRICE_TRANSFER_INTERVAL_MS ){
            lastMs = milliSecond;
            TriceTransfer();
        }
        #endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        {
            static unsigned lastTricesTime = 0;
            // send some trices every few ms
            if( milliSecond >= lastTricesTime + 200 ){
                static int index = 0;
                int select = index;
                TRICE16( ID(12605),"MSG: 💚 START select = %d\n", select );
                TriceCheckSet(select);
                #if TRICE_MODE == TRICE_HALF_BUFFER_SIZE
                TRICE16( Id(14013),"MSG: ✅ STOP  select = %d, TriceDepthMax =%4u of %d\n", select, TriceDepthMax(), TRICE_HALF_BUFFER_SIZE );
                #endif
                #if TRICE_MODE == TRICE_STREAM_BUFFER
                TRICE( Id(11125), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
                #endif
                index += 10;
                index = index > 1000 ? 0 : index;
                lastTricesTime = milliSecond;
                //{
                //    volatile uint32_t st0 = SysTick->VAL;
                //    volatile uint32_t us = ReadUs32();
                //    volatile uint32_t st1 = SysTick->VAL;
                //    TRICE( id(14860), "time: %d µs - ReadUs32() lasts %d ticks\n", us, st0 - st1);
                //}
            }
        }
        serveUs();
        __WFI(); //lint !e718 !e746 wait for interrupt (sleep)
        serveUs();
    }
//lint -e835 -e534
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  PA12   ------> USART1_DE
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 2400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_EVEN;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_EnableDEMode(USART1);
  LL_USART_SetDESignalPolarity(USART1, LL_USART_DE_POLARITY_HIGH);
  LL_USART_SetDEAssertionTime(USART1, 0);
  LL_USART_SetDEDeassertionTime(USART1, 0);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = USART_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USART_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART_RX_GPIO_Port, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART2);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);

  /**/
  LL_GPIO_SetPinPull(B1_GPIO_Port, B1_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(B1_GPIO_Port, B1_Pin, LL_GPIO_MODE_INPUT);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//lint +e835 +e534
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  for(;;)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
