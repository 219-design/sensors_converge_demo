/**
  ******************************************************************************
  * @file    Secure/Src/secure_nsc.c
  * @author  MCD Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
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
/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"
#include <stdio.h>

/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

/**
  * @brief  Secure call to printf (COM port is assigned to secure).
  * @param  format string to display
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_DisplayMessage(const uint8_t *format)
{
  printf("%s", (const char *)format);
}

/**
  * @brief  Secure call to toggle LED_RED (assigned to secure)
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_ToggleRedLed(void)
{
  HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
}

/**
  * @brief  Configure SAU and MPCWM2 in initial conditions
  * @note   SAU and MPCWM2 are setup so that only first half FMC NOR memory
  *         is accessible from non-secure
  *         Also used to restore initial configuration with
  *         FMC SRAM memory split into 2 halves:
  *         0x60000000-0x600FFFFF : non-secure
  *         0x60100000-0x601FFFFF : secure
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_SAU_MPCWM2_SetInitConfig(void)
{
  MPCWM_ConfigTypeDef MPCWM_Desc;

  /* protect this SAU/MPCWM setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* set first half of memory as non-secure, second half as secure */

  /* SAU update based on partition_stm32l552xx.h used at setup      */
  /* Reduced non-secure area (SAU region 4) : 0x60000000-0x600FFFFF */
  /* memory from 0x60100000 are then secure */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BANK1 & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BANK1 + 0x000FFFFF) & SAU_RLAR_LADDR_Msk)
               | ((0 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)
               | 1U);

  /* MPCWM2: set one non-secure area on first half */
  /* The second half is set as secure by default */
  MPCWM_Desc.AreaId = GTZC_TZSC_MPCWM_ID1;
  MPCWM_Desc.Offset = 0;
  MPCWM_Desc.Length = 8 * GTZC_TZSC_MPCWM_GRANULARITY;
  if (HAL_GTZC_TZSC_MPCWM_ConfigMemAttributes(FMC_BANK1, &MPCWM_Desc) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* re-enable fault interrupt occurrences after this SAU/MPCWM setup section */
  __enable_fault_irq();
}

/**
  * @brief  Configure SAU and MPCWM2 in WakeUp Button Error Case
  * @note   SAU and MPCWM2 are setup so that whole FMC NOR memory is
  *         accessible from non-secure
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_SAU_MPCWM2_SetWakeupButtonErrorCaseConfig(void)
{
  MPCWM_ConfigTypeDef MPCWM_Desc;

  /* protect this SAU/MPCWM setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* Here, with initial configuration, a non-secure access has already been done
   * in the secure area and the Secure Fault interrupt occurred. This function
   * is called from the handler to allow the SW to exit the handler (jump to the
   * address which generated the IRQ) with conditions which will allow the
   * execution of the operation (unless this is an infinite loop).
   * So the process here is to set the whole FMC NOR memory as non-secure,
   * on both SAU and MPCWM2 sides.
   */

  /* Whole non-secure area (SAU region 4) : 0x60000000-0x601FFFFF */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BANK1 & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BANK1 + 0x001FFFFF) & SAU_RLAR_LADDR_Msk)
               | ((0 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)
               | 1U);

  /* MPCWM2: set one non-secure area on whole memory */
  MPCWM_Desc.AreaId = GTZC_TZSC_MPCWM_ID1;
  MPCWM_Desc.Offset = 0;
  MPCWM_Desc.Length = 16 * GTZC_TZSC_MPCWM_GRANULARITY;
  if (HAL_GTZC_TZSC_MPCWM_ConfigMemAttributes(FMC_BANK1, &MPCWM_Desc) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* re-enable fault interrupt occurrences after this SAU/MPCWM setup section */
  __enable_fault_irq();
}

/**
  * @brief  Configure SAU and MPCWM2 in Tamper Button Error Case
  * @note   SAU is setup so that whole FMC NOR memory is declared as non-secure
  *         GTZC MPCWM2 is kept as the initial configuration with only the first
  *         half NOR memory accessible from non-secure
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_SAU_MPCWM2_SetTamperButtonErrorCaseConfig(void)
{
  /* protect this SAU/MPCWM setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* Here the objective is to provoke a GTZC_IRQn occurrence.
   * We will do a non-secure access in the secure area, but before that, we need
   * to setup SAU, to have a whole non-secure area from its point of view.
   * By this way, SAU and MPCWM2 configuration will be misaligned, and the
   * access to the secure area will be allowed by SAU, but not by MPCWM2.
   * This will generate a GTZC_IRQn interrupt, which is our goal.
   */

  /* Whole non-secure area (SAU region 4) : 0x60000000-0x601FFFFF */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BANK1 & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BANK1 + 0x00200000) & SAU_RLAR_LADDR_Msk)
               | ((0 << SAU_RLAR_NSC_Pos)  & SAU_RLAR_NSC_Msk)
               | 1U);

  /* MPCWM2: no change */

  /* re-enable fault interrupt occurrences after this SAU/MPCWM setup section */
  __enable_fault_irq();
}

/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
