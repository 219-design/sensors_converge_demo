#ifndef LOGGING_H_
#define LOGGING_H_

#include "error_codes.h"

int logging_init(void *p_target_task_handle);

int logging_print(const char *p_str);

#endif // LOGGING_H_
