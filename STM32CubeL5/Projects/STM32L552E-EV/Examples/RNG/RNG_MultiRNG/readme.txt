/**
  @page RNG_MultiRNG Multiple Random Numbers Generator example
  
  @verbatim
  ******************************************************************************
  * @file    RNG/RNG_MultiRNG/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of multiple random numbers generation example.
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

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 110 MHz.

The RNG peripheral configuration is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change RNG configuration.

After startup, user is asked to press WKUP push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers 
at each WKUP push-button press.

 
The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED5 is toggling at a frequency of 1Hz.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

@par Directory contents 

  - RNG/RNG_MultiRNG/Inc/stm32l552e_eval_conf.h     BSP configuration file
  - RNG/RNG_MultiRNG/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - RNG/RNG_MultiRNG/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - RNG/RNG_MultiRNG/Inc/main.h                  Header for main.c module
  - RNG/RNG_MultiRNG/Src/stm32l5xx_it.c          Interrupt handlers
  - RNG/RNG_MultiRNG/Src/main.c                  Main program
  - RNG/RNG_MultiRNG/Src/stm32l5xx_hal_msp.c     HAL MSP module 
  - RNG/RNG_MultiRNG/Src/system_stm32l5xx.c      STM32L5xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.
  
  - This example has been tested with STM32L552E-EV board and can be
    easily tailored to any other supported device and development board.

@note Due to the sharing of some I/Os of STM32L552ZET6QU by multiple peripherals,
      the following limitations apply in using the LED features:
      The green LED I/O cannot be operated simultaneously with JTAG SWO

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 
