#include "application.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "adc_task.h"
#include "button_task.h"
#include "display_task.h"
#include "error_codes.h"
#include "i2c_bus_task.h"
#include "logging.h"
#include "motion_task.h"
#include "pins.h"
#include "sample_logging_task.h"
#include "serial_task.h"
#include "version.h"

#include <string.h>

int application_on_reset(void) {
  // Known bug in ST Cube, the only way to change it is via a global, if not
  // fixed then an assert will be tripped at startup (FreeRTOS has its own timer
  // instead of using Systick)
  // https://community.st.com/s/question/0D50X0000C6duTo/halnvicsetpriority-assert-issue-and-incorrect-preempt-priority-value-for-the-stm32f4-stm32g4-
  extern uint32_t uwTickPrio;
  uwTickPrio = TICK_INT_PRIORITY;
  return 0;
}

int application_init(void) {
  display_task_init();
  return 0;
}

int application_end_of_main(void) { return 0; }

void application_start_task_idle(void *p_handle) {
  TickType_t last_wake_ticks = xTaskGetTickCount();

  for (;;) {
    pins_toggle(PINS_LED_GREEN);
    vTaskDelayUntil(&last_wake_ticks, pdMS_TO_TICKS(500));
  }
}

void application_start_task_serial(void *p_handle,
                                   void *p_request_queue_handle) {

  logging_init(p_request_queue_handle);
  logging_print("\r\n\r\nSensors Converge Demo " VERSION_STR "\r\n");

  serial_task_main(p_handle, p_request_queue_handle);
}

void application_start_task_i2c_bus(void *p_handle,
                                    void *p_request_queue_handle) {

  i2c_bus_task_main(p_handle, p_request_queue_handle);
}

void application_start_task_button(void *p_handle) {
  button_task_main(p_handle);
}

void application_start_task_display(void *p_handle) {
  display_task_main(p_handle);
}

void application_start_task_motion(void *p_handle) {
  motion_task_main(p_handle);
}

void application_start_task_adc(void *p_handle) { adc_task_main(p_handle); }

void application_start_task_sample_logging(void *p_handle,
                                           void *p_request_queue_handle) {
  sample_logging_task_main(p_handle, p_request_queue_handle);
}
