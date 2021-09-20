/**
  @page PWR_LPSLEEP Low Power sleep Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_LPSLEEP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Low Power Sleep Mode example.
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

How to enter the Low-power sleep mode and wake up from this mode by using 
an interrupt.

In the associated software, the system clock is set to 110 MHz.
An EXTI line is connected to the user button through PC.13 and configured 
to generate an interrupt on falling edge upon key press.
 
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED10 is toggled in order to indicate whether the MCU is in LP SLEEP mode 
or RUN mode.

5 seconds after start-up, the system automatically enters LP SLEEP mode and 
LED10 stops toggling.
The User push-button can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering LP SLEEP mode again. 

Two leds LED10 and LED9 are used to monitor the system state as following:
 - LED10 and LED9 ON: configuration failed (system will go to an infinite loop)
 - LED10 toggling: system in RUN mode
 - LED10 off : system in LP SLEEP mode

These steps are repeated in an infinite loop.


      
@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M33 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Power, PWR, Low Power, Sleep mode, Interrupt, Wakeup, External reset

@par Directory contents 

  - PWR/PWR_LPSLEEP/Inc/stm32l562e_discovery_conf.h     BSP configuration file
  - PWR/PWR_LPSLEEP/Inc/stm32l5xx_conf.h         HAL Configuration file
  - PWR/PWR_LPSLEEP/Inc/stm32l5xx_it.h           Header for stm32l5xx_it.c
  - PWR/PWR_LPSLEEP/Inc/main.h                         Header file for main.c
  - PWR/PWR_LPSLEEP/Src/system_stm32l5xx.c       STM32L5xx system clock configuration file
  - PWR/PWR_LPSLEEP/Src/stm32l5xx_it.c           Interrupt handlers
  - PWR/PWR_LPSLEEP/Src/stm32l5xx_hal_msp.c      HAL MSP module
  - PWR/PWR_LPSLEEP/Src/main.c                         Main program

@par Hardware and Software environment

  - This example runs on STM32L5xx devices
      
  - This example has been tested with STMicroelectronics STM32L562E-DK
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32L562E-DK set-up:
    - Use LED10 and LED9 connected respectively to PG.12 and PE.01 pins	
    - Use the User push-button connected to pin PC.13 (External line 13)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
