#ifndef SAMPLE_LOGGING_TASK_H_
#define SAMPLE_LOGGING_TASK_H_

#include "error_codes.h"

#define SAMPLE_LOGGING_TASK_PRINT_LINE_WIDTH UINT8_C(80)

typedef enum {
  // Internally, the samples are printed in this order after the timestamp
  // NOTE: update publish_sample_headers() internally if changing the order
  e_sample_logging_sample_type_TEMPERATURE = 0,
  e_sample_logging_sample_type_ACCELEROMETER_X,
  e_sample_logging_sample_type_ACCELEROMETER_Y,
  e_sample_logging_sample_type_ACCELEROMETER_Z,
  e_sample_logging_sample_type_BUTTON_PRESS,
  e_sample_logging_sample_type_BUTTON_RELEASE,
  // Add new here
  e_sample_logging_sample_type_NUM,
} sample_logging_sample_type_t;

typedef struct {
  uint32_t sample_timestamp;
  sample_logging_sample_type_t sample_type;
  uint32_t sample;           // Assume all samples can fit in a uint32
  unsigned int sample_width; // Num bytes of sample that are used
} sample_logging_task_request_t;

void sample_logging_task_main(void *p_handle, void *p_request_queue_handle);

void *sample_logging_task_get_queue_handle(void);

#endif // SAMPLE_LOGGING_TASK_H_
