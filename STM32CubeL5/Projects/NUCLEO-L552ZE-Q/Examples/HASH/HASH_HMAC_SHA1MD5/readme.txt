/**
  @page HASH_HMAC_SHA1MD5  HMAC digest calculation using HMAC SHA1 and HMAC MD5 example
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_HMAC_SHA1MD5/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the digest calculation using HMAC SHA1 and HMAC MD5 Example.
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

How to use the HASH peripheral to hash data with HMAC SHA-1 
and HMAC MD5 algorithms.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in
polling mode.

The key used is long type (2088 bits = 261 bytes).
The HMAC SHA-1 message digest result is a 160-bit long (20 bytes) data and the HMAC MD5 message
digest result is a 128-bit long (16 bytes) data.

The expected HASH digests (for SHA1 and MD5) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.

When SHA1 (respectively MD5) digest computation is successful, LED_GREEN (resp. LED_BLUE) is turned on.
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

System, Security, HASH, HMAC, SHA1, MD5, DMA, digest 

@par Directory contents 

  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32l5xx_nucleo_conf.h Board configuration file
  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA1MD5/Inc/main.h                  Header for main.c module  
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32l5xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA1MD5/Src/main.c                  Main program
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32l5xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA1MD5/Src/system_stm32l5xx.c      STM32L5xx system source file


@par Hardware and Software environment

    - This example runs on STM32L552ZETxQ devices.
  
    - This example has been tested with a STM32L552xx embedded on an 
    NUCLEO-L552ZE-Q board and can be easily tailored to any other supported 
    device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 