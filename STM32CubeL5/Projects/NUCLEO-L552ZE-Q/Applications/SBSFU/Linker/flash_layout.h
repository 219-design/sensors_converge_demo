/*
 * Copyright (c) 2018 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Flash layout configuration : begin */
#define MCUBOOT_PRIMARY_ONLY       /* Defined: No secondary (download) slot(s),
                                               only primary slot(s) for each image.
                                      Undefined: Primary and secondary slot(s) for each image. */

#define MCUBOOT_EXT_LOADER         /* Defined: Add external local loader application.
                                               To enter it, press user button at reset.
                                      Undefined: No external local loader application. */

#define MCUBOOT_IMAGE_NUMBER 1     /* 1: S and NS application binaries are assembled in one single image.
                                      2: Two separated images for S and NS application binaries. */

/* Flash layout configuration : end */


/* The size of a partition. This should be large enough to contain a S or NS
 * sw binary. Each FLASH_AREA_IMAGE contains two partitions. See Flash layout
 * above.
 */
#ifdef MCUBOOT_EXT_LOADER
#define LOADER_FLASH_DEV_NAME             TFM_Driver_FLASH0
#endif

/* Flash layout info for BL2 bootloader */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)     /* 4 KB */
#define FLASH_B_SIZE                    (0x40000)    /* 256 Kbytes*/
#define FLASH_TOTAL_SIZE                (FLASH_B_SIZE+FLASH_B_SIZE) /* 512 Kbytes */
#define FLASH_BASE_ADDRESS              (0x0c000000) /* same as FLASH0_BASE_S */


/* Flash device ID */


/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */
/* area for HUK and anti roll back counter */
#define FLASH_BL2_NVCNT_AREA_OFFSET     (0x0000)
#define FLASH_BL2_NVCNT_AREA_SIZE       (0x1000)
/* scratch area */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_BL2_NVCNT_AREA_OFFSET+FLASH_BL2_NVCNT_AREA_SIZE)
#define FLASH_AREA_SCRATCH_SIZE         (0x0000) /* Not used in MCUBOOT_OVERWRITE_ONLY mode */
/* personal area */
#define FLASH_AREA_PERSO_OFFSET         (FLASH_AREA_SCRATCH_OFFSET+FLASH_AREA_SCRATCH_SIZE)
#define FLASH_AREA_PERSO_SIZE           (0x800)
#else
#define FLASH_AREA_PERSO_OFFSET         (FLASH_BL2_NVCNT_AREA_OFFSET+FLASH_BL2_NVCNT_AREA_SIZE)
#define FLASH_AREA_PERSO_SIZE           (0x800)
#endif /* MCUBOOT_PRIMARY_ONLY */
/* area for BL2 code protected by hdp */
#define FLASH_AREA_BL2_OFFSET           (FLASH_AREA_PERSO_OFFSET+FLASH_AREA_PERSO_SIZE)
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_AREA_BL2_SIZE             (0xc000)
#else
#define FLASH_AREA_BL2_SIZE             (0xb000)
#endif
/* HDP area end at this address */
#define FLASH_BL2_HDP_END               (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE-1)
/* area for BL2 code not protected by hdp */
#define FLASH_AREA_BL2_NOHDP_OFFSET     (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_BL2_NOHDP_SIZE       (0x800)

#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_S_PARTITION_SIZE          (0x04000) /* 16 KB for S partition */
#if defined(MCUBOOT_EXT_LOADER)
#define FLASH_NS_PARTITION_SIZE         (0x31000) /* 208 KB for NS partition */
#else
#define FLASH_NS_PARTITION_SIZE         (0x34000) /* 208 KB for NS partition */
#endif /* MCUBOOT_EXT_LOADER */
#else
#define FLASH_S_PARTITION_SIZE          (0x08000) /* 32 KB for S partition */
#define FLASH_NS_PARTITION_SIZE         (0x63000) /* 416 KB for NS partition */
#endif /* MCUBOOT_PRIMARY_ONLY */
#define FLASH_PARTITION_SIZE            (FLASH_S_PARTITION_SIZE+FLASH_NS_PARTITION_SIZE)

#if MCUBOOT_IMAGE_NUMBER == 2
/* Secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_DEVICE_ID          (FLASH_DEVICE_ID-FLASH_DEVICE_ID)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_SIZE)
#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_DEVICE_ID          (FLASH_AREA_0_DEVICE_ID)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE               (FLASH_NS_PARTITION_SIZE)
#if !defined(MCUBOOT_PRIMARY_ONLY)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_DEVICE_ID          (FLASH_AREA_1_DEVICE_ID)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE               (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID                 (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_DEVICE_ID          (FLASH_AREA_2_DEVICE_ID)
#define FLASH_AREA_3_OFFSET             (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE               (FLASH_NS_PARTITION_SIZE)
#define FLASH_AREA_END_OFFSET			(FLASH_AREA_3_OFFSET+FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_DEVICE_ID    (FLASH_DEVICE_ID)
#else
#define FLASH_AREA_END_OFFSET			(FLASH_AREA_1_OFFSET+FLASH_AREA_1_SIZE)
#endif /* MCUBOOT_PRIMARY_ONLY */
#else
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_DEVICE_ID          (FLASH_DEVICE_ID-FLASH_DEVICE_ID)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_SIZE)
#define FLASH_AREA_0_SIZE               (FLASH_PARTITION_SIZE)
/* Non-secure image primary slot */
#if !defined(MCUBOOT_PRIMARY_ONLY)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_DEVICE_ID          (FLASH_AREA_0_DEVICE_ID)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE               (FLASH_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_END_OFFSET			(FLASH_AREA_2_OFFSET+FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_DEVICE_ID    (FLASH_DEVICE_ID)
#else
#define FLASH_AREA_END_OFFSET			(FLASH_AREA_0_OFFSET+FLASH_AREA_0_SIZE)
#endif /* MCUBOOT_PRIMARY_ONLY */

#endif /* MCUBOOT_IMAGE_NUMBER == 2 */

/*
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES         ((2 * FLASH_PARTITION_SIZE) / \
                                         FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS           ((2 * FLASH_PARTITION_SIZE) / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)
										 
#define FLASH_NV_COUNTERS_AREA_SIZE     (0x18)     /* 16 Bytes */

#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE




/* Flash device name used by BL2 and NV Counter
 * Name is defined in flash driver file: low_level_flash.c
 */

#define FLASH_DEV_NAME                  TFM_Driver_FLASH0

/* Secure Storage (SST) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M SST Integration Guide.
 */
#define SST_FLASH_DEV_NAME              TFM_Driver_FLASH0

/* Secure Storage (SST) Service definitions */
/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define SST_FLASH_AREA_ADDR             FLASH_SST_AREA_OFFSET
#define SST_SECTOR_SIZE                 FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define SST_NBR_OF_SECTORS              (FLASH_SST_AREA_SIZE / SST_SECTOR_SIZE)
/* Specifies the smallest flash programmable unit in bytes */
#define SST_FLASH_PROGRAM_UNIT          (0x8)
/* The maximum asset size to be stored in the SST area */
#define SST_MAX_ASSET_SIZE              (512)
/* The maximum number of assets to be stored in the SST area */
#define SST_NUM_ASSETS                  (10)


#define ITS_FLASH_DEV_NAME              TFM_Driver_FLASH0

#define ITS_FLASH_AREA_ADDR             FLASH_ITS_AREA_OFFSET
#define ITS_SECTOR_SIZE                 FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define ITS_NBR_OF_SECTORS              (FLASH_ITS_AREA_SIZE / ITS_SECTOR_SIZE)
/* Specifies the smallest flash programmable unit in bytes */
#define ITS_FLASH_PROGRAM_UNIT          (0x8)
/* The maximum asset size to be stored in the ITS area */
#define ITS_MAX_ASSET_SIZE              (512)
/* The maximum number of assets to be stored in the ITS area */
#define ITS_NUM_ASSETS                  (10)

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR        FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE        (0x18)/* 24 Bytes */
#define TFM_NV_COUNTERS_SECTOR_ADDR      FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE      FLASH_AREA_IMAGE_SECTOR_SIZE

/* BL2 NV Counters definitions  */
#define BL2_NV_COUNTERS_AREA_ADDR        FLASH_BL2_NVCNT_AREA_OFFSET
#define BL2_NV_COUNTERS_AREA_SIZE        FLASH_BL2_NVCNT_AREA_SIZE

/* FIXME: not valid today */
#define BL2_S_RAM_ALIAS_BASE             (0x30000000)
#define BL2_NS_RAM_ALIAS_BASE            (0x20000000)

/*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
#define BOOT_TFM_SHARED_DATA_SIZE        (0x0)
#define BOOT_TFM_SHARED_DATA_BASE        (0x30040000)
#endif /* __FLASH_LAYOUT_H__ */
