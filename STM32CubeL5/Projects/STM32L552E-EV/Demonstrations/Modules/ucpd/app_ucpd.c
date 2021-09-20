/**
  ******************************************************************************
  * @file    app_ucpd.c
  * @author  MCD Application Team
  * @brief   UCPD application implementation.
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
#define __APP_UCPD_C

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx.h"
#include "stm32l5xx_ll_bus.h"
#include "stm32l5xx_ll_utils.h"
#include "stm32l5xx_ll_system.h"
#include "stm32l5xx_ll_rcc.h"

#include "stm32l552e_eval.h"
#include "stm32l552e_eval_lcd.h"
#include "stm32l552e_eval_io.h"
#include "app_ucpd.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_window.h"

#include "usbpd_core.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_hw_if.h"
#include "usbpd_pwr_if.h"
#include "demo_application.h"

#include "stm32_lcd.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */

/* Private typedef ----------------------------------------------------------*/
/* Private constants ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _UcpdDemoExec(void);
KMODULE_RETURN _UcpdExit(void);
KMODULE_RETURN _UcpdConfig(void);

/* Private Variable ----------------------------------------------------------*/
__IO UCPD_TypeDef  *__UCDP1;
__IO ADC_TypeDef  *__ADC1;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleUcpd =
{
  MODULE_UCPD,
  _UcpdConfig,
  _UcpdDemoExec,
  NULL,
  NULL
};

/**
  * @brief  setup the HW for the ucpd application
  * @param  None.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _UcpdConfig(void)
{
  UcpdDemoRunning = 1;

  /* ## Backup register access ## */
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  __UCDP1 = UCPD1;
  __ADC1 = ADC1;

  /* Disable SMSP use */
  (void)HAL_PWREx_SMPS_SetMode(PWR_SMPS_BYPASS);

  /* Enable HSI */
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

  /* Initialize the Device Policy Manager */
  if (USBPD_OK != USBPD_DPM_InitCore())
  {
    /* error the RTOS can't be started  */
    while(1);
  }

  /* Initialise the DPM application */
  if (USBPD_OK != USBPD_DPM_UserInit())
  {
    while(1);
  }

#if defined(_GUI_INTERFACE)
  /* Initialize GUI before retrieving PDO from RAM */
  GUI_Init(BSP_GetBoardName, BSP_GetBoardID, HW_IF_PWR_GetVoltage, HW_IF_PWR_GetCurrent);
#endif /* _GUI_INTERFACE */

  /* Initialize the Device Policy Manager */
  if (USBPD_OK != USBPD_DPM_InitOS())
  {
    /* error the RTOS can't be started  */
    while(1);
  }

  return KMODULE_OK;
}

/**
  * @brief  Run the ucpd application
  * @param  None.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _UcpdDemoExec(void)
{
  /* App Execution    */
  USBPD_DPM_Run();

  return KMODULE_OK;
}


/**
  * @brief  exit from the application
  * @param  None.
  * @retval KMODULE_RETURN status.
  */
KMODULE_RETURN _UcpdExit(void)
{
  return KMODULE_OK;
}

/**
  * @brief  BSP Joystick Callback for UCPD demo.
  * @param  JOY    Joystick instance
  * @param  JoyPin Specifies the pin connected EXTI line
  * @retval None.
  */
void UcpdJoyCallback(JOY_TypeDef JOY, uint32_t JoyPin)
{
  if (UcpdDemoRunning == 0)
  {
    return;
  }

  if (JOY != JOY1)
  {
    return;
  }

  if (JoyPin == JOY_LEFT)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_LEFT_PRESS);
  }
  else if (JoyPin == JOY_RIGHT)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_RIGHT_PRESS);
  }
  else if (JoyPin == JOY_DOWN)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_DOWN_PRESS);
  }
  else if (JoyPin == JOY_UP)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_UP_PRESS);
  }
  else if (JoyPin == JOY_SEL)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_SEL_PRESS);
  }
}

/**
  * @brief  BSP Push Button callback for UCPD demo.
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (UcpdDemoRunning == 0)
  {
    return;
  }

  if (BUTTON_TAMPER == Button)
  {
    HAL_NVIC_SystemReset();
  }
  if (BUTTON_WAKEUP == Button)
  {
    DEMO_PostMMIMessage(DEMO_MMI_ACTION_SEL_PRESS);
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
