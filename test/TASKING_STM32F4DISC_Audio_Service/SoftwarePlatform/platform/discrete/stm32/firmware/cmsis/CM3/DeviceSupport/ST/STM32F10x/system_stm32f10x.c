/**
  ******************************************************************************
  * @file    system_stm32f10x.c
  * @author  MCD Application Team
  * @version V3.6.1
  * @date    09-March-2012
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  *
  * 1.  This file provides two functions and one global variable to be called from
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      factors, AHB/APBx prescalers and Flash settings).
  *                      This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f10x_xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the HSI (8 MHz) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32f10x_xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HSI still used as system clock source. User can
  *    add some code to deal with this issue inside the SetSysClock() function.
  *
  * 4. The default value of HSE crystal is set to 8 MHz (or 25 MHz, depedning on
  *    the product used), refer to "HSE_VALUE" define in "stm32f10x.h" file.
  *    When HSE is used as system clock source, directly or through PLL, and you
  *    are using different crystal you have to adapt the HSE value to your own
  *    configuration.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f10x_system
  * @{
  */

/** @addtogroup STM32F10x_System_Private_Includes
  * @{
  */

#include "stm32f10x.h"
#include "stm32f10x_cmsis_cfg.h"

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_Defines
  * @{
  */

/*!< Uncomment the following line if you need to use external SRAM mounted
     on STM3210E-EVAL board (STM32 High density and XL-density devices) or on
     STM32100E-EVAL board (STM32 High-density value line devices) as data memory */
#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
/* #define DATA_IN_ExtSRAM */
#endif

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field.
                                  This value must be a multiple of 0x200. */

#if STM32F10X_CMSIS_CLKSRC == STM32F10X_CMSIS_CLKSRC_HSI
#endif
#if STM32F10X_CMSIS_CLKSRC == STM32F10X_CMSIS_CLKSRC_HSE
#  define SYSCLOCK_SETUP_HSE
#  define SYSCLOCK_CLKSRC  RCC_CFGR_SW_HSE
#endif
#if STM32F10X_CMSIS_CLKSRC == STM32F10X_CMSIS_CLKSRC_PLLCLKBYHSI
#  define SYSCLOCK_SETUP_PLL
#  define SYSCLOCK_PLL_INPUT  RCC_CFGR_PLLSRC_HSI_Div2
#  define SYSCLOCK_CLKSRC  RCC_CFGR_SW_PLL
#endif
#if STM32F10X_CMSIS_CLKSRC == STM32F10X_CMSIS_CLKSRC_PLLCLKBYHSE
#  define SYSCLOCK_SETUP_HSE
#  define SYSCLOCK_SETUP_PLL
#  ifdef RCC_CFGR_PLLSRC_HSE
#    define SYSCLOCK_PLL_INPUT  RCC_CFGR_PLLSRC_HSE
#  else
#    define SYSCLOCK_PLL_INPUT  RCC_CFGR_PLLSRC_PREDIV1
#  endif
#  define SYSCLOCK_CLKSRC  RCC_CFGR_SW_PLL
#endif
#if STM32F10X_CMSIS_CLKSRC == STM32F10X_CMSIS_CLKSRC_PLLCLKBYHSEPLL2
#  define SYSCLOCK_SETUP_HSE
#  define SYSCLOCK_SETUP_PLL
#  define SYSCLOCK_SETUP_PLL2
#  define SYSCLOCK_PLL_INPUT  RCC_CFGR_PLLSRC_PREDIV1
#  define SYSCLOCK_CLKSRC  RCC_CFGR_SW_PLL
#endif

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
#  if STM32F10X_CMSIS_CLOCKHZ <= 24000000
#    define SYSCLOCK_FLASHLATENCY  FLASH_ACR_LATENCY_0
#  elif STM32F10X_CMSIS_CLOCKHZ <= 48000000
#    define SYSCLOCK_FLASHLATENCY  FLASH_ACR_LATENCY_1
#  else
#    define SYSCLOCK_FLASHLATENCY  FLASH_ACR_LATENCY_2
#  endif
#endif


/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/

uint32_t SystemCoreClock = STM32F10X_CMSIS_CLOCKHZ;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

#ifdef DATA_IN_ExtSRAM
  static void SystemInit_ExtMemCtl(void);
#endif /* DATA_IN_ExtSRAM */

/**
  * @}
  */

/** @addtogroup STM32F10x_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the
  *         SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* Reset PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x00FF0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#else
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
  #ifdef DATA_IN_ExtSRAM
    SystemInit_ExtMemCtl();
  #endif /* DATA_IN_ExtSRAM */
#endif

  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied by the PLL factors.
  *
  *         (*) HSI_VALUE is a constant defined in stm32f10x.h file (default value
  *             8 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSE_VALUE is a constant defined in stm32f0xx.h file (default value
  *              8 MHz or 25 MHz, depending on the product used), user has to ensure
  *              that HSE_VALUE is same as the real frequency of the crystal used.
  *              Otherwise, this function may have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
  uint32_t prediv1factor = 0;
#endif /* STM32F10X_LD_VL or STM32F10X_MD_VL or STM32F10X_HD_VL */

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

#ifndef STM32F10X_CL
      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
 #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
       prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
       /* HSE oscillator clock selected as PREDIV1 clock entry */
       SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
 #else
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          SystemCoreClock = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18;

      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13 / 2;
      }

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */

        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & RCC_CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;

        if (prediv1source == 0)
        {
          /* HSE oscillator clock selected as PREDIV1 clock entry */
          SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */

          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & RCC_CFGR2_PLL2MUL) >> 8 ) + 2;
          SystemCoreClock = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;
        }
      }
#endif /* STM32F10X_CL */
      break;

    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }

  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
#ifdef SYSCLOCK_SETUP_HSE
  __IO uint32_t StartUpCounter = 0;
#endif

  /* Configure MCO clock outputs */
  RCC->CFGR |= (STM32F10X_CMSIS_MCOSRC << 24);

  /* At this stage the HSI is enabled and used as System clock source */

#ifdef SYSCLOCK_SETUP_HSE
// use pincfg document setting or backup from core plugin
# if defined(PINCFG_RCC_OSC_HSE_EXTERNAL_CLOCK_SOURCE) || (defined(STM32F10X_CMSIS_HSE_BYPASS) && STM32F10X_CMSIS_HSE_BYPASS)
  /* Bypass HSE oscillator */
  RCC->CR |= RCC_CR_HSEBYP;
# endif

  /* Enable HSE */
  RCC->CR |= RCC_CR_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
  } while (((RCC->CR & RCC_CR_HSERDY) == RESET) && (++StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) == RESET)
  {
      /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
  else
#endif
  {
    /* HCLK */
    RCC->CFGR |= (STM32F10X_CMSIS_AHB << 4);

    /* PCLK2 */
    RCC->CFGR |= (STM32F10X_CMSIS_APB2 << 11);

    /* PCLK1 */
    RCC->CFGR |= (STM32F10X_CMSIS_APB1 << 8);

#ifdef SYSCLOCK_SETUP_PLL2
    /* Configure PLL2 */
    RCC->CFGR2 |= (STM32F10X_CMSIS_PLL2MUL << 8) | ((STM32F10X_CMSIS_PREDIV2 - 1) << 4) | SYSCLOCK_PLL_INPUT;

    /* Enable PLL2 */
    RCC->CR |= RCC_CR_PLL2ON;

    /* Wait till PLL2 is ready */
    while((RCC->CR & RCC_CR_PLL2RDY) == 0)
    {
    }
#endif

#ifdef SYSCLOCK_SETUP_PLL
#ifdef STM32F10X_CMSIS_PLLXTPRE
    RCC->CFGR |= (STM32F10X_CMSIS_PLLXTPRE << 17);
#endif

#ifdef STM32F10X_CMSIS_PREDIV1
    RCC->CFGR2 |= ((STM32F10X_CMSIS_PREDIV1 - 1) << 0);
#endif

    /* Configure the main PLL */
    RCC->CFGR |= SYSCLOCK_PLL_INPUT | (STM32F10X_CMSIS_PLLMUL << 18);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
#endif

#ifdef SYSCLOCK_FLASHLATENCY
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    do
    {
      FLASH->ACR = FLASH_ACR_PRFTBE | SYSCLOCK_FLASHLATENCY;
    	/* Check that the new number of wait states is taken into account to access */
    	/* the Flash memory by reading the FLASH_ACR register */
    }
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != SYSCLOCK_FLASHLATENCY);
#endif

#ifdef SYSCLOCK_CLKSRC
    /* Select the given clock as system clock source */
    RCC->CFGR |= SYSCLOCK_CLKSRC;

    /* Wait till the given clock is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS ) != (SYSCLOCK_CLKSRC << 2));
    {
    }
#endif
  }

#ifdef STM32F10X_CMSIS_USB_DIV
  /* Configure USB clock */
  RCC->CFGR |= (STM32F10X_CMSIS_USB_DIV << 22);
#endif
}


/**
  * @brief  Setup the external memory controller. Called in startup_stm32f10x.s
  *          before jump to __main
  * @param  None
  * @retval None
  */
#ifdef DATA_IN_ExtSRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f10x_xx.s/.c before jump to main.
  * 	      This function configures the external SRAM mounted on STM3210E-EVAL
  *         board (STM32 High density devices). This SRAM will be used as program
  *         data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
/*!< FSMC Bank1 NOR/SRAM3 is used for the STM3210E-EVAL, if another Bank is
  required, then adjust the Register Addresses */

  /* Enable FSMC clock */
  RCC->AHBENR = 0x00000114;

  /* Enable GPIOD, GPIOE, GPIOF and GPIOG clocks */
  RCC->APB2ENR = 0x000001E0;

/* ---------------  SRAM Data lines, NOE and NWE configuration ---------------*/
/*----------------  SRAM Address lines configuration -------------------------*/
/*----------------  NOE and NWE configuration --------------------------------*/
/*----------------  NE3 configuration ----------------------------------------*/
/*----------------  NBL0, NBL1 configuration ---------------------------------*/

  GPIOD->CRL = 0x44BB44BB;
  GPIOD->CRH = 0xBBBBBBBB;

  GPIOE->CRL = 0xB44444BB;
  GPIOE->CRH = 0xBBBBBBBB;

  GPIOF->CRL = 0x44BBBBBB;
  GPIOF->CRH = 0xBBBB4444;

  GPIOG->CRL = 0x44BBBBBB;
  GPIOG->CRH = 0x44444B44;

/*----------------  FSMC Configuration ---------------------------------------*/
/*----------------  Enable FSMC Bank1_SRAM Bank ------------------------------*/

  FSMC_Bank1->BTCR[4] = 0x00001011;
  FSMC_Bank1->BTCR[5] = 0x00000200;
}
#endif /* DATA_IN_ExtSRAM */



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
