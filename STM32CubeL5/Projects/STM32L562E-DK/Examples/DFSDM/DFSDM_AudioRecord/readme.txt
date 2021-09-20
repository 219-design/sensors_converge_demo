/**
  @page DFSDM_AudioRecord  Description of the DFSDM audio record example
  
  @verbatim
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM audio record example.
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

How to use the DFSDM HAL API to perform mono audio recording. This example uses
the SPH0641LM4H-1 digital microphone mounted on the board.

SPH0641LM4H-1 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency has to be delivered by DFSDM. So channel of DFSDM will be configurated
to enable output clock generation, to choose audio clock as output clock source,
and output clock divider will be set to generate output clock on SPH0641LM4H-1 frequency clock range.
Audio clock will be configurated to 11.294MHz, so DFSDM output clock divider will be set to 4.

The digital audio output from the microphone is coded in PDM (Pulse Density Modulation) and 
is connected to PB12. When CLK = 0, the audio PDM signal is sent on PB12.

PB12 can be configurated as DFSDM_DATIN1 (DFSDM channel 1 external data input).
We will use DFSDM channel 1 to get audio PDM signal. DFSDM channel 1 will be configurated to take
inputs from external serial inputs and from pins of channel 1. DFSDM channel 1 will also be 
configurated to operate in SPI mode with rising edge and with internal clock.

DFSDM filter 0 will be used to perform audio PDM signal conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on DFSDM filter 0 in order to fill
a buffer containing audio PCM samples at 44.1KHz.

Playback of the recorded data will be performed on headset using HAL_SAI and CS42L51 audio codec.
A circular playback buffer will be filled as soon as recorded data are available. 
When half record buffer will be filled, we put first part of recorded data on first half of playback buffer. 
When record buffer will be full filled, we put second part of recorded data on second half of playback buffer.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
      from Flash memory and external memories, and reach the maximum performance.

@par Keywords

Audio, DFSDM, Audio record, Digital Microphone, Mono, SPH0641LM4H-1, DMA, PDM, Filter, Playback, Audio codec, SAI

@par Directory contents  

  - DFSDM/DFSDM_AudioRecord/Src/main.c                       Main program
  - DFSDM/DFSDM_AudioRecord/Src/stm32l5xx_hal_msp.c          MSP initializations and de-initializations
  - DFSDM/DFSDM_AudioRecord/Src/system_stm32l5xx.c           STM32L5xx system source file
  - DFSDM/DFSDM_AudioRecord/Src/stm32l5xx_it.c               Interrupt handlers
  - DFSDM/DFSDM_AudioRecord/Inc/main.h                       Main program header file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32l5xx_hal_conf.h         HAL configuration file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32l562e_discovery_conf.h  Board configuration file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32l5xx_it.h               Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32L562QEIxQ devices.
    
  - This example has been tested with STMicroelectronics STM32L562E-DK
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32L562E-DK Set-up :
    - Make sure that nothing is plugged on CN2 connector.
    - Make sure that JP1 is in open state.
    - Plug headset on CN13 connector.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
