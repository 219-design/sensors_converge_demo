/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/**
  * @brief Variables used for character reception from PC Com port
  */
__IO uint8_t  ubFinalCharReceived = 0;
__IO uint32_t ubReceivedChar;

/**
  * @brief Text string printed on PC Com port to inform MCU will enter in Stop 2 Mode
  */
uint8_t aTextInfo[] = "\r\nLPUART Example : MCU will now enter in Stop 2 mode.\n\rEnter any character for waking up MCU.\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
void LED_Blinking(uint32_t Period);
static void LED_Init(void);
static void LED_Blinking_3s(void);
static void LED_On(void);
static void LED_Off(void);
static void Configure_PWR(void);
static void Configure_LPUART1(void);
static void PrepareLPUARTToStopMode(void);
static void EnterSTOP2Mode(void);
static void PrintInfo(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  LL_PWR_DisableUCPDDeadBattery();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize LED1 */
  LED_Init();

  /* Configure Power IP */
  Configure_PWR();

  /* Configure LPUART1 (LPUART IP configuration and related GPIO initialization) */
  Configure_LPUART1();

  /* Start main program loop :
     - make LED blink during 3 sec
     - Enter Stop 2 mode (LED turned Off)
     - Wait for any character received on LPUART RX line for waking up MCU
  */
  while (ubFinalCharReceived == 0)
  {
    /* LED blinks during 3 seconds */
    LED_Blinking_3s();

    /* Send Text Information on LPUART TX to PC Com port */
    PrintInfo();

    /* Prepare LPUART for entering Stop Mode */
    PrepareLPUARTToStopMode();

    /* Enter Stop 2 mode */
    EnterSTOP2Mode();

    /* At this point, MCU just wakes up from Stop 2 mode */
  }
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(16000000);

  LL_SetSystemCoreClock(16000000);
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
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED Clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /* Configure IO in output push-pull mode to drive external LED */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_VERY_HIGH */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  /* Reset value is LL_GPIO_PULL_UP */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_7, LL_GPIO_PULL_UP);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Set LED1 to Blinking mode during 3s.
  * @param  None
  * @retval None
  */
static void LED_Blinking_3s(void)
{
  uint32_t index=0;

  /* Toggle IO in during 3s (15*200ms) */
  for(index = 0; index < 15; index++)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
    LL_mDelay(200);
  }
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
}

/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
static void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
}

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Ensure that HSI is wake-up system clock */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);
}

/**
  * @brief  This function configures LPUART1.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the LPUART1 pins.
  *         -2- NVIC Configuration for LPUART1 interrupts.
  *         -3- Enable the LPUART1 peripheral clock and clock source.
  *         -4- Configure LPUART1 functional parameters.
  *         -5- Enable LPUART1.
  * @param  None
  * @retval None
  */
void Configure_LPUART1(void)
{
  LL_LPUART_InitTypeDef lpuart_initstruct;

  /* (1) Enable GPIO clock and configures the LPUART1 pins *******************/
  /*    (TX on PG.7, RX on PG.8)                        **********************/

  /* Enable the peripheral clock of GPIOG */
  LL_PWR_EnableVddIO2();
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);

  /* Configure TX Pin as : Alternate function, High Speed, PushPull, Pull up */
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOG, LL_GPIO_PIN_7, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOG, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOG, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOG, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);

  /* Configure RX Pin as : Alternate function, High Speed, PushPull, Pull up */
  LL_GPIO_SetPinMode(GPIOG, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOG, LL_GPIO_PIN_8, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOG, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOG, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOG, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);

  /* (2) NVIC Configuration for LPUART1 interrupts */
  /*  - Set priority for LPUART1_IRQn */
  /*  - Enable LPUART1_IRQn           */
  NVIC_SetPriority(LPUART1_IRQn, 0);
  NVIC_EnableIRQ(LPUART1_IRQn);

  /* (3) Enable the LPUART1 peripheral clock and clock source ****************/
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

  /* Set LPUART1 clock source as HSI */
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSI);

  /* (4) Configure LPUART1 functional parameters ********************************/

  /* Disable LPUART1 prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_LPUART_Disable(LPUART1);

  /* Set fields of initialization structure                   */
  /*  - Prescaler           : LL_LPUART_PRESCALER_DIV1        */
  /*  - BaudRate            : 9600                            */
  /*  - DataWidth           : LL_LPUART_DATAWIDTH_8B          */
  /*  - StopBits            : LL_LPUART_STOPBITS_1            */
  /*  - Parity              : LL_LPUART_PARITY_NONE           */
  /*  - TransferDirection   : LL_LPUART_DIRECTION_TX_RX       */
  /*  - HardwareFlowControl : LL_LPUART_HWCONTROL_NONE        */
  lpuart_initstruct.PrescalerValue      = LL_LPUART_PRESCALER_DIV1;
  lpuart_initstruct.BaudRate            = 9600;
  lpuart_initstruct.DataWidth           = LL_LPUART_DATAWIDTH_8B;
  lpuart_initstruct.StopBits            = LL_LPUART_STOPBITS_1;
  lpuart_initstruct.Parity              = LL_LPUART_PARITY_NONE;
  lpuart_initstruct.TransferDirection   = LL_LPUART_DIRECTION_TX_RX;
  lpuart_initstruct.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;

  /* Initialize LPUART instance according to parameters defined in initialization structure */
  LL_LPUART_Init(LPUART1, &lpuart_initstruct);

  /* Set the wake-up event type : specify wake-up on RXNE flag */
  LL_LPUART_SetWKUPType(LPUART1, LL_LPUART_WAKEUP_ON_RXNE);

  /* (5) Enable LPUART1 **********************************************************/
  LL_LPUART_Enable(LPUART1);
}

/**
  * @brief  Function to configure LPUART for being ready to enter "Stop 2" mode.
  * @param  None
  * @retval None
  */
void PrepareLPUARTToStopMode(void)
{

  /* Empty RX Fifo before entering Stop mode (Otherwise, characters already present in FIFO
     will lead to immediate wake up */
  while (LL_LPUART_IsActiveFlag_RXNE(LPUART1))
  {
    /* Read Received character. RXNE flag is cleared by reading of RDR register */
    ubReceivedChar = LL_LPUART_ReceiveData8(LPUART1);
  }

  /* Clear OVERRUN flag */
  LL_LPUART_ClearFlag_ORE(LPUART1);

  /* Make sure that no LPUART transfer is on-going */
  while(LL_LPUART_IsActiveFlag_BUSY(LPUART1) == 1)
  {
  }
  /* Make sure that LPUART is ready to receive */
  while(LL_LPUART_IsActiveFlag_REACK(LPUART1) == 0)
  {
  }

  /* About to enter stop mode: switch off LED */
  LED_Off();

  /* Configure LPUART1 transfer interrupts : */
  /* Clear WUF flag and enable the UART Wake Up from stop mode Interrupt */
  LL_LPUART_ClearFlag_WKUP(LPUART1);
  LL_LPUART_EnableIT_WKUP(LPUART1);

  /* Enable Wake Up From Stop */
  LL_LPUART_EnableInStopMode(LPUART1);
}

/**
  * @brief  Function to enter in "Stop 2" mode.
  * @param  None
  * @retval None
  */
void EnterSTOP2Mode(void)
{
  /** Request to enter "Stop 2" mode
    * Following procedure describe in STM32L5xx Reference Manual
    * See PWR part, section Low-power modes, "Stop 2" mode
    */
  /* Set Stop 2 mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();

  /* Request Wait For Interrupt */
  __WFI();
}

/**
  * @brief  Send Txt information message on LPUART Tx line (to PC Com port).
  * @param  None
  * @retval None
  */
void PrintInfo(void)
{
  uint32_t index = 0;

  /* Send characters one per one, until last char to be sent */
  for (index = 0; index < sizeof(aTextInfo); index++)
  {
    /* Wait for TXE flag to be raised */
    while (!LL_LPUART_IsActiveFlag_TXE(LPUART1))
    {
    }

    /* Write character in Transmit Data register.
       TXE flag is cleared by writing data in TDR register */
    LL_LPUART_TransmitData8(LPUART1, aTextInfo[index]);
  }

  /* Wait for TC flag to be raised for last char */
  while (!LL_LPUART_IsActiveFlag_TC(LPUART1))
  {
  }
}

/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/

/**
  * @brief  Function called from LPUART IRQ Handler when RXNE flag is set
  *         Function is in charge of reading character received on LPUART RX line.
  * @param  None
  * @retval None
  */
void LPUART_CharReception_Callback(void)
{
  /* Read Received character. RXNE flag is cleared by reading of RDR register */
  ubReceivedChar = LL_LPUART_ReceiveData8(LPUART1);

  /* Check if received value is corresponding to specific one : S or s */
  if ((ubReceivedChar == 'S') || (ubReceivedChar == 's'))
  {
    /* Turn LED1 On : Expected character has been received */
    LED_On();

    /* End of program : set boolean for main loop exit */
    ubFinalCharReceived = 1;
  }

  /* Echo received character on TX */
  LL_LPUART_TransmitData8(LPUART1, ubReceivedChar);
}

/**
  * @brief  Function called in case of error detected in LPUART IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
  /* Disable LPUART1_IRQn */
  NVIC_DisableIRQ(LPUART1_IRQn);

  /* Unexpected event : Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Infinite loop */
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
