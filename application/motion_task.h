#ifndef MOTION_TASK_H_
#define MOTION_TASK_H_

#include "error_codes.h"

#define MOTION_TASK_SAMPLE_PERIOD_MS UINT8_C(200)

void motion_task_main(void *p_handle);

#endif // MOTION_TASK_H_
