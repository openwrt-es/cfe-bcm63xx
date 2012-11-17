/*
<:copyright-broadcom 
 
 Copyright (c) 2002 Broadcom Corporation 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom Corporation 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom Corporation 
 company private, proprietary, and trade secret. 
 
:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  board.h                                                  */
/*   PURPOSE: Board specific information.  This module should include  */
/*            all base device addresses and board specific macros.     */
/*                                                                     */
/***********************************************************************/
#ifndef _BOARD_H
#define _BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#define	DYING_GASP_API

/*****************************************************************************/
/*                    Physical Memory Map                                    */
/*****************************************************************************/

#define PHYS_DRAM_BASE           0x00000000     /* Dynamic RAM Base */
#define PHYS_FLASH_BASE          0x1FC00000     /* Flash Memory     */

/*****************************************************************************/
/* Note that the addresses above are physical addresses and that programs    */
/* have to use converted addresses defined below:                            */
/*****************************************************************************/
#define DRAM_BASE           (0x80000000 | PHYS_DRAM_BASE)   /* cached DRAM */
#define DRAM_BASE_NOCACHE   (0xA0000000 | PHYS_DRAM_BASE)   /* uncached DRAM */

/* Binary images are always built for a standard MIPS boot address */
#define IMAGE_BASE          (0xA0000000 | 0x1FC00000)

/* Some chips support alternative boot vector */
#if defined(_BCM96348_) || defined(CONFIG_BCM96348) || defined(_BCM96358_) || defined(CONFIG_BCM96358)
#define FLASH_BASE          (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00))
#define BOOT_OFFSET         (FLASH_BASE - IMAGE_BASE)
#else
#define FLASH_BASE          (0xA0000000 | PHYS_FLASH_BASE)  /* uncached Flash  */
#define BOOT_OFFSET         0
#endif

/*****************************************************************************/
/*  Select the PLL value to get the desired CPU clock frequency.             */
/*****************************************************************************/
#define FPERIPH            50000000

/*****************************************************************************/
/* Board memory type offset                                                  */
/*****************************************************************************/
#define SDRAM_TYPE_ADDRESS_OFFSET       16
#define THREAD_NUM_ADDRESS_OFFSET       20
#define NVRAM_DATA_OFFSET               0x0580
#define NVRAM_DATA_ID                   0x0f1e2d3c
#define BOARD_SDRAM_TYPE                *(unsigned long *)(FLASH_BASE + SDRAM_TYPE_ADDRESS_OFFSET)
#define BOARD_SDRAM_TYPE_ADDRESS        (0xA0000000 + PHYS_FLASH_BASE + SDRAM_TYPE_ADDRESS_OFFSET)
#define BOARD_THREAD_NUM                *(unsigned long *)(FLASH_BASE + THREAD_NUM_ADDRESS_OFFSET)
#define BOARD_THREAD_NUM_ADDRESS        (0xA0000000 + PHYS_FLASH_BASE + THREAD_NUM_ADDRESS_OFFSET)
#define BOARD_IMAGE_DOWNLOAD_ADDRESS    \
    ((cfe_sdramsize > 0x00800000) ? 0x80800000 : 0x80000000) 
#define BOARD_IMAGE_DOWNLOAD_SIZE       \
    ((cfe_sdramsize > 0x00800000) ? 0x00800000 : 0x00400000) 

/*****************************************************************************/
/*       NVRAM Offset and definition                                         */
/*****************************************************************************/
#define NVRAM_PSI_DEFAULT               24              // default psi in K byes
#define ONEK                            1024
#define FLASH_LENGTH_BOOT_ROM           (64*ONEK)
#define FLASH_RESERVED_AT_END           (64*ONEK) /*reserved for PSI, scratch pad*/

#define NVRAM_LENGTH                    ONEK    // 1k nvram 
#define NVRAM_VERSION_NUMBER            2
#define NVRAM_VERSION_NUMBER_ADDRESS    0

#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6
#define NVRAM_MAC_COUNT_MAX             32
#define NVRAM_MAC_COUNT_DEFAULT         0 

/*****************************************************************************/
/*       Misc Offsets                                                        */
/*****************************************************************************/
#define CFE_VERSION_OFFSET           0x0570
#define CFE_VERSION_MARK_SIZE        5
#define CFE_VERSION_SIZE             5

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H */

