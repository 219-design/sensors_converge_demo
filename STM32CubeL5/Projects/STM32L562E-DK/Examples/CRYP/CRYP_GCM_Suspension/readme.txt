/**
  @page CRYP_GCM_Suspension  Suspend a low priority message GCM enciphering
      to carry out a high priority message CCM enciphering

  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_GCM_Suspension/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm suspension/resumption
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

How to use the CRYP peripheral to suspend then resume an authentication ciphering processing.

In this example, a low priority message is enciphered in interrupt mode. Ciphering
algorithm is GCM, key is 128-bit long, there is no data swapping.

The low-priority data processing is suspended to carry out the CCM-enciphering of
a high priority message with a 256-bit long key.

Once the high priority message processing is completed and the enciphered output
and tags are checked against the expected results, the low priority message processing
is resumed.

Artificial scheme is used to trigger the suspension request after eleven blocks
of the low priority data have been processed.
User can use any other interruption to request the suspension.

When all enciphering and tags generations operations are successful, LED_GREEN is turned on.
In case of ciphering, tag generation, initialization, suspension or resumption issue, LED_RED is turned on.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Security, Cryptography, CRYPT, AESGCM

@par Directory contents

  - CRYP/CRYP_GCM_Suspension/Inc/stm32l5xx_hal_conf.h         HAL configuration file
  - CRYP/CRYP_GCM_Suspension/Inc/stm32l562e_discovery_conf.h  Board configuration file
  - CRYP/CRYP_GCM_Suspension/Inc/stm32l5xx_it.h               Interrupt handlers header file
  - CRYP/CRYP_GCM_Suspension/Inc/main.h                       Header for main.c module
  - CRYP/CRYP_GCM_Suspension/Src/stm32l5xx_it.c               Interrupt handlers
  - CRYP/CRYP_GCM_Suspension/Src/main.c                       Main program
  - CRYP/CRYP_GCM_Suspension/Src/stm32l5xx_hal_msp.c          HAL MSP module
  - CRYP/CRYP_GCM_Suspension/Src/system_stm32l5xx.c           STM32L5xx system source file

@par Hardware and Software environment

  - This example runs on STM32L562xx (TZEN = 0) devices.

  - This example has been tested with STMicroelectronics STM32L562E-DK board mounted with an STM32L562QE-Q chip
    and can be easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
