/**
  ******************************************************************************
  * @file    lcd.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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

/** @addtogroup STM32L5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM        3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature;

/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo(void)
{
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature);

  while (1)
  {
    if (UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;

      if (++LCD_Feature < LCD_FEATURES_NUM)
      {
        LCD_Show_Feature(LCD_Feature);
      }
      else
      {
        return;
      }
    }
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, 240, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, use User push-button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"to display next page", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, 220, 140, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, 218, 138, UTIL_LCD_COLOR_BLUE);
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[] = {{20, 150}, {50, 150}, {50, 200}};
  Point Points2[] = {{60, 150}, {90, 150}, {90, 200}};

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(12, 92, 216, 136, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);

  switch (feature)
  {
    case 0:
      /* Text Feature */

      UTIL_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
      UTIL_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(14, 130, (uint8_t *)"Right aligned Text", RIGHT_MODE);
      UTIL_LCD_SetFont(&Font24);
      UTIL_LCD_DisplayStringAt(14, 150, (uint8_t *)"Font24", LEFT_MODE);
      UTIL_LCD_SetFont(&Font20);
      UTIL_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font20", LEFT_MODE);
      UTIL_LCD_SetFont(&Font16);
      UTIL_LCD_DisplayStringAt(14, 210, (uint8_t *)"Font16", LEFT_MODE);
      break;

    case 1:

      /* Draw misc. Shapes */
      UTIL_LCD_DrawRect(20, 100, 30 , 40, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_FillRect(60, 100, 30 , 40, UTIL_LCD_COLOR_BLACK);

      UTIL_LCD_DrawCircle(130, 120, 20, UTIL_LCD_COLOR_GRAY);
      UTIL_LCD_FillCircle(195, 120, 20, UTIL_LCD_COLOR_GRAY);

      UTIL_LCD_DrawPolygon(Points, 3, UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_FillPolygon(Points2, 3, UTIL_LCD_COLOR_GREEN);

      UTIL_LCD_DrawEllipse(130, 170, 30, 20, UTIL_LCD_COLOR_RED);
      UTIL_LCD_FillEllipse(195, 170, 30, 20, UTIL_LCD_COLOR_RED);

      UTIL_LCD_DrawHLine(20, 210, 30, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawLine (100, 220, 220, 190, UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawLine (100, 190, 220, 220, UTIL_LCD_COLOR_BLACK);
      break;

    case 2:
      /* Draw Bitmap */
      UTIL_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(130, 100, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(20, 160, (uint8_t *)stlogo);
      HAL_Delay(100);

      UTIL_LCD_DrawBitmap(130, 160, (uint8_t *)stlogo);
      HAL_Delay(100);

      break;
    default :
      break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

