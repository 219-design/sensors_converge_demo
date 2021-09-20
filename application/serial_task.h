#ifndef SERIAL_TASK_H_
#define SERIAL_TASK_H_

#include "error_codes.h"

#define QUEUE_SERIAL_STR_LEN UINT8_C(128) // includes termination

typedef struct {
  char str[QUEUE_SERIAL_STR_LEN];
} serial_task_request_t;

void serial_task_main(void *p_handle, void *p_request_queue_handle);

#endif // SERIAL_TASK_H_
