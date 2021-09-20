#include "stm32l562e_discovery_bus.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "i2c_bus_task.h"

#include <assert.h>

static int32_t transaction(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                           uint16_t Length);

int32_t BSP_GetTick(void) { return (int32_t)HAL_GetTick(); }

int32_t BSP_I2C1_Init(void) {
  // No other init is needed here, the it is done in the HAL
  return BSP_ERROR_NONE;
}

int32_t BSP_I2C1_DeInit(void) { return BSP_ERROR_NONE; }

int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                          uint16_t Length) {
  return transaction(DevAddr & UINT16_C(0xfffe), Reg, pData, Length);
}

int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                         uint16_t Length) {
  return transaction(DevAddr | UINT16_C(0x0001), Reg, pData, Length);
}

static int32_t transaction(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                           uint16_t Length) {
  int32_t rc = BSP_ERROR_NONE;
  BaseType_t rtos_rc = 0;

  void *const p_this_task_handle = xTaskGetCurrentTaskHandle();
  void *const p_queue_handle = i2c_bus_task_get_queue_handle();

  if (NULL == p_queue_handle) {
    return BSP_ERROR_NO_INIT;
  }

  if ((DevAddr > UINT8_MAX) || (Reg > UINT8_MAX) || (Length > UINT8_MAX) ||
      (NULL == pData)) {
    return BSP_ERROR_WRONG_PARAM;
  }

  const i2c_bus_task_request_t request = {
      .p_caller_task_handle = p_this_task_handle,
      .bus_id = e_i2c_bus_task_id_I2C1,
      .i2c_addr_8bit = (uint8_t)DevAddr,
      .reg_id = (uint8_t)Reg,
      .num_bytes = (uint8_t)Length,
      .p_rx_tx_data = pData,
  };

  rtos_rc = xQueueSend(p_queue_handle, &request, pdMS_TO_TICKS(50));

  switch (rtos_rc) {
  case pdPASS:
    rc = BSP_ERROR_NONE;
    break;

  case errQUEUE_FULL:
    rc = BSP_ERROR_BUSY;
    break;

  default:
    rc = BSP_ERROR_UNKNOWN_FAILURE;
    break;
  }

  if (BSP_ERROR_NONE == rc) {
    static_assert(sizeof(rc) <= sizeof(uint32_t));
    int transaction_rc = 0;

    // Wait for the i2C task to complete the transaction
    rtos_rc = xTaskNotifyWait(UINT32_MAX, 0, (uint32_t *)&transaction_rc,
                              pdMS_TO_TICKS(50));

    switch (rtos_rc) {
    case pdTRUE:
      // Received notification from the driver, transaction completed
      switch (transaction_rc) {
      case ERROR_CODES_SUCCESS:
        rc = BSP_ERROR_NONE;
        break;

      case ERROR_CODES_TIMEOUT:
        rc = BSP_ERROR_BUS_FAILURE; // peripheral timeout
        break;

      case ERROR_CODES_NAK:
        rc = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
        break;

      case ERROR_CODES_DRIVER_ERROR:
        rc = BSP_ERROR_PERIPH_FAILURE;
        break;

      default:
        rc = BSP_ERROR_UNKNOWN_FAILURE;
        break;
      }
      break;

    case pdFALSE:
      // No notification from the driver in time
      rc = BSP_ERROR_BUS_FAILURE; // request timeout
      break;

    default:
      rc = BSP_ERROR_UNKNOWN_FAILURE;
      break;
    }
  }

  return rc;
}
