#include "serial.h"

#include "error_codes.h"

#include "main.h"
#include "usart.h"

#define mp_serial &huart1

int serial_tx(const uint8_t *p_data, uint16_t num_bytes) {
  HAL_StatusTypeDef hal_rc = 0;
  int rc = 0;

  hal_rc = HAL_UART_Transmit_IT(mp_serial, (uint8_t *)p_data, num_bytes);
  rc = error_codes_from_hal_rc(hal_rc);

  return rc;
}
