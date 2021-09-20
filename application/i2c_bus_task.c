#include "i2c_bus_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "i2c.h"
#include "main.h"

#include "error_codes.h"

#include <assert.h>

static void *mp_request_queue_handle = NULL;
static void *mp_task_transaction_complete_notify = NULL;

static int process_transaction(void *p_this_task_handle,
                               i2c_bus_task_request_t *p_req,
                               I2C_HandleTypeDef *const p_bus);

void i2c_bus_task_main(void *p_handle, void *p_request_queue_handle) {
  BaseType_t rtos_rc = 0;
  i2c_bus_task_request_t incoming_request = {0};
  int rc = 0;

  const i2c_bus_task_t task = {
      .p_handle = p_handle,
      .p_request_queue_handle = p_request_queue_handle,
      .p_i2c_buses =
          {
              (void *)&hi2c1,
          },
  };

  mp_request_queue_handle = p_request_queue_handle;
  mp_task_transaction_complete_notify = NULL;

  for (;;) {
    rtos_rc = xQueueReceive(task.p_request_queue_handle, &incoming_request,
                            portMAX_DELAY);

    if (pdPASS == rtos_rc) {
      // Only 1 bus supported for now, if need more then use p_req->bus_id
      I2C_HandleTypeDef *const p_bus =
          (I2C_HandleTypeDef *)(task.p_i2c_buses[0]);

      rc = process_transaction(p_handle, &incoming_request, p_bus);

      static_assert(sizeof(rc) <= sizeof(uint32_t));
      xTaskNotify(incoming_request.p_caller_task_handle, (uint32_t)rc,
                  eSetValueWithOverwrite);
    }
  }
}

void *i2c_bus_task_get_queue_handle(void) { return mp_request_queue_handle; }

// Weakly linked in the HAL, and must be overridden here
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *p_i2c) {
  // This is called after the transaction started by HAL_I2C_Mem_Write_IT()
  // which sets the memory pointer with a write first, then a repeated start
  // followed by the read
  BaseType_t higher_priority_task_woken = pdFALSE;

  if (mp_task_transaction_complete_notify != NULL) {
    vTaskNotifyGiveFromISR(mp_task_transaction_complete_notify,
                           &higher_priority_task_woken);
  }

  portYIELD_FROM_ISR(higher_priority_task_woken);
}

// Weakly linked in the HAL, and must be overridden here
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *p_i2c) {
  // This is called after the transaction started by HAL_I2C_Mem_Read_IT() which
  // sets the memory pointer with a write first, then a repeated start followed
  // by the read
  BaseType_t higher_priority_task_woken = pdFALSE;

  if (mp_task_transaction_complete_notify != NULL) {
    vTaskNotifyGiveFromISR(mp_task_transaction_complete_notify,
                           &higher_priority_task_woken);
  }

  portYIELD_FROM_ISR(higher_priority_task_woken);
}

// Weakly linked in the HAL, and must be overridden here
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *p_i2c) {
  BaseType_t higher_priority_task_woken = pdFALSE;

  if (mp_task_transaction_complete_notify != NULL) {
    vTaskNotifyGiveFromISR(mp_task_transaction_complete_notify,
                           &higher_priority_task_woken);
  }

  portYIELD_FROM_ISR(higher_priority_task_woken);
}

static int process_transaction(void *p_this_task_handle,
                               i2c_bus_task_request_t *p_req,
                               I2C_HandleTypeDef *const p_bus) {
  HAL_StatusTypeDef hal_rc = 0;
  int rc = 0;

  mp_task_transaction_complete_notify = p_this_task_handle;
  xTaskNotifyStateClear(p_this_task_handle); // ensure no notifications pending

  const bool is_read = (bool)(p_req->i2c_addr_8bit & UINT8_C(1));
  p_req->i2c_addr_8bit &= UINT8_C(0xfe); // API expects R/W zero

  if (is_read) {
    hal_rc = HAL_I2C_Mem_Read_IT(p_bus, p_req->i2c_addr_8bit, p_req->reg_id,
                                 I2C_MEMADD_SIZE_8BIT, p_req->p_rx_tx_data,
                                 p_req->num_bytes);
  } else {
    hal_rc = HAL_I2C_Mem_Write_IT(p_bus, p_req->i2c_addr_8bit, p_req->reg_id,
                                  I2C_MEMADD_SIZE_8BIT, p_req->p_rx_tx_data,
                                  p_req->num_bytes);
  }

  rc = error_codes_from_hal_rc(hal_rc);

  if (ERROR_CODES_HAL_ERROR == rc) {
    // The I2C driver checks for NAK and returns HAL_ERROR if it finds one, but
    // also returns HAL_ERROR for other conditions as well. For now, just report
    // it as a NAK
    rc = ERROR_CODES_NAK;
  } else if (ERROR_CODES_SUCCESS == rc) {
    // Transaction started, wait for the notification when done

    const uint32_t notification_value = ulTaskNotifyTake(
        pdFALSE, pdMS_TO_TICKS(I2C_BUS_TASK_TRANSACTION_TIMEOUT_MS));

    switch (notification_value) {
    case 1: {
      // Got the ISR notification, now check if the I2C driver reported an error
      const uint32_t hal_error = HAL_I2C_GetError(p_bus);

      // Some of the error codes are ORed, and some are written completely
      if (HAL_I2C_ERROR_NONE == hal_error) {
        rc = ERROR_CODES_SUCCESS;
      } else if (HAL_I2C_ERROR_INVALID_PARAM == hal_error) {
        rc = ERROR_CODES_INVALID_PARAM;
      } else if (hal_error & HAL_I2C_ERROR_TIMEOUT) {
        rc = ERROR_CODES_TIMEOUT;
      } else if (hal_error & HAL_I2C_ERROR_AF) {
        rc = ERROR_CODES_NAK;
      } else {
        // This could be a combination of errors, see bitmask
        rc = ERROR_CODES_DRIVER_ERROR;
      }

      break;
    }

    default:
      // Receiving the notification timed out
      rc = ERROR_CODES_TIMEOUT;
      break;
    }
  }

  // We should clear this in the ISR, however it is reasonable for now to clear
  // it after we get the notification from the ISR
  mp_task_transaction_complete_notify = NULL;
  return rc;
}
