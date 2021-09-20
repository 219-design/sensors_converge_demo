#include "adc_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "logging.h"
#include "sample_logging_task.h"

#include "adc.h"
#include "main.h"

#define ADC_REF_MV UINT16_C(3300)

static void *mp_task_handle = NULL;

static int set_channel_internal_temp_sensor(ADC_HandleTypeDef *p_adc);
static int sample(ADC_HandleTypeDef *p_adc, uint16_t *p_sample_out);
static int convert_sample_to_temperature_dc(ADC_HandleTypeDef *p_adc,
                                            uint16_t sample,
                                            int8_t *p_temp_out);
static int send_event(int8_t temperature, uint32_t timestamp);

void adc_task_main(void *p_handle) {
  int rc = 0;

  TickType_t last_wake_ticks = xTaskGetTickCount();
  TickType_t ticks_now = 0;
  ADC_HandleTypeDef *const p_adc = &hadc1;

  uint16_t adc_sample = 0;
  int8_t temp_dc = 0;
  int8_t last_temp_dc = 0;
  bool first_sample = true;

  mp_task_handle = p_handle;

  set_channel_internal_temp_sensor(p_adc);

  for (;;) {
    vTaskDelayUntil(&last_wake_ticks, pdMS_TO_TICKS(1000));

    ticks_now = xTaskGetTickCount();
    rc = sample(p_adc, &adc_sample);

    if (ERROR_CODES_SUCCESS == rc) {
      rc = convert_sample_to_temperature_dc(p_adc, adc_sample, &temp_dc);

      if (ERROR_CODES_SUCCESS == rc) {
        if (!first_sample) {
          int16_t temp = (last_temp_dc + temp_dc);
          temp /= 2;
          temp_dc = (int8_t)temp;
        }
        first_sample = false;

        send_event(temp_dc, ticks_now);

        last_temp_dc = temp_dc;
      } else {
        logging_print("Failed to calc temperature\r\n");
      }

    } else {
      logging_print("Failed to acquire temperature sample\r\n");
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *p_adc) {
  BaseType_t higher_priority_task_woken = pdFALSE;

  if (mp_task_handle != NULL) {
    vTaskNotifyGiveFromISR(mp_task_handle, &higher_priority_task_woken);
  }

  portYIELD_FROM_ISR(higher_priority_task_woken);
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *p_adc) {
  BaseType_t higher_priority_task_woken = pdFALSE;

  if (mp_task_handle != NULL) {
    vTaskNotifyGiveFromISR(mp_task_handle, &higher_priority_task_woken);
  }

  portYIELD_FROM_ISR(higher_priority_task_woken);
}

static int set_channel_internal_temp_sensor(ADC_HandleTypeDef *p_adc) {
  HAL_StatusTypeDef hal_rc = 0;
  int rc = 0;

  ADC_ChannelConfTypeDef ch_config = {0};

  // To keep it simple, we just overwrite the first channel to be sampled, and
  // the ADC is config'd to only sample once
  ch_config.Channel = ADC_CHANNEL_TEMPSENSOR;
  ch_config.Rank = ADC_REGULAR_RANK_1;
  ch_config.SamplingTime = ADC_SAMPLETIME_247CYCLES_5; // 27 us
  ch_config.SingleDiff = ADC_SINGLE_ENDED;
  ch_config.OffsetNumber = ADC_OFFSET_NONE;
  ch_config.Offset = 0;

  hal_rc = HAL_ADC_ConfigChannel(p_adc, &ch_config);
  rc = error_codes_from_hal_rc(hal_rc);

  return rc;
}

static int sample(ADC_HandleTypeDef *p_adc, uint16_t *p_sample_out) {
  HAL_StatusTypeDef hal_rc = 0;
  int rc = 0;

  xTaskNotifyStateClear(mp_task_handle); // ensure no notifications pending

  hal_rc = HAL_ADC_Start_IT(p_adc);
  rc = error_codes_from_hal_rc(hal_rc);
  RETURN_IF_ERROR(rc);

  // Transaction started, wait for the notification when done

  const uint32_t notification_value =
      ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(25));

  switch (notification_value) {
  case 1: {
    // Got the ISR notification
    const uint32_t hal_error = HAL_ADC_GetError(p_adc);

    // Some of the error codes are ORed, and some are written completely
    if (HAL_ADC_ERROR_NONE == hal_error) {
      const uint32_t sample_temp = HAL_ADC_GetValue(p_adc);

      if (sample_temp <= UINT16_MAX) {
        *p_sample_out = (uint16_t)sample_temp;
        rc = ERROR_CODES_SUCCESS;
      } else {
        rc = ERROR_CODES_INTEGER_OVERFLOW;
      }

    } else {
      // This could be a combination of errors, see bitmask
      rc = ERROR_CODES_DRIVER_ERROR;
    }

    break;
  }

  default:
    // Receiving the notification timed out
    rc = ERROR_CODES_TIMEOUT;
    break;
  }

  return rc;
}

static int convert_sample_to_temperature_dc(ADC_HandleTypeDef *p_adc,
                                            uint16_t sample,
                                            int8_t *p_temp_out) {
  int rc = ERROR_CODES_SUCCESS;

  const int32_t temp = __HAL_ADC_CALC_TEMPERATURE(ADC_REF_MV, (uint32_t)sample,
                                                  ADC_GET_RESOLUTION(p_adc));

  if ((temp <= INT8_MAX) && (temp >= INT8_MIN)) {
    *p_temp_out = (int8_t)temp;
  } else {
    rc = ERROR_CODES_INTEGER_OVERFLOW;
  }

  return rc;
}

static int send_event(int8_t temperature, uint32_t timestamp) {
  int rc = 0;

  const sample_logging_task_request_t request = {
      .sample_timestamp = timestamp,
      .sample_type = e_sample_logging_sample_type_TEMPERATURE,
      .sample = (uint32_t)temperature,
      .sample_width = sizeof(temperature),
  };

  QueueHandle_t queue_handle = sample_logging_task_get_queue_handle();
  BaseType_t rtos_rc = 0;

  rtos_rc = xQueueSend(queue_handle, &request, pdMS_TO_TICKS(100));

  if (rtos_rc != pdPASS) {
    logging_print("Failed to log temperature event!\r\n");
    rc = ERROR_CODES_QUEUE_FULL;
  }

  return rc;
}
