/**
  ******************************************************************************
  * @file    k_widgets.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel widgets functions
  ******************************************************************************
   * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "stm32l552e_eval_lcd.h"
#include "stm32_lcd.h"
#include "k_widgets.h"

/* External variables --------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  kWidgetsProgressBar_t progressBar;
  uint32_t             progress;
  uint32_t             maxValue;
  uint32_t             step;
}ProgressBar_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*                     P R O G R E S S   B A R                                */
/******************************************************************************/
/**
  * @brief  Create a progress bar
  * @param  pProgressBar Progress Bar descriptor
  * @param  MaxValue Maximum representable value
  * @retval Progress bar handle
  */
void * kWidgets_ProgressBarCreate(kWidgetsProgressBar_t * pProgressBar, uint32_t MaxValue)
{
  ProgressBar_t * pb = (ProgressBar_t *)NULL;

  pb = (ProgressBar_t *) malloc(sizeof(ProgressBar_t));

  if (pb != NULL)
  {
    memcpy(&(pb->progressBar), pProgressBar, sizeof(kWidgetsProgressBar_t));
    pb->progress = 0;
    pb->maxValue = MaxValue;
    pb->step =  MaxValue / pb->progressBar.width;
    UTIL_LCD_SetTextColor(pProgressBar->backgroungColor);
    UTIL_LCD_FillRect(pProgressBar->xpos+1, pProgressBar->ypos+1, pProgressBar->width-2, pProgressBar->height-2, pProgressBar->backgroungColor);
    UTIL_LCD_SetTextColor(pProgressBar->textColor);
    UTIL_LCD_DrawRect(pProgressBar->xpos, pProgressBar->ypos, pProgressBar->width, pProgressBar->height, pProgressBar->textColor);
  }

  return (void *)pb;
}

/**
  * @brief  Destroy a progress bar
  * @param  hwidgetProgressBar Progress bar handle
  * @retval None
  */
void kWidgets_ProgressBarDestroy(void * hwidgetProgressBar)
{
  free(hwidgetProgressBar);
}

/**
  * @brief  Update a progress bar
  * @param  hwidgetProgressBar Progress bar handle
  * @param  Progress progress bar update
  * @retval None
  */
void kWidgets_ProgressBarUpdate(void * hwidgetProgressBar, uint32_t Progress)
{
  ProgressBar_t * pb = (ProgressBar_t *) hwidgetProgressBar;
  uint32_t width;
  uint32_t step = pb->step;

  width = Progress - pb->progress;

  if (width >= step )
  {
    UTIL_LCD_SetTextColor(pb->progressBar.textColor);
    UTIL_LCD_FillRect(pb->progressBar.xpos + (uint32_t)(pb->progress/step),
                     pb->progressBar.ypos,
                     width/step,
                 pb->progressBar.height,
                 pb->progressBar.textColor);
    pb->progress += step;
  }
}

/**
  * @brief  Reset a progress bar
  * @param  hwidgetProgressBar Progress bar handle
  * @retval None
  */
void kWidgets_ProgressBarReset(void * hwidgetProgressBar)
{
  ProgressBar_t * pb = (ProgressBar_t *) hwidgetProgressBar;

  pb->progress = 0;
  UTIL_LCD_SetBackColor(pb->progressBar.backgroungColor);
  UTIL_LCD_SetTextColor(pb->progressBar.backgroungColor);
  UTIL_LCD_FillRect(pb->progressBar.xpos + 1,
               pb->progressBar.ypos + 1,
               pb->progressBar.width - 2,
               pb->progressBar.height - 2,
               pb->progressBar.backgroungColor);
  UTIL_LCD_SetTextColor(pb->progressBar.textColor);
  UTIL_LCD_DrawRect(pb->progressBar.xpos,
                   pb->progressBar.ypos,
                   pb->progressBar.width,
               pb->progressBar.height,
               pb->progressBar.textColor);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
