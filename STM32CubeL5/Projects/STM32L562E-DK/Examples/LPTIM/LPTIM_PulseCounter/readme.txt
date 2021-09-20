/**
  @page LPTIM_PulseCounter Low power timer pulse counter example
  
  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_PulseCounter/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM Pulse counter example
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

How to configure and use, through the LPTIM HAL API, the LPTIM peripheral 
to count pulses.

To reduce power consumption, MCU enters stop mode after starting counting. Each
time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woke up from stop mode and LED_GREEN toggles the last state.
  
In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM2_IN1 pin, an interrupt is generated and LED_GREEN
toggles.

In this example the internal clock provided to the LPTIM2 is LSI (32 Khz),
so the external input is sampled with LSI clock. In order not to miss any event,
the frequency of the changes on the external Input1 signal should never exceed the
frequency of the internal clock provided to the LPTIM2 (LSI for the
present example).

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Timer, Low Power, Pulse Counter, Stop mode, Interrupt

@par Directory contents  

  - LPTIM/LPTIM_PulseCounter/Inc/stm32l5xx_hal_conf.h         HAL configuration file
  - LPTIM/LPTIM_PulseCounter/Inc/stm32l562e_discovery_conf.h  Board configuration file
  - LPTIM/LPTIM_PulseCounter/Inc/stm32l5xx_it.h               Interrupt handlers header file
  - LPTIM/LPTIM_PulseCounter/Inc/main.h                       Header for main.c module  
  - LPTIM/LPTIM_PulseCounter/Src/stm32l5xx_it.c               Interrupt handlers
  - LPTIM/LPTIM_PulseCounter/Src/main.c                       Main program
  - LPTIM/LPTIM_PulseCounter/Src/stm32l5xx_hal_msp.c          HAL MSP module
  - LPTIM/LPTIM_PulseCounter/Src/system_stm32l5xx.c           STM32L5xx system source file


@par Hardware and Software environment

  - This example runs on STM32L562QEIxQ devices.
    
  - This example has been tested with STMicroelectronics STM32L562E-DK
    board and can be easily tailored to any other supported device
    and development board.      

  - Generate pulses on PD12 (pin 4 in CN12 connector). (Connect a square waveform).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
