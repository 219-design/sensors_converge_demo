#ifndef PINS_H_
#define PINS_H_

#include <stdbool.h>

#include "main.h" // Dev kit pins are defined here
#include "stm32l5xx_hal_gpio.h"

#define PINS_LED_GREEN LED_GREEN_GPIO_Port, LED_GREEN_Pin
#define PINS_LED_RED LED_RED_GPIO_Port, LED_RED_Pin

#define PINS_DEBUG_0 ARD_D8_GPIO_Port, ARD_D8_Pin
#define PINS_DEBUG_1 ARD_D7_GPIO_Port, ARD_D7_Pin

#define PINS_USER_BUTTON USER_BUTTON_GPIO_Port, USER_BUTTON_Pin

#define pins_set(...) HAL_GPIO_WritePin(__VA_ARGS__, GPIO_PIN_SET)
#define pins_clr(...) HAL_GPIO_WritePin(__VA_ARGS__, GPIO_PIN_RESET)
#define pins_toggle(...) HAL_GPIO_TogglePin(__VA_ARGS__)
#define pins_read(...) (bool)(GPIO_PIN_SET == HAL_GPIO_ReadPin(__VA_ARGS__))

#endif // PINS_H_
