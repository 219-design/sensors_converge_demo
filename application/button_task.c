#include "button_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "logging.h"
#include "pins.h"
#include "sample_logging_task.h"

static int send_event(sample_logging_sample_type_t evt, uint32_t timestamp);

void button_task_main(void *p_handle) {
  TickType_t last_wake_ticks = xTaskGetTickCount();
  uint8_t raw_state = 0;
  bool logical_state_is_pressed = false;

  vTaskDelay(pdMS_TO_TICKS(10)); // let sample logging task start up

  // Start at zero, even though no action has happened yet
  send_event(e_sample_logging_sample_type_BUTTON_RELEASE, 0);
  send_event(e_sample_logging_sample_type_BUTTON_PRESS, 0);

  for (;;) {
    vTaskDelayUntil(&last_wake_ticks,
                    pdMS_TO_TICKS(BUTTON_TASK_SAMPLE_PERIOD_MS));

    raw_state <<= 1;
    raw_state |= (pins_read(PINS_USER_BUTTON) ? 1 : 0);

    if (0 == raw_state) {
      if (logical_state_is_pressed) {
        send_event(e_sample_logging_sample_type_BUTTON_RELEASE,
                   xTaskGetTickCount());
      }
      logical_state_is_pressed = false;

    } else if (UINT8_MAX == raw_state) {

      if (!logical_state_is_pressed) {
        send_event(e_sample_logging_sample_type_BUTTON_PRESS,
                   xTaskGetTickCount());
      }
      logical_state_is_pressed = true;
    }
    // else not debounced
  }
}

static int send_event(sample_logging_sample_type_t evt, uint32_t timestamp) {
  int rc = 0;

  const sample_logging_task_request_t request = {
      .sample_timestamp = timestamp,
      .sample_type = evt,
      .sample = timestamp,
      .sample_width = sizeof(timestamp),
  };

  QueueHandle_t queue_handle = sample_logging_task_get_queue_handle();
  BaseType_t rtos_rc = 0;

  rtos_rc = xQueueSend(queue_handle, &request, pdMS_TO_TICKS(100));

  if (rtos_rc != pdPASS) {
    logging_print("Failed to log button event!\r\n");
    rc = ERROR_CODES_QUEUE_FULL;
  }

  return rc;
}
