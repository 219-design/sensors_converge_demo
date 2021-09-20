/**
  ******************************************************************************
  * @file    demo_application.h
  * @author  MCD Application Team
  * @brief   Header file for demo_application.h file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#ifndef __DEMO_APPLICATION_H_
#define __DEMO_APPLICATION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DEMO
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/*  MMI
     - 15-0 id event                       */
#define DEMO_MMI_ACTION_Pos                   (0U)
#define DEMO_MMI_ACTION_Msk                   (0xFFu << DEMO_MMI_ACTION_Pos) /*!< 0x00FF */
#define DEMO_MMI_ACTION                       DPM_USER_ACTION_Msk
#define DEMO_MMI_ACTION_NONE                  (0u)
#define DEMO_MMI_ACTION_TAMPER_PRESS          (1u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_USER_PRESS            DEMO_MMI_ACTION_TAMPER_PRESS
#define DEMO_MMI_ACTION_RIGHT_PRESS           (2u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_LEFT_PRESS            (3u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_UP_PRESS              (4u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DOWN_PRESS            (5u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_SEL_PRESS             (6u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_DISPLAY_VBUS_IBUS     (7u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_JOY_PRESS             (8u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_ERROR_POWER           (13u << DEMO_MMI_ACTION_Pos)
#define DEMO_MMI_ACTION_ERROR_SETTINGS        (14u << DEMO_MMI_ACTION_Pos)

/* Demo Error types */
#define DEMO_ERROR_TYPE_POWER                 (0u)    /* Power Supply error */
#define DEMO_ERROR_TYPE_FLASH_SETTINGS        (1u)    /* Error in settings stored in flash */
#define DEMO_ERROR_TYPE_MAXNBITEMS            (2u)

/* Exported constants --------------------------------------------------------*/
typedef enum{
     DEMO_OK,
     DEMO_ERROR
} DEMO_ErrorCode;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
DEMO_ErrorCode DEMO_InitBSP(void);
DEMO_ErrorCode DEMO_InitTask(void);
void DEMO_PostCADMessage(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc);
void DEMO_PostNotificationMessage(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal);
void DEMO_PostMMIMessage(uint32_t EventVal);
void DEMO_SetCableInfo(uint8_t PortNum, USBPD_DiscoveryIdentity_TypeDef *pIdentity);
void DEMO_Display_Error(uint8_t PortNum, uint8_t ErrorType);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_APPLICATION_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
