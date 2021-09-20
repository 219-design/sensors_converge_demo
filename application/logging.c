#include "logging.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "serial_task.h"

#include <string.h>

static void *mp_target_task_handle = NULL;

int logging_init(void *p_target_task_handle) {
  RETURN_IF_NULL(p_target_task_handle);
  mp_target_task_handle = p_target_task_handle;
  return ERROR_CODES_SUCCESS;
}

int logging_print(const char *p_str) {
  RETURN_IF_NULL(mp_target_task_handle); // not initialized
  RETURN_IF_NULL(p_str);

  BaseType_t rtos_rc = 0;
  int rc = 0;

  // Copying allows for the caller's string to leave scope, if it adds too much
  // time then remove
  serial_task_request_t request = {0};
  strncpy(request.str, p_str, sizeof(request.str));

  rtos_rc = xQueueSend(mp_target_task_handle, &request, 0);

  switch (rtos_rc) {
  case pdPASS:
    rc = ERROR_CODES_SUCCESS;
    break;

  case errQUEUE_FULL:
    rc = ERROR_CODES_NO_MEMORY;
    break;

  default:
    rc = ERROR_CODES_UNKNOWN;
    break;
  }

  return rc;
}
