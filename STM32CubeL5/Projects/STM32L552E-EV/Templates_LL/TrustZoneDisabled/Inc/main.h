/**
  ******************************************************************************
  * @file    Templates_LL/Legacy/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
/* LL drivers common to all LL examples */
#include "stm32l5xx_ll_bus.h"
#include "stm32l5xx_ll_rcc.h"
#include "stm32l5xx_ll_system.h"
#include "stm32l5xx_ll_utils.h"
#include "stm32l5xx_ll_pwr.h"
#include "stm32l5xx_ll_exti.h"
#include "stm32l5xx_ll_gpio.h"
#include "stm32l5xx_ll_icache.h"
/* LL drivers specific to LL examples IPs */
#include "stm32l5xx_ll_adc.h"
#include "stm32l5xx_ll_comp.h"
#include "stm32l5xx_ll_cortex.h"
#include "stm32l5xx_ll_crc.h"
#include "stm32l5xx_ll_crs.h"
#include "stm32l5xx_ll_dac.h"
#include "stm32l5xx_ll_dma.h"
#include "stm32l5xx_ll_dmamux.h"
#include "stm32l5xx_ll_i2c.h"
#include "stm32l5xx_ll_iwdg.h"
#include "stm32l5xx_ll_lptim.h"
#include "stm32l5xx_ll_lpuart.h"
#include "stm32l5xx_ll_opamp.h"
#include "stm32l5xx_ll_rng.h"
#include "stm32l5xx_ll_rtc.h"
#include "stm32l5xx_ll_spi.h"
#include "stm32l5xx_ll_tim.h"
#include "stm32l5xx_ll_ucpd.h"
#include "stm32l5xx_ll_usart.h"
#include "stm32l5xx_ll_wwdg.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief LED4
  */
#define LED4_PIN                           LL_GPIO_PIN_3
#define LED4_GPIO_PORT                     GPIOB
#define LED4_GPIO_CLK_ENABLE()             LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                   GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_13
#define USER_BUTTON_EXTI_IRQn                   EXTI13_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13)
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13)
#define USER_BUTTON_IRQHANDLER                  EXTI13_IRQHandler
/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
