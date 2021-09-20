/**
  ******************************************************************************
  * @file    tfm_app.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for tkms application examples.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TFM_APP_H
#define TFM_APP_H

#ifdef __cplusplus
extern "C" {
#endif


#define INVOKE_SCHEDULE_NEEDS()                                                \
      do {                                                                     \
        ;                                                                      \
      } while(0);



/* Exported prototypes ------------------------------------------------------ */
void tfm_app_menu(void);

#ifdef __cplusplus
}
#endif

#endif /* TFM_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
