/**
  @page TFM_Appli

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @brief   This application generates the TFM application secure and non
  *          secure binaries
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application description

The TFM_Appli application consists in two distinct projects: the TFM_Appli
Secure and TFM_Appli Non Secure projects.

The TFM_Appli Secure project is used to generate the TFM_Appli Secure binary
file. The core function of this application relies on the trusted firmware (TFM)
middleware and the mbed-crypto middleware.
It has to be compiled after TFM_SBSFU_Boot project.

The TFM_Appli Non Secure project is used to generate the TFM_Appli Non Secure
binary file. The core function of this application relies on the trusted
firmware (TFM) middleware.
It has to be compiled after TFM_Appli Secure project.

In order to ease the development process, prebuild and postbuild commands are
integrated in each toolchain for both projects.
The prebuild command is in charge of preparing the scatter file according to
common flash layout description in linker folder.
The postbuild command is in charge of signing the binaries, so that they are
trusted by firmware update process.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Security, TFM, mbedCrypto, AES, CRYP, GTZC, HASH, PKA, OSPI-HSPI,
Initial attestation, Secure storage, Internal trusted storage,
Cryptography, PSA, TrustZone

@par Directory contents

  - TFM_Appli/Secure/Inc/aes_alt.h               Header file for aes_alt.c
  - TFM_Appli/Secure/Inc/ccm_alt.h               Header file for ccm_alt.c
  - TFM_Appli/Secure/Inc/cmsis.h                 Header file for CMSIS
  - TFM_Appli/Secure/Inc/ecp_alt.h               Header file for ecp_alt.c
  - TFM_Appli/Secure/Inc/gcm_alt.h               Header file for gcm_alt.c
  - TFM_Appli/Secure/Inc/md5_alt.h               Header file for md5_alt.c
  - TFM_Appli/Secure/Inc/mpu_armv8m_drv.h        Header file for mpu_armv8m_drv.c
  - TFM_Appli/Secure/Inc/platform_irq.h          Platform interrupt requests
  - TFM_Appli/Secure/Inc/rsa_alt.h               Header file for rsa_alt.c
  - TFM_Appli/Secure/Inc/sha1_alt.h              Header file for sha1_alt.c
  - TFM_Appli/Secure/Inc/sha256_alt.h            Header file for sha256_alt.c
  - TFM_Appli/Secure/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - TFM_Appli/Secure/Inc/target_cfg.h            Header file for target_cfg.c
  - TFM_Appli/Secure/Inc/tfm_mbedcrypto_config.h Mbedcrypto configuration file
  - TFM_Appli/Secure/Inc/tfm_peripherals_def.h   Peripheral definitions
 
  - TFM_Appli/Secure/Src/aes_alt.c               AES HW crypto interface
  - TFM_Appli/Secure/Src/attest_hal.c            Initial attestation example
  - TFM_Appli/Secure/Src/ccm_alt.c               CCM HW crypto interface
  - TFM_Appli/Secure/Src/dummy_boot_seed.c       Boot seed implementation
  - TFM_Appli/Secure/Src/dummy_crypto_keys.c     HUK and EAT private key implementation
  - TFM_Appli/Secure/Src/dummy_device_id.c       Device IDs and versions implementation
  - TTM_Appli/Secure/Src/dummy_nv_counters.c     Non Volatile counters implementation
  - TFM_Appli/Secure/Src/ecdsa_alt.c             ECDSA HW crypto interface
  - TFM_Appli/Secure/Src/ecp_alt.c               ECP HW crypto interface
  - TFM_Appli/Secure/Src/ecp_curves_alt.c        ECP curves HW crypto
  - TFM_Appli/Secure/Src/gcm_alt.c               GCM HW crypto interface
  - TFM_Appli/Secure/Src/low_level_com.c         Uart low level interface
  - TFM_Appli/Secure/Src/low_level_flash.c       Flash Low level interface
  - TFM_Appli/Secure/Src/low_level_rng.c         Random generator interface
  - TFM_Appli/Secure/Src/md5_alt.c               MD5 HW crypto interface
  - TFM_Appli/Secure/Src/mpu_armv8m_drv.c        MPU low level interface
  - TFM_Appli/Secure/Src/rsa_alt.c               RSA HW crypto interface
  - TFM_Appli/Secure/Src/sha1_alt.c              SHA1 HW crypto interface
  - TFM_Appli/Secure/Src/sha256_alt.c            SHA256 HW crypto interface
  - TFM_Appli/Secure/Src/spm_hal.c               Secure Partition Manager HAL interface
  - TFM_Appli/Secure/Src/system_stm32l5xx.c      System Init file
  - TFM_Appli/Secure/Src/target_cfg.c            Target start up
  - TFM_Appli/Secure/Src/tfm_platform_system.c   Platform specific implementation
  - TFM_Appli/Secure/Src/tick.c                  HAL Tick implementation

  - TFM_Appli/NonSecure/Inc/cmsis.h              Header file for CMSIS
  - TFM_Appli/NonSecure/Inc/com.h                Header file for com.c
  - TFM_Appli/NonSecure/Inc/common.h             Header file for common.c
  - TFM_Appli/NonSecure/Inc/flash_if.h           Header file for flash_if.c
  - TFM_Appli/NonSecure/Inc/fw_update_app.h      Header file for fw_update.c
  - TFM_Appli/NonSecure/Inc/main.h               Header file for main.c
  - TFM_Appli/NonSecure/Inc/mpu_armv8m_drv.h     Header file for mpu_armv8m_drv.c
  - TFM_Appli/NonSecure/Inc/stm32l5xx_hal_conf.h HAL configuration file
  - TFM_Appli/NonSecure/Inc/stm32l5xx_it.h       Header file for stm32l5xx_it.c
  - TFM_Appli/NonSecure/Inc/test_protections.h   Header file for test_protections.c
  - TFM_Appli/NonSecure/Inc/tfm_app.h            Header file for tfm_app.c
  - TFM_Appli/NonSecure/Inc/ymodem.h             Header file for ymodem.c

  - TFM_Appli/NonSecure/Src/com.c                Uart low level interface
  - TFM_Appli/NonSecure/Src/common.c             Uart common functionalities
  - TFM_Appli/NonSecure/Src/flash_if.c           Flash Low level interface
  - TFM_Appli/NonSecure/Src/fw_update_app.c      Firmware update example
  - TFM_Appli/NonSecure/Src/main.c               Main application file
  - TFM_Appli/NonSecure/Src/mpu_armv8m_drv.c     MPU low level interface
  - TFM_Appli/NonSecure/Src/stm32l5xx_it.c       Interrupt handlers
  - TFM_Appli/NonSecure/Src/system_stm32l5xx.c   System init file
  - TFM_Appli/NonSecure/Src/test_protections.c   Test protections example
  - TFM_Appli/NonSecure/Src/tfm_app.c            Tfm crypto tests example
  - TFM_Appli/NonSecure/Src/tfm_ns_lock.c        NS secure lock for secure access
  - TFM_Appli/NonSecure/Src/ymodem.c             Ymodem support

@par How to use it ?

In order to build the TFM_Appli Secure and Non Secure projects, you must do the
following:
 - Open your preferred toolchain
 - Rebuild the two projects

Then refer to TFM\readme.txt for TFM usage.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
