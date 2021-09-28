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
#include <stdlib.h> // srand
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
int milliSecond = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define TRICE_BUFFER_SIZE 256

static uint32_t triceBuffer[2][TRICE_BUFFER_SIZE>>2] = {0}; //!< triceBuffer is double buffer for better write speed.
int TriceDepthMax = 0; //!< TriceDepthMax is a diagnostics value.
#define TRICE_ACTIVE 0 //!< TRICE_ACTIVE is the init value for swap.
static int swap = TRICE_ACTIVE; //!< swap is the active write buffer. !swap is the active read buffer.
static uint32_t* wTb = &triceBuffer[TRICE_ACTIVE][0]; //!< wTb is the active write position.
static uint32_t* rTb = &triceBuffer[!TRICE_ACTIVE][0]; //!< rTb is the active read position.
static uint8_t cycle = 80; //!< trice cycle counter


//! triceRead returns a pointer to next complete trice message or it returns NULL if no data to process.
//! If in a first try the buffer is empty a buffer swap is done:
//! \li Switch next write to this buffer here, because it is empty.
//! \li Switch next read to the other buffer, may be there is stuff to read.
//! If both buffers empty each triceRead call results in a buffer swap, what is ok.
//! There is no wTp overflow check! The read buffer must be read out fast enough to be swapped before the write buffer can overflow.
uint32_t* triceRead( void ){
    uint8_t* p;
    int triceDepth = &triceBuffer[swap][0] - wTb;                            // diagnostics
    TriceDepthMax = triceDepth < TriceDepthMax ? TriceDepthMax : triceDepth; // diagnostics
    if( 0 == *rTb ){ // This buffer is empty
        TRICE_ENTER_CRITICAL_SECTION
        *wTb = 0; // write end marker
        swap = !swap;
        wTb = &triceBuffer[swap][0];
        rTb = &triceBuffer[!swap][0];
        TRICE_LEAVE_CRITICAL_SECTION
        if( 0 == *rTb ){ // This buffer is empty
            return (uint32_t*)0;
        }
    } 
    p = (uint8_t*)rTb;
    rTb += (p[1]+7)>>2; // step to next entry (little endian)
    return (uint32_t*)p;
}

void ServeTriceTranslation( void ){
    uint32_t* p;
    uint8_t clen, tlen;
    uint8_t* q;
    if( triceU8FifoDepth() ){
        return; // transmission not done yet
    }
    p = triceRead();
    if( (uint32_t*)0 == p ){
        return; // no trice data to transmit
    }
    q = (uint8_t*)p;
    tlen = q[1] + 4; // little endian
    q[1] = q[0]; // write cycle to 2nd position (little endian assumed!)
    clen = triceCOBSREncode(triceU8Fifo, &q[1], tlen);
    triceU8Fifo[clen] = 0; // add 0-delimiter
    //memcpy(triceU8Fifo, q, tlen );
    TRICE_ENTER_CRITICAL_SECTION
    triceU8FifoWriteIndex = clen+1;
    triceU8FifoReadIndex = 0;
    TRICE_LEAVE_CRITICAL_SECTION
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#ifdef ENCRYPT
    InitXteaTable();
#endif
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
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
    LL_USART_EnableIT_RXNE(TRICE_UART); // enable UART2 interrupt
    //TRICE_HEADLINE;
    //TRICE16_1( Id( 44374), "tst:Hi! %x\n", 0x1122 );
    srand(SYSTICKVAL16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    static int lastTricesTime = 0;
    { // send some trices every few ms
        if( milliSecond >= lastTricesTime + 1000 ){
            TRICE0( Id( 48738), "tst:Hi!\n" );                     // [33 230 0]        ...  [34 230 0]
            TRICE8_1( Id( 36005), "tst:Hi! %d\n", 1 );             // [4 202 81 1 0]    ...  [4 202 82 1 0]
            TRICE8_2( Id( 53180), "tst:Hi! %d %d\n", 1, 2 );       // [5 251 195 1 2 0] ...  [5 251 196 1 2 0]
            TRICE8_3( Id( (5*256 + 6)), "tst:Hi! %d %d %d\n", 1, 2, 3 ); // [6 28 67 1 2 3 0] ...  [6 28 68 1 2 3 0]
            TRICE8_3( Id( (8*256 + 9)), "tst:Hi! %d %d %d\n", 11, 12, 13 ); // [6 28 67 1 2 3 0] ...  [6 28 68 1 2 3 0]
          //TRICE16_1( Id( 44374), "tst:Hi! %x\n", 0x1213 );       // [18 213 103 19 0] ...  [18 213 103 19 0]


            //static int index = 0;
            //int select = index % 2;
            //#if TRICE_FLEX_ENCODING == TRICE_ENCODING
            //TRICE16_2( Id(1047663),"MSG: triceFifoMaxDepth = %d, select = %d\n", triceFifoMaxDepth, select ); // bigID check
            //#else
            //TRICE16_2( Id(47663),"MSG: triceFifoMaxDepth = %d, select = %d\n", triceFifoMaxDepth, select );
            //#endif
            //triceCheckSet(select);
            //index++;
            lastTricesTime = milliSecond;
        }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        { // serve every few ms
            static int lastMs = 0;
            if( milliSecond >= lastMs + 100 ){
                lastMs = milliSecond;
                ServeTriceTranslation();
                #ifdef ENCRYPT
                triceServeFifoEncryptedToBytesBuffer();
                #endif
            }
        }
  }
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
