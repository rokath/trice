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
#define TRICE_FILE Id(50869)
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
//! :-( Because the STM32G031 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
//! I see no way to find out if the systick ISR was already active shortly after a systick counter wrap, despite calling this
//! function in intervals smaller than 1 ms if not using hardware timers. To make it clear: You can use ReadUs64 to measure long
//! intervals up to 584542 years, but the "OS" needs to call ReadUs64 internally regularely in <1ms intervals.
//! \retval us count since last reset
uint64_t ReadUs64( void ){
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
//! :-( Because the STM32G031 has no 32-bit sysclock counter we need to compute this value or concatenate two 16-bit timers. )
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

uint32_t ReadTime( void ){
    return ReadUs32();
}

//! serveUs should be called in intervals secure smaller than 1ms.
static void serveUs( void ){
    static uint64_t st64_1 = 0;
    static uint32_t st32_1 = 0;
    uint64_t st64 = ReadUs64();
    uint32_t st32 = ReadUs32();
    if( st64 < st64_1 || st32 < st32_1 ){
        while(1); // stop, timing error
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
  #ifdef TRICE_ENCRYPT
    TriceInitXteaTable();
  #endif
    #ifdef TRICE_UART
    LL_USART_EnableIT_RXNE(TRICE_UART); // enable UART2 interrupt
    #endif
    TRICE_HEADLINE;
    {
        float a = 5.934;
        float b = a + ((a > 0) ? 0.0005f : -0.0005f);
        int c = b;
        int d = (int)(b * 1000) % 1000;
        int e = 1000 * (float)(a - c); 
        TRICE( Id(38382), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e );
        TRICE( Id(60363), "1/11 = %g\n", aFloat( 1.0/11 ) );
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1){
        if( triceCommandFlag ){
            triceCommandFlag = 0;
            TRICE_S( Id(48217), "att:Executing command %s ...\n", triceCommand );
            // do
            TRICE( Id(64230), "att:...done\n" );
        }

        // serve every few ms
        #ifdef TRICE_HALF_BUFFER_SIZE
        static int lastMs = 0;
        if( milliSecond >= lastMs + TRICE_TRANSFER_INTERVAL_MS ){
            lastMs = milliSecond;
            TriceTransfer();
        }
        #endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        {
            static int lastTricesTime = 0;
            // send some trices every few ms
            if( milliSecond >= lastTricesTime + 1000 ){
                static int index = 0;
                int select = index;
                TRICE16( Id(50543),"MSG: 💚 START select = %d, TriceDepthMax =%4u\n", select, TriceDepthMax() );
                TriceCheckSet(select);
                TRICE16( Id(40126),"MSG: ✅ STOP  select = %d, TriceDepthMax =%4u\n", select, TriceDepthMax() );
                index += 10;
                index = index > 1000 ? 0 : index;
                lastTricesTime = milliSecond;
                {
                    volatile uint32_t st0 = SysTick->VAL;
                    volatile uint32_t us = ReadUs32();
                    volatile uint32_t st1 = SysTick->VAL;
                    TRICE( Id(47239), "time: %d µs - ReadUs32() lasts %d ticks\n", us, st0 - st1);
                }
            }
        }
        serveUs();
        __WFI(); // wait for interrupt (sleep)
        serveUs();
    }
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
  GPIO_InitStruct.Pin = T_VCP_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(T_VCP_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = T_VCP_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(T_VCP_RX_GPIO_Port, &GPIO_InitStruct);

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
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LD3_GPIO_Port, LD3_Pin);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTF, LL_EXTI_CONFIG_LINE2);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(T_NRST_GPIO_Port, T_NRST_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(T_NRST_GPIO_Port, T_NRST_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

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
  while (1)
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
