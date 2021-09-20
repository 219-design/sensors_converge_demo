#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "stm32l562e_discovery_errno.h"

#define ERROR_CODES_SUCCESS (0)
#define ERROR_CODES_HAL_ERROR (-1)
#define ERROR_CODES_HAL_BUSY (-2)
#define ERROR_CODES_HAL_TIMEOUT (-3)
#define ERROR_CODES_HAL_UNKNOWN (-4)
#define ERROR_CODES_NULL_POINTER (-5)
#define ERROR_CODES_NO_MEMORY (-6)
#define ERROR_CODES_UNKNOWN (-7)
#define ERROR_CODES_TIMEOUT (-8)
#define ERROR_CODES_DRIVER_ERROR (-9)
#define ERROR_CODES_NAK (-10)
#define ERROR_CODES_INVALID_PARAM (-11)
#define ERROR_CODES_INTEGER_OVERFLOW (-12)
#define ERROR_CODES_QUEUE_FULL (-13)

#define RETURN_IF_NULL(p)                                                      \
  do {                                                                         \
    const void *_p = (p);                                                      \
    if (NULL == _p) {                                                          \
      return ERROR_CODES_NULL_POINTER;                                         \
    }                                                                          \
  } while (0)

#define RETURN_IF_ERROR(rc)                                                    \
  do {                                                                         \
    const int _rc = (rc);                                                      \
    if (_rc != ERROR_CODES_SUCCESS) {                                          \
      return _rc;                                                              \
    }                                                                          \
  } while (0)

#define ERROR_CODES_IS_SUCCESS(rc) (bool)(ERROR_CODES_SUCCESS == (int)(rc))

static inline int error_codes_from_hal_rc(HAL_StatusTypeDef hal_rc) {
  // HAL error codes are translated to the first few negative codes since there
  // are few to handle
  switch (hal_rc) {
  case HAL_OK:
    return ERROR_CODES_SUCCESS;

  case HAL_ERROR:
  case HAL_BUSY:
  case HAL_TIMEOUT:
    return ((int)hal_rc) * -1;

  default:
    // Add to the list above, and handle here
    return ERROR_CODES_HAL_UNKNOWN;
  }
}

static inline int error_codes_from_bsp_rc(int32_t bsp_rc) {
  // BSP error codes are mapped directly from negative to positive since there
  // are more to handle than the HAL
  static_assert(sizeof(int) >= sizeof(bsp_rc));
  static_assert(BSP_ERROR_NONE == ERROR_CODES_SUCCESS);
  return (-1 * bsp_rc);
}

#endif // ERROR_CODES_H_
