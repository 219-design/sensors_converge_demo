/**
  @page HeartRate  BLE HeartRate application

  @verbatim
  ******************************************************************************
  * @file    BLE/HeartRate/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BLE HeartRate application
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

@par Application Description

This application shows how to use BLE component for HeartRate profile application.

The STM32L562E-DK Discovery board acts as GATT server.
Once the code is downloaded on the STM32L562E-DK Discovery board and executed,
the modules are initialized and the device starts advertising.
The user needs an application (running for example on smartphone) in order to act as GATT client.
ST provides the following application (STM32 BLE Profiles):
https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation

The user should activate Bluetooth Smart, execute such application, and SCAN for Bluetooth devices.
The device STM32L562E-DK will be detected.
The user should connect the smartphone to the STM32L562E-DK.
The presence of HeartRate profile should be detected by the smartphone application.
The user should click on the Heart Rate profile (this depends on the smartphone application used).
In general any smartphone application should allow the user to see the heart Rate measurement.
STM32 BLE Profiles smartphone application displays and expresses them in BPM (beat per minute).
In this example STM32L562E-DK Discovery sends dummy values each second.

The STM32L562E-DK example supports the full feature of Heart Rate Service as from specification
https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml
i.e. it provides other information like the sensor location and by writing 0x1 to the Heart Rate Control point
the Energy Expanded field is reset.
STM32 BLE Profiles smartphone application does not support these options but other smartphone applications
can be found on the internet and used to test these features.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Connectivity, BLE, Bluetooth, BlueNRG-MS, BLE pairing, BLE profile, BLE protocol, HCI, SPI

@par Directory contents

 - BLE/HeartRate/Src/main.c                   Main Program
 - BLE/HeartRate/Src/system_stm32l5xx.c       STM32L5xx system clock configuration file
 - BLE/HeartRate/Src/stm32l5xx_it.c           STM32 interrupt handlers
 - BLE/HeartRate/Src/hr.c                     Heart Rate application
 - BLE/HeartRate/Src/hrs_app.c                Heart Rate Service Application
 - BLE/HeartRate/Src/dis_app.c                Device Information Service Application
 - BLE/HeartRate/Inc/config.h                 Application configuration file
 - BLE/HeartRate/Inc/stm32l5xx_hal_conf.h     HAL configuration file
 - BLE/HeartRate/Inc/stm32l5xx_it.h           STM32 interrupt handlers header file
 - BLE/HeartRate/Inc/hr.h                     Exported files and function of hr.c
 - BLE/HeartRate/Inc/hrs_app.h                Exported files and function of hrs_app.c
 - BLE/HeartRate/Inc/dis_app.h                Exported files and function of dis_app.c
 - BLE/Common/                                These files are Common also to other applic/examples then HeartRate
 - BLE/Common/ble_core/*.*                    BlueNRG-MS Bluetooth Low Energy device driver
 - BLE/Common/ble_services/*.*                GATT services according to BLE spec
 - BLE/Common/debug/*.*                       Code useful for debugging (DBG should be activated in config.h)
 - BLE/Common/tl/*.*                          HCI layer and SPI protocol
 - BLE/Common/hw/*.*                          HW board and device dependencies (SPI pins, timeserver, low power)
 - BLE/Common/utilities/*.*                   Scheduler, memory/queue/fifo/list management, otp, etc

@par Hardware and Software environment

  - This application runs on STM32L562XX devices.

  - This application has been tested with the following environment:
     - STM32L562E-DK board (MB1373)

  - When using Android Phone
    - Activate Bluetooth
    - Install a BLE application supporting HeartRate service, e.g.:
      https://play.google.com/store/apps/details?id=com.stm.bluetoothlevalidation
    - SCAN for bluetooth devices and connect to STM32L562E-DK

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
