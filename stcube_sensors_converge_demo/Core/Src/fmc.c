/**
  ******************************************************************************
  * File Name          : FMC.c
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.NBLSetupTime = 0;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  hsram1.Init.MaxChipSelectPulse = DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.DataHoldTime = 0;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PD5   ------> FMC_NWE
  PD4   ------> FMC_NOE
  PD1   ------> FMC_DA3
  PD1   ------> FMC_D3
  PD0   ------> FMC_DA2
  PD0   ------> FMC_D2
  PF0   ------> FMC_A0
  PD7   ------> FMC_NE1
  PD14   ------> FMC_DA0
  PD14   ------> FMC_D0
  PD15   ------> FMC_DA1
  PD15   ------> FMC_D1
  PE8   ------> FMC_DA5
  PE8   ------> FMC_D5
  PE10   ------> FMC_DA7
  PE10   ------> FMC_D7
  PE12   ------> FMC_DA9
  PE12   ------> FMC_D9
  PD9   ------> FMC_DA14
  PD9   ------> FMC_D14
  PE7   ------> FMC_DA4
  PE7   ------> FMC_D4
  PE14   ------> FMC_DA11
  PE14   ------> FMC_D11
  PE11   ------> FMC_DA8
  PE11   ------> FMC_D8
  PE15   ------> FMC_DA12
  PE15   ------> FMC_D12
  PD8   ------> FMC_DA13
  PD8   ------> FMC_D13
  PE9   ------> FMC_DA6
  PE9   ------> FMC_D6
  PE13   ------> FMC_DA10
  PE13   ------> FMC_D10
  PD10   ------> FMC_DA15
  PD10   ------> FMC_D15
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_0
                          |GPIO_PIN_7|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_9
                          |GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_7
                          |GPIO_PIN_14|GPIO_PIN_11|GPIO_PIN_15|GPIO_PIN_9
                          |GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN FMC_MspInit 1 */

  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PD5   ------> FMC_NWE
  PD4   ------> FMC_NOE
  PD1   ------> FMC_DA3
  PD1   ------> FMC_D3
  PD0   ------> FMC_DA2
  PD0   ------> FMC_D2
  PF0   ------> FMC_A0
  PD7   ------> FMC_NE1
  PD14   ------> FMC_DA0
  PD14   ------> FMC_D0
  PD15   ------> FMC_DA1
  PD15   ------> FMC_D1
  PE8   ------> FMC_DA5
  PE8   ------> FMC_D5
  PE10   ------> FMC_DA7
  PE10   ------> FMC_D7
  PE12   ------> FMC_DA9
  PE12   ------> FMC_D9
  PD9   ------> FMC_DA14
  PD9   ------> FMC_D14
  PE7   ------> FMC_DA4
  PE7   ------> FMC_D4
  PE14   ------> FMC_DA11
  PE14   ------> FMC_D11
  PE11   ------> FMC_DA8
  PE11   ------> FMC_D8
  PE15   ------> FMC_DA12
  PE15   ------> FMC_D12
  PD8   ------> FMC_DA13
  PD8   ------> FMC_D13
  PE9   ------> FMC_DA6
  PE9   ------> FMC_D6
  PE13   ------> FMC_DA10
  PE13   ------> FMC_D10
  PD10   ------> FMC_DA15
  PD10   ------> FMC_D15
  */

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_0
                          |GPIO_PIN_7|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_9
                          |GPIO_PIN_8|GPIO_PIN_10);

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_7
                          |GPIO_PIN_14|GPIO_PIN_11|GPIO_PIN_15|GPIO_PIN_9
                          |GPIO_PIN_13);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
