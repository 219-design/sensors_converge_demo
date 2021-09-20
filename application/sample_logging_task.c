#include "sample_logging_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "logging.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static void *mp_request_queue_handle = NULL;

static int publish_sample_headers(void);
static int process_sample(const sample_logging_task_request_t *p_sample,
                          sample_logging_task_request_t *p_sample_list_out);
static int
publish_latest_samples(uint32_t time_now, uint32_t lines_printed_so_far,
                       const sample_logging_task_request_t *p_sample_list);

void sample_logging_task_main(void *p_handle, void *p_request_queue_handle) {
  BaseType_t rtos_rc = 0;
  TickType_t last_wake_ticks = xTaskGetTickCount();

  sample_logging_task_request_t incoming_request = {0};
  sample_logging_task_request_t samples[e_sample_logging_sample_type_NUM] = {0};
  uint32_t lines_printed = 0;

  mp_request_queue_handle = p_request_queue_handle;

  for (;;) {
    rtos_rc = xQueueReceive(p_request_queue_handle, &incoming_request, 0);

    if (pdPASS == rtos_rc) {
      process_sample(&incoming_request, samples);
    }

    const TickType_t current_ticks = xTaskGetTickCount();
    if ((current_ticks - last_wake_ticks) >= pdMS_TO_TICKS(1000)) {
      publish_latest_samples(current_ticks, lines_printed, samples);
      lines_printed++;
      last_wake_ticks = current_ticks;
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void *sample_logging_task_get_queue_handle(void) {
  return mp_request_queue_handle;
}

static int process_sample(const sample_logging_task_request_t *p_sample,
                          sample_logging_task_request_t *p_sample_list_out) {
  RETURN_IF_NULL(p_sample);

  int rc = ERROR_CODES_SUCCESS;

  if ((0 == p_sample->sample_width) || (p_sample->sample_width > 4)) {
    rc = ERROR_CODES_INVALID_PARAM;
  } else {
    const unsigned int index = (unsigned int)(p_sample->sample_type);

    if (index < e_sample_logging_sample_type_NUM) {
      memcpy(p_sample_list_out + index, p_sample, sizeof(*p_sample_list_out));
    } else {
      rc = ERROR_CODES_INVALID_PARAM;
    }
  }

  return rc;
}

static int publish_sample_headers(void) {
  // TODO: use a lookup table to generate the header string in correct order
  int rc = logging_print(
      "sys time\ttemp\taccel_x \taccel_y \taccel_z \tbtn press\tbtn rel\r\n");
  return rc;
}

static int
publish_latest_samples(uint32_t time_now, uint32_t lines_printed_so_far,
                       const sample_logging_task_request_t *p_sample_list) {
  int rc = 0;
  char line[SAMPLE_LOGGING_TASK_PRINT_LINE_WIDTH];
  unsigned int i = 0;

  if ((lines_printed_so_far % 16) == 0) {
    publish_sample_headers();
  }

  // If all of the samples + timestamp were uint32, and delimited with tabs, is
  // there enough space to hold a complete line?
  // Technically there is one extra char here since there is no delimiting char
  // after the last sample, but we'll just count that as margin here
  static_assert(((e_sample_logging_sample_type_NUM + 1) * 9) <=
                (SAMPLE_LOGGING_TASK_PRINT_LINE_WIDTH - 3));

  snprintf(line, sizeof(line), "%08" PRIx32, time_now);
  i += 8;

  for (unsigned int j = 0;
       (j < (unsigned int)e_sample_logging_sample_type_NUM) &&
       (i < sizeof(line));
       j++) {
    const char *p_fmt = NULL;
    unsigned int num_chars = 0;

    switch (p_sample_list->sample_width) {
    case 1:
      p_fmt = "\t%02" PRIx16; // PRIx8 not supported
      num_chars = 3;
      break;
    case 2:
      p_fmt = "\t%04" PRIx16;
      num_chars = 5;
      break;
    case 3:
      p_fmt = "\t%06" PRIx32;
      num_chars = 7;
      break;
    case 4:
      p_fmt = "\t%08" PRIx32;
      num_chars = 9;
      break;
    default:
      p_fmt = "\t??";
      num_chars = 3;
      break;
    }

    snprintf(line + i, num_chars + 1, p_fmt, p_sample_list->sample);
    i += num_chars; // snprintf includes null termination in the return value
    p_sample_list++;

    vTaskDelay(pdMS_TO_TICKS(3)); // yield to let lower priority tasks run
  }

  if (i > (sizeof(line) - 3)) {
    i = (sizeof(line) - 3);
  }

  line[i] = '\r';
  line[i + 1] = '\n';
  line[i + 2] = '\0';

  rc = logging_print(line);
  return rc;
}
