/**
  @page GPIO_InfiniteLedToggling GPIO example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/GPIO/GPIO_InfiniteLedToggling/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the GPIO example.
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure and use GPIOs to toggle the on-board user LEDs
every 250 ms. This example is based on the STM32L5xx LL API. The peripheral
is initialized with LL unitary service functions to optimize
for performance and size.

PC.07 IO (configured in output pushpull mode) toggles in a forever loop.
On NUCLEO-L552ZE-Q C-02 board this IO is connected to LED1.

In this example, HCLK is configured at 110 MHz.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

System, GPIO, Output, No pull, Speed, Push-pull, Toggle


@par Directory contents

  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_InfiniteLedToggling/Inc/main.h                  Header for main.c module
  - GPIO/GPIO_InfiniteLedToggling/Inc/stm32_assert.h          Template file to include assert_failed function
  - GPIO/GPIO_InfiniteLedToggling/Src/stm32l5xx_it.c          Interrupt handlers
  - GPIO/GPIO_InfiniteLedToggling/Src/main.c                  Main program
  - GPIO/GPIO_InfiniteLedToggling/Src/system_stm32l5xx.c      STM32L5xx system source file


@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.

  - This example has been tested with NUCLEO-L552ZE-Q C-02 board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
