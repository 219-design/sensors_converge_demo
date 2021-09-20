#include "motion_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "stm32l562e_discovery_motion_sensors.h"

#include "logging.h"
#include "sample_logging_task.h"

#include <assert.h>

// These defines are for ST's BSP
#define LSM6DS0_0 0
#define ACCELERO_INSTANCE LSM6DS0_0
#define GYRO_INSTANCE LSM6DS0_0

static int init_motion_sensors(void);
static int sample_and_report(void);
static int send_event(sample_logging_sample_type_t evt, int32_t val,
                      uint32_t timestamp);

void motion_task_main(void *p_handle) {
  TickType_t last_wake_ticks = xTaskGetTickCount();

  vTaskDelay(pdMS_TO_TICKS(50)); // let I2C bus come up

  init_motion_sensors();

  for (;;) {
    vTaskDelayUntil(&last_wake_ticks,
                    pdMS_TO_TICKS(MOTION_TASK_SAMPLE_PERIOD_MS));

    sample_and_report();
  }
}

static int init_motion_sensors(void) {
  int rc = 0;
  int32_t bsp_rc = 0;

  // Not used
  float sensitivity = 0.0f;
  float DefaultAccODR = 0.0f;

  bsp_rc =
      BSP_MOTION_SENSOR_Init(ACCELERO_INSTANCE, MOTION_ACCELERO | MOTION_GYRO);
  rc = error_codes_from_bsp_rc(bsp_rc);
  RETURN_IF_ERROR(rc);

  BSP_MOTION_SENSOR_SetFullScale(ACCELERO_INSTANCE, MOTION_ACCELERO, 2.0f);

  BSP_MOTION_SENSOR_GetSensitivity(ACCELERO_INSTANCE, MOTION_ACCELERO,
                                   &sensitivity);

  BSP_MOTION_SENSOR_GetOutputDataRate(ACCELERO_INSTANCE, MOTION_ACCELERO,
                                      &DefaultAccODR);

  bsp_rc = BSP_MOTION_SENSOR_Enable(ACCELERO_INSTANCE, MOTION_ACCELERO);
  rc = error_codes_from_bsp_rc(bsp_rc);
  RETURN_IF_ERROR(rc);

  return rc;
}

static int sample_and_report(void) {
  int rc = 0;
  int32_t bsp_rc = 0;
  BSP_MOTION_SENSOR_Axes_t axes = {0};

  const TickType_t ticks_now = xTaskGetTickCount();

  bsp_rc = BSP_MOTION_SENSOR_GetAxes(ACCELERO_INSTANCE, MOTION_ACCELERO, &axes);
  rc = error_codes_from_bsp_rc(bsp_rc);

  if (ERROR_CODES_SUCCESS == rc) {
    send_event(e_sample_logging_sample_type_ACCELEROMETER_X, axes.x, ticks_now);
    send_event(e_sample_logging_sample_type_ACCELEROMETER_Y, axes.y, ticks_now);
    send_event(e_sample_logging_sample_type_ACCELEROMETER_Z, axes.z, ticks_now);
  } else {
    logging_print("Failed to acquire motion event!\r\n");
  }

  return rc;
}

static int send_event(sample_logging_sample_type_t evt, int32_t val,
                      uint32_t timestamp) {
  int rc = 0;

  const sample_logging_task_request_t request = {
      .sample_timestamp = timestamp,
      .sample_type = evt,
      .sample = (uint32_t)val,
      .sample_width = sizeof(val),
  };

  QueueHandle_t queue_handle = sample_logging_task_get_queue_handle();
  BaseType_t rtos_rc = 0;

  rtos_rc = xQueueSend(queue_handle, &request, pdMS_TO_TICKS(100));

  if (rtos_rc != pdPASS) {
    logging_print("Failed to log motion event!\r\n");
    rc = ERROR_CODES_QUEUE_FULL;
  }

  return rc;
}
