/**
  @page CRYP_GCM_GMAC_CCM_Modes  Encrypt data using AES Algo in GCM/GMAC/CCM
  authentication modes with all possible key sizes.

  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_GCM_GMAC_CCM_Modes/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm in GCM/GMAC/CCM modes and
             all key sizes Example
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

How to use the CRYP peripheral to encrypt data and generate authentication tags using GCM/GMAC/CCM
modes.

In this example, the following key sizes are used: 128 or 256.

Ciphering with a 128- or 256-bit long key is used in interrupt mode with data type set to 32-bit (no swapping).
Authentication tag is generated in polling mode.

This example successively carries out

1. GCM mode:
   - AES128 GCM encryption and authentication tag generation
2. GMAC mode:
   - AES256 GCM authentication tag generation (no payload)
3. CCM mode:
   - AES256 GCM encryption and authentication tag generation
   - payload size not a multiple of block to illustrate HAL and peripheral padding capabilities

When all ciphering and tag generation are successful, LED_GREEN is turned on.
In case of ciphering or tag generation issue, LED_RED is turned on.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Security, Cryptography, CRYPT, AESGCM, AESGMAC, AESCCM

@par Directory contents

  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32l5xx_hal_conf.h         HAL configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32l562e_discovery_conf.h  Board configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32l5xx_it.h               Interrupt handlers header file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/main.h                       Header for main.c module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32l5xx_it.c               Interrupt handlers
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/main.c                       Main program
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32l5xx_hal_msp.c          HAL MSP module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/system_stm32l5xx.c           STM32L5xx system source file

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
