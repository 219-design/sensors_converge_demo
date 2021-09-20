/**
  @page LPTIM_PWM_LSE LPTIM PWM Low Power example with LSE clock source
  
  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_PWM_LSE/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM PWM LSE example
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

How to configure and use, through the HAL LPTIM API, the LPTIM peripheral using LSE 
as counter clock, to generate a PWM signal, in a low-power mode.

In this example, the LPTIM instance used is LPTIM1 and the low power mode is Stop 2 mode.

Note that LPTIM2 can't be used in Stop 2 mode.

The counter clock is LSE (32.768 KHz), Autoreload equal to 99 so the output
frequency (FrequencyOutput) will be equal to 327.680.

  FrequencyOutput = Counter Clock Frequency / (Autoreload + 1)
                  = 32768 / 100
                  = 327.680 Hz

Pulse value equal to 49 and the duty cycle (DutyCycle) is computed as follow:

  DutyCycle = 1 - ((PulseValue + 1)/ (Autoreload + 1))
  DutyCycle = 50%

To minimize the power consumption, after starting generating the PWM signal,
the MCU enters in Stop 2 mode. Note that GPIOs are configured in Low Speed to
enhance the consumption. 

WKUP push-button pin (PC.13)is configured as input with external interrupt (External line 13),
falling edge. When WKUP push-button is pressed, wakeup event is generated and PWM signal
generation is stopped.

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

Timer, Low Power, PWM, Stop mode, Interrupt, LSE, Output, Duty Cycle

@par Directory contents

  - LPTIM/LPTIM_PWM_LSE/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32l552e_eval_conf.h  Board configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_PWM_LSE/Inc/main.h                  Header for main.c module  
  - LPTIM/LPTIM_PWM_LSE/Src/stm32l5xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PWM_LSE/Src/main.c                  Main program
  - LPTIM/LPTIM_PWM_LSE/Src/stm32l5xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_PWM_LSE/Src/system_stm32l5xx.c      stm32l5xx system source file


@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.
    
  - This example has been tested with STMicroelectronics STM32L552E-EV
    board and can be easily tailored to any other supported device
    and development board.      

  - Connect PB.02 (pin 3 in CN6 connector) to an oscilloscope to monitor the LPTIM1 output waveform.
  
  - LSE oscillator must be mounted on the board, else this example does not work.

@note Due to the sharing of some I/Os of STM32L552ZET6QU by multiple peripherals,
      the following limitations apply in using the LED features:
      The green LED I/O cannot be operated simultaneously with JTAG SWO

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
