/**
  @page COMP_CompareGpioVsVrefInt_OutputGpio_Init COMP example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_OutputGpio_Init example.
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

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the internal voltage reference (VREFINT). The comparator output is connected
to a GPIO. This example is based on the STM32L5xx COMP LL API.
The peripheral initialization uses LL unitary service functions for optimization
purposes (performance and size).

Example configuration:
Comparator instance COMP1 is configured to compare:
 - input plus set to a GPIO pin (cf pin below) in analog mode.
 - input minus set to internal voltage reference VrefInt (voltage level 1.2V,
   refer to device datasheet for min/typ/max values)
A GPIO pin a configured in alternate function to be connected directly
to comparator output (cf pin below).
Comparator is set in power mode medium speed.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt voltage threshold (comparator input minus),
the comparator output is setting the logical level on GPIO.
LED_GREEN is turned on or off depending on comparator output state (monitoring
of state of GPIO connected to comparator output):
turned on if comparator voltage level on input plus is higher than input minus,
turned-off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator threshold, you can:
 - Either use a wire to connect the GPIO pin of comparator input (cf pin below)
   to other connectors on board:
    - below comparator threshold: to ground
    - above comparator threshold: to voltage of power supply Vdda
      (on board Nucleo: by default, Vdda connected to 3.3V)
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED_GREEN
  1 GPIO for analog input of comparator: PB.02 (connector CN9 pin 13)
  1 GPIO for digital output of comparator: PB.00 (connector CN10 pin 21)
  
@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Comparator, voltage, analog input, digital output, VREFINT, COMP1, Vdda, interrupt

@par Directory contents 

  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/stm32l5xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/system_stm32l5xx.c      STM32L5xx system source file


@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.
    
  - This example has been tested with NUCLEO-L552ZE-Q board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
