/**
  ******************************************************************************
  * @file    fw_update_app.c
  * @author  MCD Application Team
  * @brief   Firmware Update module.
  *          This file provides set of firmware functions to manage Firmware
  *          Update functionalities.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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

#include "stm32l5xx_hal.h"
#include "com.h"
#include "common.h"
/*#include "low_level_flash.h"*/
#include "ymodem.h"
#include "fw_update_app.h"
#include "region_defs.h"
#include "Driver_Flash.h"
#include "string.h"
#ifdef MCUBOOT_PRIMARY_ONLY
#include "secure_nsc.h"
#endif /* MCUBOOT_PRIMARY_ONLY */

/** @addtogroup USER_APP User App Example
  * @{
  */
extern ARM_DRIVER_FLASH LOADER_FLASH_DEV_NAME;
/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */
/** @defgroup  FW_UPDATE_Private_Variables Private Variables
  * @{
  */
static uint32_t m_uFileSizeYmodem = 0U;  /* !< Ymodem File size*/
static uint32_t m_uNbrBlocksYmodem = 0U; /* !< Ymodem Number of blocks*/
static uint32_t m_uPacketsReceived = 0U; /* !< Ymodem packets received*/
static uint32_t m_uFlashSectorSize = 0U; /* !< Flash Sector Size */
static uint32_t m_uFlashMinWriteSize = 0U;/*!< FLash Min Write access*/
#if !defined(MCUBOOT_PRIMARY_ONLY)
/** @defgroup  FW_UPDATE_Private_Const Private Const
  * @{
  */
const uint32_t MagicTrailerValue[] =
{
  0xf395c277,
  0x7fefd260,
  0x0f505235,
  0x8079b62c,
};
#endif /* !defined(MCUBOOT_PRIMARY_ONLY) */
/**
  * @}
  */

/** @defgroup  FW_UPDATE_Private_Functions Private Functions
  * @{
  */
static void FW_UPDATE_PrintWelcome(void);
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea);
static HAL_StatusTypeDef FW_UPDATE_SECURE_IMAGE(void);
#if (MCUBOOT_IMAGE_NUMBER == 2)
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_IMAGE(void);
#endif /* (MCUBOOT_IMAGE_NUMBER == 2) */
/**
  * @}
  */
/** @defgroup  FW_UPDATE_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup  FW_UPDATE_Control_Functions Control Functions
  * @{
   */
void FW_UPDATE_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;

  /* Print Firmware Update welcome message */
  FW_UPDATE_PrintWelcome();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      switch (key)
      {
        case '1' :
          printf("  -- Install image : reboot\r\n\n");
          NVIC_SystemReset();
          break;
        case '2' :
          FW_UPDATE_SECURE_IMAGE();
          break;
#if (MCUBOOT_IMAGE_NUMBER == 2)
        case '3' :
          FW_UPDATE_NONSECURE_IMAGE();
          break;
#endif /* MCUBOOT_IMAGE_NUMBER == 2 */
        default:
          printf("Invalid Number !\r");
          break;
      }
      /* Print Main Menu message */
      FW_UPDATE_PrintWelcome();
    }

  }
}

/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_SECURE_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
#if (MCUBOOT_IMAGE_NUMBER == 2)
  printf("Download Secure Image\r\n");
#else
  printf("Download Image\r\n");
#endif /* (MCUBOOT_IMAGE_NUMBER == 2) */
  /* Get Info about the download area */
#if defined(MCUBOOT_PRIMARY_ONLY)
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_0_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_AREA_0_SIZE;
#else
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_2_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = FLASH_AREA_2_SIZE;
#endif /* MCUBOOT_PRIMARY_ONLY */
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
#if (MCUBOOT_IMAGE_NUMBER == 2)
    printf("  -- Secure Image correctly downloaded \r\n\n");
#else
    printf("  -- Image correctly downloaded \r\n\n");
#endif /* (MCUBOOT_IMAGE_NUMBER == 2) */
    HAL_Delay(1000U);
  }

  return ret;
}
#if (MCUBOOT_IMAGE_NUMBER == 2)
/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_NONSECURE_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download NonSecure Image\r\n");

  /* Get Info about the download area */
#if defined(MCUBOOT_PRIMARY_ONLY)
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_1_OFFSET;
#else
  fw_image_dwl_area.DownloadAddr =  FLASH_AREA_3_OFFSET;
#endif /* MCUBOOT_PRIMARY_ONLY */
  fw_image_dwl_area.MaxSizeInBytes = FLASH_NS_PARTITION_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- NonSecure Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}
#endif /* (MCUBOOT_IMAGE_NUMBER == 2) */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup  FW_UPDATE_Private_Functions
  * @{
  */

/**
  * @brief  Display the FW_UPDATE Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_UPDATE_PrintWelcome(void)
{
  printf("\r\n================ New Fw Download =========================\r\n\n");
  printf("  Reset to trigger Installation ------------------------- 1\r\n\n");
#if (MCUBOOT_IMAGE_NUMBER == 2)
  printf("  Download Secure Image --------------------------------- 2\r\n\n");
  printf("  Download NonSecure Image ------------------------------ 3\r\n\n");
#else
  printf("  Download Image ---------------------------------------- 2\r\n\n");
#endif /* (MCUBOOT_IMAGE_NUMBER == 2) */
}
/**
  * @brief Download a new Firmware from the host.
  * @retval HAL status
  */
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  COM_StatusTypeDef e_result;
  int32_t ret_arm;
  uint32_t u_fw_size = pFwImageDwlArea->MaxSizeInBytes ;
  uint32_t sector_address;

  /* Clear download area */
  printf("  -- Erasing download area \r\n\n");

  for (sector_address = pFwImageDwlArea->DownloadAddr;
       sector_address < pFwImageDwlArea->DownloadAddr + pFwImageDwlArea->MaxSizeInBytes;
       sector_address += m_uFlashSectorSize)
  {
#if defined(MCUBOOT_PRIMARY_ONLY)
    if (sector_address < NS_IMAGE_PRIMARY_PARTITION_OFFSET)
    {
      ret_arm = SECURE_Flash_EraseSector(sector_address);
    }
    else
    {
      ret_arm = LOADER_FLASH_DEV_NAME.EraseSector(sector_address);
    }
#else
    ret_arm = LOADER_FLASH_DEV_NAME.EraseSector(sector_address);
#endif /*  defined(MCUBOOT_PRIMARY_ONLY) */
    if (ret_arm < 0)
    {
      return HAL_ERROR;
    }
  }

  printf("  -- Send Firmware \r\n\n");

  /* Download binary */
  printf("  -- -- File> Transfer> YMODEM> Send \t\n");

  /*Init of Ymodem*/
  Ymodem_Init();

  /*Receive through Ymodem*/
  e_result = Ymodem_Receive(&u_fw_size, pFwImageDwlArea->DownloadAddr);
  printf("\r\n\n");

  if ((e_result == COM_OK))
  {
    printf("  -- -- Programming Completed Successfully!\r\n\n");
#if defined(__ARMCC_VERSION)
    printf("  -- -- Bytes: %u\r\n\n", u_fw_size);
#else
    printf("  -- -- Bytes: %lu\r\n\n", u_fw_size);
#endif /*  __ARMCC_VERSION */
    ret = HAL_OK;
#if !defined(MCUBOOT_PRIMARY_ONLY)
    if (u_fw_size <= (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue)))
    {
      uint32_t MagicAddress =
        pFwImageDwlArea->DownloadAddr + (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue));
      /* write the magic to trigger installation at next reset */
#if defined(__ARMCC_VERSION)
      printf("  Write Magic Trailer at %x\r\n\n", MagicAddress);
#else
      printf("  Write Magic Trailer at %lx\r\n\n", MagicAddress);
#endif /*  __ARMCC_VERSION */
      if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
      {
        ret = HAL_ERROR;
      }
    }
#endif /* !defined(MCUBOOT_PRIMARY_ONLY) */
  }
  else if (e_result == COM_ABORT)
  {
    printf("  -- -- !!Aborted by user!!\r\n\n");
    COM_Flush();
    ret = HAL_ERROR;
  }
  else
  {
    printf("  -- -- !!Error during file download!!\r\n\n");
    ret = HAL_ERROR;
    HAL_Delay(500U);
    COM_Flush();
  }

  return ret;
}


/**
  * @}
  */

/** @defgroup FW_UPDATE_Callback_Functions Callback Functions
  * @{
  */

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  uFileSize Dimension of the file that will be received (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{
  /*Reset of the ymodem variables */
  m_uFileSizeYmodem = 0U;
  m_uPacketsReceived = 0U;
  m_uNbrBlocksYmodem = 0U;

  /*Filesize information is stored*/
  m_uFileSizeYmodem = uFileSize;

  /* compute the number of 1K blocks */
  m_uNbrBlocksYmodem = (m_uFileSizeYmodem + (PACKET_1K_SIZE - 1U)) / PACKET_1K_SIZE;

  /* NOTE : delay inserted for Ymodem protocol*/
  HAL_Delay(1000);
  return HAL_OK;
}

extern uint32_t total_size_received;
/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  uSize Packet dimension (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{
  int32_t ret;
  m_uPacketsReceived++;

  /*Increase the number of received packets*/
  if (m_uPacketsReceived == m_uNbrBlocksYmodem) /*Last Packet*/
  {
    /*Extracting actual payload from last packet*/
    if (0 == (m_uFileSizeYmodem % PACKET_1K_SIZE))
    {
      /* The last packet must be fully considered */
      uSize = PACKET_1K_SIZE;
    }
    else
    {
      /* The last packet is not full, drop the extra bytes */
      uSize = m_uFileSizeYmodem - ((uint32_t)(m_uFileSizeYmodem / PACKET_1K_SIZE) * PACKET_1K_SIZE);
    }

    m_uPacketsReceived = 0U;
  }
  /*Adjust dimension to 64-bit length */
  if (uSize %  m_uFlashMinWriteSize != 0U)
  {
    memset(&pData[uSize], 0xff, (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize)));
    uSize += (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize));
  }
  /* Write Data in Flash - size has to be 64-bit aligned */
#if defined(MCUBOOT_PRIMARY_ONLY)
  if (uFlashDestination < NS_IMAGE_PRIMARY_PARTITION_OFFSET)
  {
    ret = SECURE_Flash_ProgramData(uFlashDestination, pData, uSize);
  }
  else
    ret = LOADER_FLASH_DEV_NAME.ProgramData(uFlashDestination, pData, uSize);
#else
  ret = LOADER_FLASH_DEV_NAME.ProgramData(uFlashDestination, pData, uSize);
#endif /*defined(MCUBOOT_PRIMARY_ONLY) */
  if (ret < 0)
  {
    /*Reset of the ymodem variables */
    m_uFileSizeYmodem = 0U;
    m_uPacketsReceived = 0U;
    m_uNbrBlocksYmodem = 0U;
    return HAL_ERROR;
  }
  else
    return HAL_OK;
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
