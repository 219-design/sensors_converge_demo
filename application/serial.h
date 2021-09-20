#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

int serial_tx(const uint8_t *p_data, uint16_t num_bytes);

#endif // SERIAL_H_
