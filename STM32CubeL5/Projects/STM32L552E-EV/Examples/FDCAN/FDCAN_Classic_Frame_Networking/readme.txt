/**
  @page FDCAN_Classic_Frame_Networking FDCAN Classic Frame Networking example

  @verbatim
  ******************************************************************************
  * @file    FDCAN/FDCAN_Classic_Frame_Networking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Classic_Frame_Networking ewample.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure the FDCAN peripheral to send and receive Classic CAN frames.

The sent frames are used to control LEDs by pressing WKUP push-button.

The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

FDCAN peripheral is configured to operate in classic CAN frame mode, with CAN
Bit Rate of 1 MBit/s.

This program behaves as follows:

  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the Flash interface and the systick.
  The SystemClock_Config() function is used to configure the system clock (SYSCLK)
  to run at 110 MHz.

  After reset, all LEDs are OFF on the couple of eval boards connected to the CAN bus.
  By Pressing WKUP push-button: a CAN frame is sent on the CAN bus, to turn
  ON LED4 and turn OFF all other LEDs on both eval boards.
  Each time WKUP push-button is pressed again, the LEDn+1 is turned ON, and all
  other LEDs are turned OFF on both eval boards.

  If at any time of the process an error is encountered, all LEDs are turned ON.

@note Any unit in the CAN bus may play the role of sender (by pressing
      WKUP push-button) or receiver.

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

FDCAN, Networking, Classic, CAN

@par Directory contents

  - FDCAN/FDCAN_Classic_Frame_Networking/Inc/stm32l552e_eval_conf.h     BSP configuration file
  - FDCAN/FDCAN_Loopback/Inc/stm32l5xx_hal_conf.h    HAL configuration file
  - FDCAN/FDCAN_Loopback/Inc/stm32l5xx_it.h          Header for stm32l5xx_it.c
  - FDCAN/FDCAN_Loopback/Inc/main.h                  Header for main.c module
  - FDCAN/FDCAN_Loopback/Src/stm32l5xx_it.c          Interrupt handlers
  - FDCAN/FDCAN_Loopback/Src/main.c                  Main program
  - FDCAN/FDCAN_Loopback/Src/stm32l5xx_hal_msp.c     HAL MSP module
  - FDCAN/FDCAN_Loopback/Src/system_stm32l5xx.c      stm32l5xx system source file

@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.

  - This example has been tested with a couple of STM32L552E-EV board and can be
    easily tailored to any other supported device and development board.

  - STM32L552E-EV set-up:
    - Jumper JP17 => fitted on position [1-2] to select high-speed mode operation of the CAN transeiver(on each board).
    - Jumper JP18 => fitted to connect terminal resistor on CAN physical link (on each board).
	- Jumper JP19 => fitted to connect CAN_RX pin PB.08 from the STM32L552ZET6QU terminal ti the DB9-male CAN connector (on each board).
    - Connect the 2 points FDCAN CN17 Connector CAN connector (CAN-H and CAN-L) of the first eval board,
      to the 2 points FDCAN CN17 Connector CAN connectors (resp. CAN-H and CAN-L) of the second eval board.
       _________________________                       _________________________ 
      |           ______________|                     |______________           |
      |          |FDCAN1        |                     |        FDCAN1|          |
      |          |              |                     |              |          |
      |          |       CAN    |1___________________1|    CAN       |          |
      |          |    connector |2___________________2| connector    |          |
      |          |      (FDCAN CN17 Connector)  |                     |   (FDCAN CN17 Connector)     |          |
      |          |______________|                     |______________|          |
      |                         |                     |                         |
      |   _    _                |                     |   _    _                |
      |  |_|  |_|               |                     |  |_|  |_|               |
      | LED4 LED5               |                     | LED4 LED5               |
      |          _              |                     |          _              |
      |         (_)             |                     |         (_)             |
      |         User            |                     |         User            |
      |                         |                     |                         |
      |      STM32 Board 1      |                     |      STM32 Board 2      |
      |                         |                     |                         |
      |_________________________|                     |_________________________|

  - This example can also be tested by connecting the CAN connector to a different
    development board (ex: STM32373C-EVAL...) loaded with the corresponding CAN Networking example.
    In this case, user must ensure that the CAN bit rate configuration is also equal to 1Mbit/s.

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
