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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//! ReadUs64 reads the 1us tick in the assumption of an 64MHz systick clock using the microSecond variable and current systick value.
//! ATTENTION: This is a quick and dirty implementation working well only if this function is called in intervals smaller than 1 ms.
//! :-( Because the STM32G071 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
//! I see no way to find out if the systick ISR was already active shortly after a systick counter wrap, despite calling this
//! function in intervals smaller than 1 ms if not using hardware timers. To make it clear: You can use ReadUs64 to measure long
//! intervals up to 584542 years, but the "OS" needs to call ReadUs64 internally regularely in <1ms intervals.
//! \retval us count since last reset
static inline uint64_t ReadUs64( void ){
    static uint64_t us_1 = 0; // result of last call
    uint64_t us = microSecond + ((SysTick->LOAD - SysTick->VAL) >> 6); // Divide 64MHz clock by 64 to get us part.
    if( us < us_1){ // Possible very close to systick ISR, when milliSecond was not incremented yet, but the systic wrapped already.
        us += 1000; // Time cannot go backwards, so correct the 1ms error in the assumption last call is not longer than 1ms back.
    }
    us_1 = us; // keep result for next call
    return us;
}

//! ReadUs32 reads the 1us tick in the assumption of an 64MHz systick clock using the microSecond variable and current systick value.
//! ATTENTION: This is a quick and dirty implementation working well only if this function is called in intervals smaller than 1 ms.
//! :-( Because the STM32G071 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
//! I see no way to find out if the systick ISR was already active shortly after a systick counter wrap, despite calling this
//! function in intervals smaller than 1 ms if not using hardware timers. To make it clear: You can use ReadUs32 to measure long
//! intervals up to over 1 hour (4294 seconds), but the "OS" needs to call ReadUs32  internally regularely in <1ms intervals.
//! \retval us count since last reset modulo 2^32
uint32_t ReadUs32( void ){
    static uint32_t us_1 = 0; // result of last call
    uint32_t us = ((uint32_t)microSecond) + ((SysTick->LOAD - SysTick->VAL) >> 6); // Divide 64MHz clock by 64 to get us part.
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

static unsigned timingError64Count = 0;
static unsigned timingError32Count = 0;

//! serveUs should be called in intervals secure smaller than 1ms.
static void serveUs( void ){
    static uint64_t st64_1 = 0;
    static uint32_t st32_1 = 0;
    uint64_t st64 = ReadUs64();
    uint32_t st32 = ReadUs32();
    static int virgin64 = 1;
    static int virgin32 = 1;
    if( st64 < st64_1 ){
        timingError64Count++;
        if( virgin64 ){
            virgin64 = 0;
            TRICE64( Id( 7408), "err:st64=%d < st64_1=%d\n", st64, st64_1 );
        
        }
    }
    if( st32 < st32_1 ){
        timingError32Count++;
        if( virgin32 ){
            virgin32 = 0;
            TRICE32( Id( 2140), "err:st32=%d < st32_1=%d\n", st32, st32_1 );        
        }
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD1_STROBE | LL_SYSCFG_UCPD2_STROBE);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // enable SysTick interrupt
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    #ifdef XTEA_ENCRYPT_KEY
        XTEAInitTable();
    #endif
    #ifdef TRICE_UARTA
    LL_USART_EnableIT_RXNE(TRICE_UARTA); // enable UART2 interrupt
    #endif
    TRICE_HEADLINE;
    {
        float a = (float)5.934;
        float b = a + ((a > 0) ? 0.0005f : -0.0005f);
        int c = (int)b;
        int d = (int)(b * 1000) % 1000;
        int e = (int)(1000 * (float)(a - c)); 
        TRICE( Id( 4716), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); //lint !e666
        TRICE( Id( 1570), "1/11 = %g\n", aFloat( 1.0/11 ) ); //lint !e666
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    for(;;){
        if( triceCommandFlag ){
            triceCommandFlag = 0;
            TRICE_S( Id( 5044), "att:Executing command %s ...\n", triceCommandBuffer );
            // do
            TRICE( Id( 2743), "att:...done\n" );
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
            if( milliSecond >= lastTricesTime + 2000 ){
                static int index = 900;
                int select = index;
                TRICE16( Id( 5350),"MSG: 💚 START select = %d\n", select );
                TriceCheckSet(select);
                #if TRICE_MODE == TRICE_DOUBLE_BUFFER
                TRICE16( Id( 4576),"MSG: ✅ STOP  select = %d, TriceDepthMax =%4u of %d\n", select, TriceDepthMax(), TRICE_HALF_BUFFER_SIZE );
                #endif
                #if TRICE_MODE == TRICE_STREAM_BUFFER
                TRICE( Id( 4833), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
                #endif

                if( timingError64Count ){
                    TRICE( Id( 1352), "err:%d timing errors 64-bit\n", timingError64Count );
                }
                if( timingError32Count ){
                    TRICE( Id( 6479), "err:%d timing errors 32-bit\n", timingError32Count );
                }
                index += 10;
                index = index > 1000 ? 0 : index;
                {
                    volatile uint32_t st0 = SysTick->VAL;
                    volatile uint32_t us = ReadUs32();
                    volatile uint32_t st1 = SysTick->VAL;
                    TRICE( Id( 5422), "time: %d µs - ReadUs32() lasts %d ticks\n", us, st0 - st1);
                }
                lastTricesTime = milliSecond;
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(64000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
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

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = USART2_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART2_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USART2_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART2_RX_GPIO_Port, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(USART2);
  LL_USART_ConfigAsyncMode(USART2);

  /* USER CODE BEGIN WKUPType USART2 */

  /* USER CODE END WKUPType USART2 */

  LL_USART_Enable(USART2);

  /* Polling USART2 initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
  {
  }
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
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

  /**/
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
