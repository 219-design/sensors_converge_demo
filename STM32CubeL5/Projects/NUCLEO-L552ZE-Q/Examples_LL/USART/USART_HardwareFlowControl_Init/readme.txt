/**
  @page USART_HardwareFlowControl_Init USART example (IT Mode with HW Flow Control)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/USART/USART_HardwareFlowControl_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_HardwareFlowControl_Init example.
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

Configuration of GPIO and  peripheral 
to receive characters asynchronously from an HyperTerminal (PC) in Interrupt mode 
with the Hardware Flow Control feature enabled. This example is based on STM32L5xx 
USART LL API. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

USART3 Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
HW flow control is used.
GPIO associated to User push-button is linked with EXTI.

Example execution:
After startup from reset and system configuration, LED1 is blinking quickly.
On press on User push-button, USART TX Empty interrupt is enabled.
First character of buffer to be transmitted is written into USART Transmit Data Register (TDR)
in order to initialise transfer procedure.
When character is sent from TDR, a TXE interrupt occurs.
USART3 IRQ Handler routine is sending next character on USART Tx line.
IT will be raised until last byte is to be transmitted : Then, Transmit Complete (TC) interrupt is enabled 
instead of TX Empty (TXE).
When character is received on USART Rx line, a RXNE interrupt occurs.
USART3 IRQ Handler routine is then checking received character value.
On a specific value ('S' or 's'), LED1 is turned On.
Received character is echoed on Tx line.
In case of errors, LED1 is blinking.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, 
hardware flow control, CTS/RTS, Transmitter, Receiver

@par Directory contents 

  - USART/USART_HardwareFlowControl_Init/Inc/stm32l5xx_it.h          Interrupt handlers header file
  - USART/USART_HardwareFlowControl_Init/Inc/main.h                  Header for main.c module
  - USART/USART_HardwareFlowControl_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_HardwareFlowControl_Init/Src/stm32l5xx_it.c          Interrupt handlers
  - USART/USART_HardwareFlowControl_Init/Src/main.c                  Main program
  - USART/USART_HardwareFlowControl_Init/Src/system_stm32l5xx.c      STM32L5xx system source file

@par Hardware and Software environment

  - This example runs on STM32L552ZETxQ devices.

  - This example has been tested with NUCLEO-L552ZE-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-L552ZE-Q Set-up
    GPIOs connected to USART3 TX/RX/CTS/RTS signals should be wired
    to corresponding pins on PC UART (could be done through a USB to UART adapter).
    - Connect STM32 MCU board USART3 TX pin (PD.08 connected to pin 14 of Arduino connector CN10)
      to PC COM port RX signal
    - Connect STM32 MCU board USART3 RX pin (PD.09 connected to pin 16 of Arduino connector CN10)
      to PC COM port TX signal
    - Connect STM32 MCU board USART3 CTS pin (PA.06 connected to pin 12 of Arduino connector CN7)
      to PC COM port RTS signal
    - Connect STM32 MCU board USART3 RTS pin (PB.01 connected to pin 7 of Arduino connector CN10)
      to PC COM port CTS signal
    - Connect STM32 MCU board GND to PC COM port GND signal

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, HW flow control enabled).

  - Launch the program. Enter characters on PC communication SW side.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
