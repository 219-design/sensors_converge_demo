/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "application.h"
#include "i2c_bus_task.h"
#include "serial_task.h"
#include "sample_logging_task.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for idle */
osThreadId_t idleHandle;
const osThreadAttr_t idle_attributes = {
  .name = "idle",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 4
};
/* Definitions for serial */
osThreadId_t serialHandle;
const osThreadAttr_t serial_attributes = {
  .name = "serial",
  .priority = (osPriority_t) osPriorityLow1,
  .stack_size = 1024 * 4
};
/* Definitions for i2c_bus */
osThreadId_t i2c_busHandle;
const osThreadAttr_t i2c_bus_attributes = {
  .name = "i2c_bus",
  .priority = (osPriority_t) osPriorityLow3,
  .stack_size = 1024 * 4
};
/* Definitions for button */
osThreadId_t buttonHandle;
const osThreadAttr_t button_attributes = {
  .name = "button",
  .priority = (osPriority_t) osPriorityLow3,
  .stack_size = 512 * 4
};
/* Definitions for display */
osThreadId_t displayHandle;
const osThreadAttr_t display_attributes = {
  .name = "display",
  .priority = (osPriority_t) osPriorityLow4,
  .stack_size = 1024 * 4
};
/* Definitions for motion */
osThreadId_t motionHandle;
const osThreadAttr_t motion_attributes = {
  .name = "motion",
  .priority = (osPriority_t) osPriorityLow4,
  .stack_size = 1024 * 4
};
/* Definitions for adc */
osThreadId_t adcHandle;
const osThreadAttr_t adc_attributes = {
  .name = "adc",
  .priority = (osPriority_t) osPriorityLow4,
  .stack_size = 1024 * 4
};
/* Definitions for sample_logging */
osThreadId_t sample_loggingHandle;
const osThreadAttr_t sample_logging_attributes = {
  .name = "sample_logging",
  .priority = (osPriority_t) osPriorityLow2,
  .stack_size = 1024 * 4
};
/* Definitions for queue_serial */
osMessageQueueId_t queue_serialHandle;
const osMessageQueueAttr_t queue_serial_attributes = {
  .name = "queue_serial"
};
/* Definitions for queue_i2c_request */
osMessageQueueId_t queue_i2c_requestHandle;
const osMessageQueueAttr_t queue_i2c_request_attributes = {
  .name = "queue_i2c_request"
};
/* Definitions for queue_sample_logging_request */
osMessageQueueId_t queue_sample_logging_requestHandle;
const osMessageQueueAttr_t queue_sample_logging_request_attributes = {
  .name = "queue_sample_logging_request"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartIdleTask(void *argument);
void StartSerialTask(void *argument);
void StartI2cBusTask(void *argument);
void StartButtonTask(void *argument);
void StartDisplayTask(void *argument);
void StartMotionTask(void *argument);
void StartAdcTask(void *argument);
void StartSampleLoggingTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queue_serial */
  queue_serialHandle = osMessageQueueNew (16, sizeof(serial_task_request_t), &queue_serial_attributes);

  /* creation of queue_i2c_request */
  queue_i2c_requestHandle = osMessageQueueNew (16, sizeof(i2c_bus_task_request_t), &queue_i2c_request_attributes);

  /* creation of queue_sample_logging_request */
  queue_sample_logging_requestHandle = osMessageQueueNew (16, sizeof(sample_logging_task_request_t), &queue_sample_logging_request_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of idle */
  idleHandle = osThreadNew(StartIdleTask, NULL, &idle_attributes);

  /* creation of serial */
  serialHandle = osThreadNew(StartSerialTask, NULL, &serial_attributes);

  /* creation of i2c_bus */
  i2c_busHandle = osThreadNew(StartI2cBusTask, NULL, &i2c_bus_attributes);

  /* creation of button */
  buttonHandle = osThreadNew(StartButtonTask, NULL, &button_attributes);

  /* creation of display */
  displayHandle = osThreadNew(StartDisplayTask, NULL, &display_attributes);

  /* creation of motion */
  motionHandle = osThreadNew(StartMotionTask, NULL, &motion_attributes);

  /* creation of adc */
  adcHandle = osThreadNew(StartAdcTask, NULL, &adc_attributes);

  /* creation of sample_logging */
  sample_loggingHandle = osThreadNew(StartSampleLoggingTask, NULL, &sample_logging_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartIdleTask */
/**
  * @brief  Function implementing the idle thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartIdleTask */
void StartIdleTask(void *argument)
{
  /* USER CODE BEGIN StartIdleTask */

  application_start_task_idle(idleHandle);

  /* USER CODE END StartIdleTask */
}

/* USER CODE BEGIN Header_StartSerialTask */
/**
* @brief Function implementing the serial thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSerialTask */
void StartSerialTask(void *argument)
{
  /* USER CODE BEGIN StartSerialTask */

  application_start_task_serial(serialHandle, queue_serialHandle);

  /* USER CODE END StartSerialTask */
}

/* USER CODE BEGIN Header_StartI2cBusTask */
/**
* @brief Function implementing the i2c_bus thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartI2cBusTask */
void StartI2cBusTask(void *argument)
{
  /* USER CODE BEGIN StartI2cBusTask */

  application_start_task_i2c_bus(i2c_busHandle, queue_i2c_requestHandle);

  /* USER CODE END StartI2cBusTask */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
* @brief Function implementing the button thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartButtonTask */
void StartButtonTask(void *argument)
{
  /* USER CODE BEGIN StartButtonTask */

  application_start_task_button(buttonHandle);

  /* USER CODE END StartButtonTask */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
* @brief Function implementing the display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN StartDisplayTask */

  application_start_task_display(displayHandle);

  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartMotionTask */
/**
* @brief Function implementing the motion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotionTask */
void StartMotionTask(void *argument)
{
  /* USER CODE BEGIN StartMotionTask */

  application_start_task_motion(motionHandle);

  /* USER CODE END StartMotionTask */
}

/* USER CODE BEGIN Header_StartAdcTask */
/**
* @brief Function implementing the adc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdcTask */
void StartAdcTask(void *argument)
{
  /* USER CODE BEGIN StartAdcTask */

  application_start_task_adc(adcHandle);

  /* USER CODE END StartAdcTask */
}

/* USER CODE BEGIN Header_StartSampleLoggingTask */
/**
* @brief Function implementing the sample_logging thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSampleLoggingTask */
void StartSampleLoggingTask(void *argument)
{
  /* USER CODE BEGIN StartSampleLoggingTask */

  application_start_task_sample_logging(sample_loggingHandle, queue_sample_logging_requestHandle);

  /* USER CODE END StartSampleLoggingTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
