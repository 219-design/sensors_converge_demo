#include "serial_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "error_codes.h"
#include "serial.h"

void serial_task_main(void *p_handle, void *p_request_queue_handle) {
  serial_task_request_t request = {0};
  BaseType_t rtos_rc = 0;
  int rc = 0;

  for (;;) {
    rtos_rc = xQueueReceive(p_request_queue_handle, &request, portMAX_DELAY);

    if (pdPASS == rtos_rc) {
      // Assume that the caller zero padded the string, and just print null
      // chars, which in this case since it is ASCII only and the time overhead
      // negligible, it doesn't matter. Add strlen() if it becomes important
      do {
        rc = serial_tx((uint8_t *)request.str, sizeof(request.str));

        switch (rc) {
        case ERROR_CODES_HAL_BUSY:
          vTaskDelay(pdMS_TO_TICKS(10));
          break;
        }

      } while (ERROR_CODES_HAL_BUSY == rc);
    }
  }
}
