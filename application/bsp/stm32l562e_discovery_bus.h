#ifndef STM32L562E_DISCOVERY_BUS_H
#define STM32L562E_DISCOVERY_BUS_H

// This file wraps access to the I2C bus keeper in the application instead of
// using the BSP's I2C drivers which use a blocking implementation

#include "stm32l562e_discovery_conf.h"
#include "stm32l562e_discovery_errno.h"

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#error Add support to this module, see stm32l562e_discovery_bus in BSP
#endif

#if 0
#if defined(HAL_I2C_MODULE_ENABLED)
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct {
  pI2C_CallbackTypeDef pMspI2cInitCb;
  pI2C_CallbackTypeDef pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
#endif /* HAL_I2C_MODULE_ENABLED */

#define BUS_I2C1 I2C1
#define BUS_I2C1_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_I2C1_CLK_DISABLE() __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_I2C1_FORCE_RESET() __HAL_RCC_I2C1_FORCE_RESET()
#define BUS_I2C1_RELEASE_RESET() __HAL_RCC_I2C1_RELEASE_RESET()
#define BUS_I2C1_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C1_SCL_GPIO_PIN GPIO_PIN_6
#define BUS_I2C1_SCL_GPIO_PORT GPIOB
#define BUS_I2C1_SCL_GPIO_AF GPIO_AF4_I2C1
#define BUS_I2C1_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C1_SDA_GPIO_PIN GPIO_PIN_7
#define BUS_I2C1_SDA_GPIO_PORT GPIOB
#define BUS_I2C1_SDA_GPIO_AF GPIO_AF4_I2C1
#endif

int32_t BSP_GetTick(void);

int32_t BSP_I2C1_Init(void);
int32_t BSP_I2C1_DeInit(void);
int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                          uint16_t Length);
int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                         uint16_t Length);

#if 0 // Not used
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                            uint16_t Length);
int32_t BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData,
                           uint16_t Length);

int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials);
#endif

// SPI is only used on this dev kit for BLE, which isn't used in this
// application yet

#endif /* STM32L562E_DISCOVERY_BUS_H */
