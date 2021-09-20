#ifndef BUTTON_TASK_H_
#define BUTTON_TASK_H_

#include "error_codes.h"

#define BUTTON_TASK_SAMPLE_PERIOD_MS UINT8_C(10)

void button_task_main(void *p_handle);

#endif // BUTTON_TASK_H_
