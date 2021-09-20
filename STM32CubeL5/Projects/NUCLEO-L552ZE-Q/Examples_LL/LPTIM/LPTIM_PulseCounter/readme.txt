/**
  @page LPTIM_PulseCounter LPTIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/LPTIM/LPTIM_PulseCounter/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM_PulseCounter example.
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

How to use the LPTIM peripheral in counter mode to generate a PWM output signal 
and update its duty cycle. This example is based on the STM32L5xx
LPTIM LL API. The peripheral is initialized with LL unitary service 
functions to optimize for performance and size.

To reduce power consumption, MCU enters stop mode after starting counting. Each
time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woken up from stop mode and LED1 toggles the last state.

In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM2_IN1 pin, an interrupt is generated and LED1
toggles. If the external function generator is set to provide a square waveform at 1Khz,
the led will toggle each second.

In this example the internal clock provided to the LPTIM2 is LSI (32 kHz),
so the external input is sampled with LSI clock. In order not to miss any event,
the frequency of the changes on the external Input1 signal should never exceed half the
frequency of the internal clock provided to the LPTIM2 (LSI for the
present example).

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Timers, LPTIM, Low power, Pulse Counter, Stop mode, Interrupt

@par Directory contents 

  - LPTIM/LPTIM_PulseCounter/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_PulseCounter/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_PulseCounter/Inc/stm32_assert.h          Template file to include assert_failed function
  - LPTIM/LPTIM_PulseCounter/Src/stm32l5xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PulseCounter/Src/main.c                  Main program
  - LPTIM/LPTIM_PulseCounter/Src/system_stm32l5xx.c      STM32L5xx system source file


@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.
    
  - This example has been tested with NUCLEO-L552ZE-Q board and can be
    easily tailored to any other supported device and development board.
    
  - Connect a square waveform generator to PD.12 (pin 7 in CN7 connector).
    If the frequency of the signal is 1 kHz, LED1 toggles every second.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
