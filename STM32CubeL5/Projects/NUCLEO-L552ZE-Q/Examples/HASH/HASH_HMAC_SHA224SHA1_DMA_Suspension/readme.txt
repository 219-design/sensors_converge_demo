/**
  @page HASH_HMAC_SHA224SHA1_DMA_Suspension HMAC digest calculation suspension example
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HMAC digest calculation suspension using SHA-1 and SHA-224 example.
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

How to suspend the HMAC digest computation when data are fed 
to the HASH unit with DMA.

The example computes the multi-buffer HMAC SHA-224 digest of a low priority 
block in entering input data in DMA mode.

The processing is suspended in order to process a higher priority block to
compute its HMAC SHA1 digest. For the higher priority block, data are entered in
DMA mode as well.

It is shown how to save then how to restore later on the low priority block
context to resume its processing.
For both the low and high priority blocks, the computed digests are compared with
the expected ones.

For illustration purposes,
- the processing of each buffer of the low priority block is suspended
- suspension request is arbitrarily initiated by a flag set by software
(it could be triggered by an interruption in the application code).

LED_GREEN is turned on when correct digest values are calculated.
In case of digest computation or initialization issue, LED_RED is turned on. 

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

System, Security, HASH, HMAC, SHA1, SHA224, DMA, digest 

@par Directory contents 

  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32l5xx_nucleo_conf.h Board configuration file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Inc/main.h			 Header for main.c module  
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/stm32l5xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/main.c	                 Main program
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/stm32l5xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA224SHA1_DMA_Suspension/Src/system_stm32l5xx.c      STM32L5xx system source file

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
 