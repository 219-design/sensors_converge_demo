#ifndef I2C_BUS_TASK_H_
#define I2C_BUS_TASK_H_

#include "error_codes.h"

// Increase if ever needed, although unlikely, idea would be to have a limit
// that is tied to some physical reality instead of being a purely generic
// interface
#define I2C_BUS_TASK_TRANSFER_MAX_NUM_BYTES UINT8_C(30)

#define I2C_BUS_TASK_TRANSACTION_TIMEOUT_MS UINT8_C(20)

typedef enum {
  e_i2c_bus_task_id_I2C1 = 0,
  e_i2c_bus_task_id_NUM,
} e_i2c_bus_task_id;

typedef struct {
  void *p_handle;
  void *p_request_queue_handle;
  void *p_i2c_buses[1]; // Only 1 bus supported for now
} i2c_bus_task_t;

typedef struct {
  void *p_caller_task_handle;
  e_i2c_bus_task_id bus_id;
  uint8_t i2c_addr_8bit; // R/W bit indicates operation requested to driver
  uint8_t reg_id;
  uint8_t num_bytes; // num bytes to transfer not including slave addr

  // Pointer to buffer with size >= I2C_BUS_TASK_TRANSFER_MAX_NUM_BYTES
  // This is filled by the driver that runs in an interrupt context, and must
  // remain in scope until the transaction is done
  uint8_t *p_rx_tx_data;
} i2c_bus_task_request_t;

void i2c_bus_task_main(void *p_handle, void *p_request_queue_handle);

void *i2c_bus_task_get_queue_handle(void);

#endif // I2C_BUS_TASK_H_
