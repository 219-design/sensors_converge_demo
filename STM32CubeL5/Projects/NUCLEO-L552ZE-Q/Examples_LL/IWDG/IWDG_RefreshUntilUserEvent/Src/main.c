/**
  ******************************************************************************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure IWDG down-counter (without Window)
  *          using the STM32L5xx IWDG LL API.
  *          Peripheral initialization done using LL unitary services functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L5xx_LL_Examples
  * @{
  */

/** @addtogroup IWDG_RefreshUntilUserEvent
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint8_t ubKeyPressed = 0;
/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Check_IWDG_Reset(void);
void     Configure_IWDG(void);
void     LED_Init(void);
void     LED_On(void);
void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 110 MHz */
  SystemClock_Config();
  
  /* For better performances, enable the instruction cache in 1-way direct mapped mode */
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();

  /* Initialize LED1 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Check if the system has resumed from IWDG reset */
  Check_IWDG_Reset();
  
  /* Configure the IWDG */
  Configure_IWDG();

  /* Infinite loop */
  while (1)
  {
    if (1 != ubKeyPressed)
    {
      /* Refresh IWDG down-counter to default value */
      LL_IWDG_ReloadCounter(IWDG);
      
      LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
      /* Note that period used for Counter Reload MUST be higher than blinking timing value*/
      /* This Counter reload timeout period is a function of this value and the 
         clock prescaler. Refer to the datasheet for the timeout information  */
      LL_mDelay(LED_BLINK_FAST);
    }
  }
}

/**
  * @brief  This function configures IWDG
  * @param  None
  * @retval None
  */
void Configure_IWDG(void)
{
  /* Enable the peripheral clock of DBG register (uncomment for debug purpose) */
  /* ------------------------------------------------------------------------- */
  /*  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_IWDG_STOP); */

  /* Enable the peripheral clock IWDG */
  /* -------------------------------- */
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
  {
  }

  /* Configure the IWDG with window option disabled */
  /* ------------------------------------------------------- */
  /* (1) Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register */
  /* (2) Enable register access by writing 0x0000 5555 in the IWDG_KR register */
  /* (3) Write the IWDG prescaler by programming IWDG_PR from 0 to 7 - LL_IWDG_PRESCALER_4 (0) is lowest divider*/
  /* (4) Write the reload register (IWDG_RLR) */
  /* (5) Wait for the registers to be updated (IWDG_SR = 0x0000 0000) */
  /* (6) Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA) */
  LL_IWDG_Enable(IWDG);                             /* (1) */
  LL_IWDG_EnableWriteAccess(IWDG);                  /* (2) */
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_4);  /* (3) */
  LL_IWDG_SetReloadCounter(IWDG, 0xFEE);            /* (4) */
  while (LL_IWDG_IsReady(IWDG) != 1)                /* (5) */
  {
  }
  LL_IWDG_ReloadCounter(IWDG);                      /* (6) */
}

/**
  * @brief  This function check if the system has resumed from IWDG reset
  * @param  None
  * @retval None
  */
void Check_IWDG_Reset(void)
{
  if (LL_RCC_IsActiveFlag_IWDGRST())
  {
    /* clear IWDG reset flag */
    LL_RCC_ClearResetFlags();

    /* turn Led on and wait for user event to perform example again */
    LED_On();
    
    while(ubKeyPressed != 1)
    {
    }

    /* Reset ubKeyPressed value */
    ubKeyPressed = 0;
  }
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Configures User push-button in EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_DOWN);
  
  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */ 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
}
    
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 110000000
  *            HCLK(Hz)                       = 110000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 55
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 5
  *            Voltage range                  = 0
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable voltage range 0 mode for frequency above 80 Mhz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }

  /* MSI already enabled at reset */

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 55, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Sysclk activation on the main PLL */
  /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Insure 1�s transition state at intermediate medium speed clock based on DWT*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);

  /* AHB prescaler 1 */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 110MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(MSI_VALUE,
                                  LL_RCC_PLLM_DIV_1, 55, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(110000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(110000000);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  ubKeyPressed = 1;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
