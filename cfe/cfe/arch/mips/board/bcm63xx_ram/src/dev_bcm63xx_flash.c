/***************************************************************************
 * Broadcom Corp. Confidential
 * Copyright 2001 Broadcom Corp. All Rights Reserved.
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED 
 * SOFTWARE LICENSE AGREEMENT BETWEEN THE USER AND BROADCOM. 
 * YOU HAVE NO RIGHT TO USE OR EXPLOIT THIS MATERIAL EXCEPT 
 * SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 ***************************************************************************
 * File Name  : bcm63xx_flash.c
 *
 * Description: This file contains the flash device driver for bcm63xx board. Very similar to
 *              board.c in linux development.
 *
 * Created on :  4/18/2002  seanl:  use cfiflash.c, cfliflash.h (AMD specific)
 *
 ***************************************************************************/


/* Includes. */
#include "lib_types.h"
#include "lib_malloc.h"
#include "lib_string.h"
#include "lib_printf.h"

#include "bcm_map.h"       
#include "board.h"
#include "dev_bcm63xx_flash.h"
#include "flash_api.h"
#include "boardparms.h"

//#define DEBUG_FLASH 

extern void dumpHex(unsigned char*, int);
extern NVRAM_DATA nvramData;            
extern int readNvramData(void);
extern int writeNvramData(void);
extern void setBoardIdName(void);
void getMacAddress(unsigned char *hwaddr);


/* This driver determines the NVRAM and persistent storage flash address and
 * length.
 */

static FLASH_ADDR_INFO fInfo;

//**************************************************************************************
// Flash read/write and image downloading..
//**************************************************************************************

void kerSysFlashInit( void )
{
    int i = 0;
    int totalBlks = 0;
    int totalSize = 0;
    int startAddr = 0;
    int usedBlkSize = 0;
    int updateNvramData = 0;

    flash_init();

    totalBlks = flash_get_numsectors();
    totalSize = flash_get_total_size();

#if defined(DEBUG_FLASH)
    printf("totalBlks=%d\n", totalBlks);
    printf("totalSize=%dK\n", totalSize/1024);
#endif

    /* Temporarily set the NVRAM flash information to the old location (end of
     * flash memory) to see if there is valid data there.  If there is, move it
     * to the new location.
     */
    fInfo.flash_nvram_length = NVRAM_LENGTH;
    fInfo.flash_nvram_start_blk = totalBlks - 1; /* last block */
    fInfo.flash_nvram_number_blk = 1; /* always fits into one block */
    fInfo.flash_nvram_blk_offset =
        flash_get_sector_size((unsigned short) fInfo.flash_nvram_start_blk) - NVRAM_LENGTH;

    if (readNvramData() == 0)
    {
        /* Old NVRAM location has valid data.  Invalidate the data at the old
         * location.
         */
        NVRAM_DATA invalidate;
        memset( &invalidate, 0xff, sizeof(invalidate) );
        kerSysNvRamSet( (char *)&invalidate, sizeof(invalidate),
            NVRAM_VERSION_NUMBER_ADDRESS );

        /* Set flag to write the valid data to the new NVRAM location. */
        updateNvramData = 1;
    }

    /* Set current NVRAM flash information. */
    fInfo.flash_nvram_length = NVRAM_LENGTH;
    fInfo.flash_nvram_start_blk = 0;  /* always the first block */
    fInfo.flash_nvram_number_blk = 1; /* always fits in the first block */
    fInfo.flash_nvram_blk_offset = NVRAM_DATA_OFFSET;
    
    if( updateNvramData == 1 )
        writeNvramData();

    if (readNvramData() != 0)
        fInfo.flash_persistent_length = NVRAM_PSI_DEFAULT;
    else
    {
        unsigned long ulPsiSize;

        /* Sets the board parameters to the board id name that is saved in NVRAM.
         * If a baord id name has not yet been saved to NVRAM, this function will
         * fail but the operator will be forced into the board parameters command
         * ('b').
         */
        setBoardIdName();

        if( BpGetPsiSize( &ulPsiSize ) == BP_SUCCESS )
            fInfo.flash_persistent_length = ulPsiSize;
        else
            fInfo.flash_persistent_length = NVRAM_PSI_DEFAULT;
    }

    fInfo.flash_persistent_length *= ONEK;
    startAddr = totalSize - fInfo.flash_persistent_length;
    fInfo.flash_persistent_start_blk = flash_get_blk(startAddr+FLASH_BASE);
    fInfo.flash_persistent_number_blk = totalBlks - fInfo.flash_persistent_start_blk;
    // find out the offset in the start_blk
    usedBlkSize = 0;
    for (i = fInfo.flash_persistent_start_blk; 
        i < (fInfo.flash_persistent_start_blk + fInfo.flash_persistent_number_blk); i++)
        usedBlkSize += flash_get_sector_size((unsigned short) i);

    fInfo.flash_persistent_blk_offset =  usedBlkSize - fInfo.flash_persistent_length;

#if defined(DEBUG_FLASH)
    printf("fInfo.flash_nvram_start_blk = %d\n", fInfo.flash_nvram_start_blk);
    printf("fInfo.flash_nvram_blk_offset = 0x%x\n", fInfo.flash_nvram_blk_offset);
    printf("fInfo.flash_nvram_number_blk = %d\n", fInfo.flash_nvram_number_blk);

    printf("psi startAddr = %x\n", startAddr+FLASH_BASE);
    printf("fInfo.flash_persistent_start_blk = %d\n", fInfo.flash_persistent_start_blk);
    printf("fInfo.flash_persistent_blk_offset = 0x%x\n", fInfo.flash_persistent_blk_offset);
    printf("fInfo.flash_persistent_number_blk = %d\n", fInfo.flash_persistent_number_blk);
#endif

}

/***********************************************************************
 * Function Name: kerSysFlashAddrInfoGet
 * Description  : Fills in a structure with information about the NVRAM
 *                and persistent storage sections of flash memory.
 * Returns      : None.
 ***********************************************************************/
void kerSysFlashAddrInfoGet(PFLASH_ADDR_INFO pflash_addr_info)
{
    pflash_addr_info->flash_nvram_blk_offset = fInfo.flash_nvram_blk_offset;
    pflash_addr_info->flash_nvram_length = fInfo.flash_nvram_length;
    pflash_addr_info->flash_nvram_number_blk = fInfo.flash_nvram_number_blk;
    pflash_addr_info->flash_nvram_start_blk = fInfo.flash_nvram_start_blk;
    pflash_addr_info->flash_persistent_blk_offset = fInfo.flash_persistent_blk_offset;
    pflash_addr_info->flash_persistent_length = fInfo.flash_persistent_length;
    pflash_addr_info->flash_persistent_number_blk = fInfo.flash_persistent_number_blk;
    pflash_addr_info->flash_persistent_start_blk = fInfo.flash_persistent_start_blk;
}


// get shared blks into *** pTempBuf *** which has to be released bye the caller!
// return: if pTempBuf != NULL, poits to the data with the dataSize of the buffer
// !NULL -- ok
// NULL  -- fail
static char *getSharedBlks(int start_blk, int end_blk)
{
    int i = 0;
    int usedBlkSize = 0;
    int sect_size = 0;
    char *pTempBuf = NULL;
    char *pBuf = NULL;

    for (i = start_blk; i < end_blk; i++)
        usedBlkSize += flash_get_sector_size((unsigned short) i);

    if ((pTempBuf = (char *) KMALLOC(usedBlkSize, sizeof(long))) == NULL)
    {
        printf("failed to allocate memory with size: %d\n", usedBlkSize);
        return pTempBuf;
    }
    
    pBuf = pTempBuf;
    for (i = start_blk; i < end_blk; i++)
    {
        sect_size = flash_get_sector_size((unsigned short) i);
#if defined(DEBUG_FLASH)
        printf("getShareBlks: i=%d, sect_size=%d, end_blk=%d\n", i, sect_size, end_blk);
#endif
        flash_read_buf((unsigned short)i, 0, pBuf, sect_size);
        pBuf += sect_size;
    }
    
    return pTempBuf;
}



// Set the pTempBuf to flash from start_blk to end_blk
// return:
// 0 -- ok
// -1 -- fail
static int setSharedBlks(int start_blk, int end_blk, char *pTempBuf)
{
    int i = 0;
    int sect_size = 0;
    int sts = 0;
    char *pBuf = pTempBuf;

    for (i = start_blk; i < end_blk; i++)
    {
        sect_size = flash_get_sector_size((unsigned short) i);
        flash_sector_erase_int(i);
        if (flash_write_buf(i, 0, pBuf, sect_size) != sect_size)
        {
            printf("Error writing flash sector %d.", i);
            sts = -1;
            break;
        }
        pBuf += sect_size;
    }

    return sts;
}



/*******************************************************************************
 * NVRAM functions
 *******************************************************************************/

// get nvram data
// return:
//  0 - ok
//  -1 - fail
int kerSysNvRamGet(char *string, int strLen, int offset)
{
    char *pBuf = NULL;

    if (strLen > NVRAM_LENGTH)
        return -1;

    if ((pBuf = getSharedBlks(fInfo.flash_nvram_start_blk,
        (fInfo.flash_nvram_start_blk + fInfo.flash_nvram_number_blk))) == NULL)
        return -1;

    // get string off the memory buffer
    memcpy(string, (pBuf + fInfo.flash_nvram_blk_offset + offset), strLen);

    KFREE(pBuf);

    return 0;
}


// set nvram 
// return:
//  0 - ok
//  -1 - fail
int kerSysNvRamSet(char *string, int strLen, int offset)
{
    int sts = 0;
    char *pBuf = NULL;

    if (strLen > NVRAM_LENGTH)
        return -1;

    if ((pBuf = getSharedBlks(fInfo.flash_nvram_start_blk,
        (fInfo.flash_nvram_start_blk + fInfo.flash_nvram_number_blk))) == NULL)
        return -1;

    // set string to the memory buffer
    memcpy((pBuf + fInfo.flash_nvram_blk_offset + offset), string, strLen);

    if (setSharedBlks(fInfo.flash_nvram_start_blk, 
        (fInfo.flash_nvram_number_blk + fInfo.flash_nvram_start_blk), pBuf) != 0)
        sts = -1;
    
    KFREE(pBuf);

    return sts;
}


/***********************************************************************
 * Function Name: kerSysEraseNvRam
 * Description  : Erase the NVRAM storage section of flash memory.
 * Returns      : 1 -- ok, 0 -- fail
 ***********************************************************************/
int kerSysEraseNvRam(void)
{
    int sts = 1;
    int memType = BP_MEMORY_8MB_1_CHIP;
    char *tempStorage = KMALLOC(NVRAM_LENGTH, sizeof(long));
    
    // just write the whole buf with '0xff' to the flash
    if (!tempStorage)
        sts = 0;
    else
    {
        memset(tempStorage, 0xff, NVRAM_LENGTH);
        if (kerSysNvRamSet(tempStorage, NVRAM_LENGTH, 0) != 0)
            sts = 0;
        KFREE(tempStorage);
    }

    // set memory type to default 
    kerSysMemoryTypeSet(memType);
    
    return sts;
}


/*******************************************************************************
 * PSI functions
 *******************************************************************************/
#if 0 /* not used */
// get psi data
// return:
//  0 - ok
//  -1 - fail
int kerSysPsiGet(char *string, int strLen, int offset)
{
    char *pBuf = NULL;

    if (strLen > fInfo.flash_persistent_length)
        return -1;

    if ((pBuf = getSharedBlks(fInfo.flash_persistent_start_blk,
        (fInfo.flash_persistent_start_blk + fInfo.flash_persistent_number_blk))) == NULL)
        return -1;

    // get string off the memory buffer
    memcpy(string, (pBuf + fInfo.flash_persistent_blk_offset + offset), strLen);

    KFREE(pBuf);

    return 0;
}
#endif


// set psi 
// return:
//  0 - ok
//  -1 - fail
int kerSysPsiSet(char *string, int strLen, int offset)
{
    int sts = 0;
    char *pBuf = NULL;

    if (strLen > fInfo.flash_persistent_length)
        return -1;

    if ((pBuf = getSharedBlks(fInfo.flash_persistent_start_blk,
        (fInfo.flash_persistent_start_blk + fInfo.flash_persistent_number_blk))) == NULL)
        return -1;

    // set string to the memory buffer
    memcpy((pBuf + fInfo.flash_persistent_blk_offset + offset), string, strLen);

    if (setSharedBlks(fInfo.flash_persistent_start_blk, 
        (fInfo.flash_persistent_number_blk + fInfo.flash_persistent_start_blk), pBuf) != 0)
        sts = -1;
   
    KFREE(pBuf);

    return sts;
}


/***********************************************************************
 * Function Name: kerSysErasePsi
 * Description  : Erase the Psi storage section of flash memory.
 * Returns      : 1 -- ok, 0 -- fail
 ***********************************************************************/
int kerSysErasePsi(int breakIntoCfe)
{
    int sts = 1;
    char *tempStorage = KMALLOC(fInfo.flash_persistent_length, sizeof(long));
    
    // just write the whole buf with '0xff' to the flash
    if (!tempStorage)
        sts = 0;
    else
    {
        memset(tempStorage, 0xff, fInfo.flash_persistent_length);
        if( breakIntoCfe )
            *(unsigned long *) tempStorage = 1;
        if (kerSysPsiSet(tempStorage, fInfo.flash_persistent_length, 0) != 0)
            sts = 0;
        KFREE(tempStorage);
    }

    return sts;
}

/***********************************************************************
 * Function Name: kerSysMemoryTypeSet
 * Description  : set the memory type 4 bytes
 * Returns      : 1 -- ok, 0 -- fail
 ***********************************************************************/
int kerSysMemoryTypeSet(int size)
{
    int sect_size;
    int blk_start;
    char *pTempBuf;

    blk_start = flash_get_blk(FLASH_BASE);
    sect_size = flash_get_sector_size(blk_start);

    if ((pTempBuf = (char *) KMALLOC(sect_size, sizeof(long))) == NULL)
    {
        printf("Failed to allocate memory with size: %d\n", sect_size);
        return 0;
    }
    
    flash_read_buf((unsigned short)blk_start, 0, pTempBuf, sect_size);
    *(unsigned long*)(pTempBuf+SDRAM_TYPE_ADDRESS_OFFSET) = size;
    flash_sector_erase_int(blk_start);     // erase blk before flash

    if (flash_write_buf(blk_start, 0, pTempBuf, sect_size) != sect_size) {
        printf("Failed to flash the memory type\n");
    }
   
    return 1;
}


/***********************************************************************
 * Function Name: kerSysMemoryTypeGet
 * Description  : get the memory type 4 bytes
 * Returns      : memory type
 ***********************************************************************/
int kerSysMemoryTypeGet(void)
{
    int sdramType = BOARD_SDRAM_TYPE;

    return sdramType;
}

/***********************************************************************
 * Function Name: kerSysThreadNumSet
 * Description  : sets CMT thread number
 * Returns      : 1 -- ok, 0 -- fail
 ***********************************************************************/
int kerSysThreadNumSet(int thread)
{
    int sect_size;
    int blk_start;
    char *pTempBuf;

    blk_start = flash_get_blk(FLASH_BASE);
    sect_size = flash_get_sector_size(blk_start);

    if ((pTempBuf = (char *) KMALLOC(sect_size, sizeof(long))) == NULL)
    {
        printf("Failed to allocate memory with size: %d\n", sect_size);
        return 0;
    }
    
    flash_read_buf((unsigned short)blk_start, 0, pTempBuf, sect_size);
    *(unsigned long*)(pTempBuf+THREAD_NUM_ADDRESS_OFFSET) = thread;
    flash_sector_erase_int(blk_start);     // erase blk before flash

    if (flash_write_buf(blk_start, 0, pTempBuf, sect_size) != sect_size) {
        printf("Failed to flash the memory type\n");
    }
   
    return 1;
}


/***********************************************************************
 * Function Name: kerSysThreadNumGet
 * Description  : get CMT Thread Number
 * Returns      : CMT Thread Number
 ***********************************************************************/
int kerSysThreadNumGet(void)
{
    int thread = BOARD_THREAD_NUM;

    return thread;
}

// flash bcm image 
// return: 
// 0 - ok
// !0 - the sector number fail to be flashed (should not be 0)
int kerSysBcmImageSet( int flash_start_addr, char *string, int size, int fWholeImage)
{
    int sts;
    int sect_size;
    int blk_start;
    char *pTempBuf = NULL;
    int savedSize = size;

    blk_start = flash_get_blk(flash_start_addr);
    if( blk_start < 0 )
        return( -1 );

    /* write image to flash memory */
    do 
    {
        sect_size = flash_get_sector_size(blk_start);

#if defined(DEBUG_FLASH)
        printf("Image flasing on block: %d\n", blk_start);
#endif

        // share the blk with nvram only when fWholeImage == 0
        if (!fWholeImage && blk_start == fInfo.flash_persistent_start_blk)  
        {
            if (size > (sect_size - fInfo.flash_persistent_length))
            {
                printf("Image is too big\n");
                break;          // image is too big. Can not overwrite to psi
            }
            if ((pTempBuf = (char *) KMALLOC(sect_size, sizeof(long))) == NULL)
            {
                printf("Failed to allocate memory with size: %d\n", sect_size);
                break;
            }
            flash_read_buf((unsigned short)blk_start, 0, pTempBuf, sect_size);
            memcpy(pTempBuf, string, size);
            flash_sector_erase_int(blk_start);     // erase blk before flash

            if (flash_write_buf(blk_start, 0, pTempBuf, sect_size) == sect_size) 
                size = 0;   // break out and say all is ok
            break;
        }
        
        flash_sector_erase_int(blk_start);     // erase blk before flash

        if (sect_size > size) 
        {
            if (size & 1) 
                size++;
            sect_size = size;
        }
        if (flash_write_buf(blk_start, 0, string, sect_size) != sect_size) {
            break;
        }
    
        printf(".");
        blk_start++;
        string += sect_size;
        size -= sect_size; 
    } while (size > 0);

    if (fWholeImage && savedSize > FLASH_LENGTH_BOOT_ROM)  
    {
        // If flashing a whole image, erase to end of flash.
        int total_blks = flash_get_numsectors();
        while( blk_start < total_blks )
        {
            flash_sector_erase_int(blk_start);
            printf(".");
            blk_start++;
        }
    }

    printf("\n\n");

    if( size == 0 ) 
        sts = 0;  // ok
    else  
        sts = blk_start;    // failed to flash this sector

    return sts;
}

int getBootToCfe(void)
{
    unsigned long btc = 0;
    flash_read_buf(fInfo.flash_persistent_start_blk,
        fInfo.flash_persistent_blk_offset, (unsigned char *)&btc, sizeof(btc));

    return( (btc == 1) ? 1 : 0 );
}

void getMacAddress(unsigned char *hwaddr)
{
    if (hwaddr)
        memcpy(hwaddr, nvramData.ucaBaseMacAddr, NVRAM_MAC_ADDRESS_LEN);
}

unsigned long kerSysReadFromFlash( void *toaddr, unsigned long fromaddr,
    unsigned long len )
{
    int sect = flash_get_blk((int) fromaddr);
    unsigned char *start = flash_get_memptr(sect);
    flash_read_buf( sect, (int) fromaddr - (int) start, toaddr, len );

    return(len);
}

