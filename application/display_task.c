#include "display_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "stm32_lcd.h"
#include "stm32l562e_discovery_lcd.h"

#include "logging.h"

#include <assert.h>

static int init_display(void);

int display_task_init(void) {
  init_display();
  return 0;
}

void display_task_main(void *p_handle) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static int init_display(void) {
  LCD_UTILS_Drv_t lcdDrv = {0};
  int rc = 0;
  int32_t bsp_rc = 0;

  bsp_rc = BSP_LCD_Init(0, LCD_ORIENTATION_PORTRAIT);
  rc = error_codes_from_bsp_rc(bsp_rc);
  RETURN_IF_ERROR(rc);

  lcdDrv.DrawBitmap = BSP_LCD_DrawBitmap;
  lcdDrv.FillRGBRect = BSP_LCD_FillRGBRect;
  lcdDrv.DrawHLine = BSP_LCD_DrawHLine;
  lcdDrv.DrawVLine = BSP_LCD_DrawVLine;
  lcdDrv.FillRect = BSP_LCD_FillRect;
  lcdDrv.GetPixel = BSP_LCD_ReadPixel;
  lcdDrv.SetPixel = BSP_LCD_WritePixel;
  lcdDrv.GetXSize = BSP_LCD_GetXSize;
  lcdDrv.GetYSize = BSP_LCD_GetYSize;
  lcdDrv.SetLayer = BSP_LCD_SetActiveLayer;
  lcdDrv.GetFormat = BSP_LCD_GetFormat;
  UTIL_LCD_SetFuncDriver(&lcdDrv);

  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  bsp_rc = BSP_LCD_DisplayOn(0);
  rc = error_codes_from_bsp_rc(bsp_rc);
  return rc;
}
