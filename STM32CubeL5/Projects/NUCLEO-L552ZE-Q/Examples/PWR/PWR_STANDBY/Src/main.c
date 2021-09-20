/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L5xx PWR HAL API to enter
  *          and exit the standby mode with a wakeup pin or external reset.
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_TOGGLE_DELAY         100UL
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static uint32_t TimingDelay = LED_TOGGLE_DELAY;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static void LowPower_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LowPower_Config(void)
{
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Needed as level shifter present on the Nucleo board  */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, PWR_GPIO_BIT_13);   /* TMS/SWDIO */
  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B, PWR_GPIO_BIT_3);    /* SWO       */ 
  HAL_PWREx_EnablePullUpPullDownConfig();
  
  /* Enable ultra low power mode. */ 
  HAL_PWREx_EnableUltraLowPowerMode();

  /* Disable the Power Voltage Detector (PVD) */
  HAL_PWR_DisablePVD();

  /* Disable dead battery behavior */
  HAL_PWREx_DisableUCPDDeadBattery();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* STM32L5xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Configure LED1 */
  BSP_LED_Init(LED1);
  /* Configure LED3 for error handler */
  BSP_LED_Init(LED3);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  /* Check if the system was resumed from Standby mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    /* Check and Clear the Wakeup flag */
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) != RESET)
    {
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    }

    /* Wait that user release the User push-button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    while (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET) {}
  }

  /* Resume Tick increment. */
  HAL_ResumeTick(); 

  /* Insert 5 seconds delay */
  HAL_Delay(5000);
  /* The Following Wakeup sequence is highly recommended prior to Standby mode entry
     - Enable wakeup
     - Clear wake up pin flag depending in edge detection & pin level.
     - Enter the Standby mode.
  */

  /* ----------------------------------------------------------------------  */

  /* Suspend Tick increment. */
  HAL_SuspendTick();
  
  
  /* Set low power configuration for Nucleo & STM32L5xx */
  LowPower_Config();
  
  /* ----------------------------------------------------------------------  */
    
  /* Enable WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);

  /* Clear all related wakeup flags*/
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);

  /* Enter the Standby mode */
  HAL_PWR_EnterSTANDBYMode();

  /* This code will never be reached! */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */
  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */


/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
  else
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* Turn LED3 on */
  BSP_LED_On(LED3);
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
