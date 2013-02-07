/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  bcm63xx utility functions
    *  
    *  Created on :  04/18/2002  seanl
    *
    *********************************************************************

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
    
#define  BCMTAG_EXE_USE
#include "bcm63xx_util.h"
#include "flash_api.h"

extern int readNvramData(void);
extern int cmd_ifconfig(ui_cmdline_t *cmd);

int parsexdigit(char str);
int changeBootLine(void);
unsigned long inet_addr( char *ip_addr_str );
int checkChipId(char *strTagChipId, char *sig2);
static void UpdateImageSequenceNumber( unsigned char *imageSequence );
static void getBootLine(int bootCt);

BOOT_INFO bootInfo;
NVRAM_DATA nvramData;   

static int parseFilename(char *fn)
{
    if (strlen(fn) < BOOT_FILENAME_LEN)
        return 0;
    else
        return 1;
}

static int parseChoiceFh(char *choice)
{

    if (*choice == 'f' || *choice == 'h')
        return 0;
    else
        return 1;
}


static int parseBootPartition(char *choice)
{
    return( (*choice == BOOT_LATEST_IMAGE || *choice == BOOT_PREVIOUS_IMAGE)
        ? 0 : 1 );
}

static int parseChoice09(char *choice)
{
    int bChoice = *choice - '0';

    if (bChoice >= 0 && bChoice <= 9)
        return 0;
    else
        return 1;
}

static int parseIpAddr(char *ipStr);
static int parseGwIpAddr(char *ipStr);

#define PARAM_IDX_BOARD_IPADDR              0
#define PARAM_IDX_HOST_IPADDR               1
#define PARAM_IDX_GW_IPADDR                 2
#define PARAM_IDX_RUN_FROM                  3
#define PARAM_IDX_RUN_FILENAME              4
#define PARAM_IDX_FLASH_FILENAME            5
#define PARAM_IDX_BOOT_DELAY                6
#define PARAM_IDX_BOOT_IMAGE                7

static PARAMETER_SETTING gBootParam[] =
{
    // prompt name                  Error Prompt    Boot Define Boot Param  Validation function
    {"Board IP address                  :", IP_PROMPT       , "e=",
        "192.168.1.1", 28, parseIpAddr, TRUE}, // index 0
    {"Host IP address                   :", IP_PROMPT       , "h=",
        "192.168.1.100", 15, parseIpAddr, TRUE}, // index 1
    {"Gateway IP address                :", IP_PROMPT       , "g=",
        "", 15, parseGwIpAddr, TRUE}, // index 2
    {"Run from flash/host (f/h)         :", RUN_FROM_PROMPT , "r=",
        "f", 1, parseChoiceFh, TRUE}, // index 3
    {"Default host run file name        :", HOST_FN_PROMPT  , "f=",
        "vmlinux", MAX_PROMPT_LEN - 1, parseFilename, TRUE}, // index 4
    {"Default host flash file name      :", FLASH_FN_PROMPT , "i=",
        "bcm963xx_fs_kernel", MAX_PROMPT_LEN - 1, parseFilename, TRUE}, // index 5
    {"Boot delay (0-9 seconds)          :", BOOT_DELAY_PROMPT,"d=",
        "1", 1, parseChoice09, TRUE}, // index 6
    {"Boot image (0=latest, 1=previous) :", BOOT_PARTITION_PROMPT,"p=",
        {BOOT_LATEST_IMAGE, '\0'}, 1, parseBootPartition, TRUE}, // index 7
    {NULL},
};

static int gNumBootParams = (sizeof(gBootParam) / sizeof(PARAMETER_SETTING))-1;

// move from lib_misc.c
int parseipaddr(const char *ipaddr,uint8_t *dest)
{
    int a,b,c,d;
    char *x;

    /* make sure it's all digits and dots. */
    x = (char *) ipaddr;
    while (*x) {
	if ((*x == '.') || ((*x >= '0') && (*x <= '9'))) {
	    x++;
	    continue;
	    }
	return -1;
	}

    x = (char *) ipaddr;
    a = lib_atoi(ipaddr);
    x = lib_strchr(x,'.');
    if (!x) return -1;
    b = lib_atoi(x+1);
    x = lib_strchr(x+1,'.');
    if (!x) return -1;
    c = lib_atoi(x+1);
    x = lib_strchr(x+1,'.');
    if (!x) return -1;
    d = lib_atoi(x+1);

    if ((a < 0) || (a > 255)) return -1;
    if ((b < 0) || (b > 255)) return -1;
    if ((c < 0) || (c > 255)) return -1;
    if ((d < 0) || (d > 255)) return -1;

    dest[0] = (uint8_t) a;
    dest[1] = (uint8_t) b;
    dest[2] = (uint8_t) c;
    dest[3] = (uint8_t) d;

    return 0;
}

#if 0
static const char hextable[16] = "0123456789ABCDEF";
void dumpHex(unsigned char *start, int len)
{
    unsigned char *ptr = start,
    *end = start + len;

    while (ptr < end)
    {
        long offset = ptr - start;
        unsigned char text[120],
        *p = text;
        while (ptr < end && p < &text[16 * 3])
        {
            *p++ = hextable[*ptr >> 4];
            *p++ = hextable[*ptr++ & 0xF];
            *p++ = ' ';
        }
        p[-1] = 0;
        printf("%4lX %s\n", offset, text);
    }
}

#endif

int parsexdigit(char str)
{
    int digit;

    if ((str >= '0') && (str <= '9')) 
        digit = str - '0';
    else if ((str >= 'a') && (str <= 'f')) 
        digit = str - 'a' + 10;
    else if ((str >= 'A') && (str <= 'F')) 
        digit = str - 'A' + 10;
    else 
        return -1;

    return digit;
}


// convert in = fffffff00 to out=255.255.255.0
// return 0 = OK, 1 failed.
static int convertMaskStr(char *in, char *out)
{
    int i;
    char twoHex[2];
    uint8_t dest[4];
    char mask[BOOT_IP_LEN];

    if (strlen(in) != MASK_LEN)      // mask len has to 8
        return 1;

    memset(twoHex, 0, 2);
    for (i = 0; i < 4; i++)
    {
        twoHex[0] = (uint8_t)*in++;
        twoHex[1] = (uint8_t)*in++;
        if (parsexdigit(*twoHex) == -1)
            return 1;
        dest[i] = (uint8_t) xtoi(twoHex);
    }
    sprintf(mask, "%d.%d.%d.%d", dest[0], dest[1], dest[2], dest[3]);
    strcpy(out, mask);
    return 0;    
}

// return 0 - OK, !0 - Bad ip
static int parseIpAddr(char *ipStr)
{
    char *x;
    uint8_t dest[4];
    char mask[BOOT_IP_LEN];       
    char ipMaskStr[2*BOOT_IP_LEN];

    strcpy(ipMaskStr, ipStr);

    x = strchr(ipMaskStr,':');
    if (!x)                     // no mask
        return parseipaddr(ipMaskStr, dest);

    *x = '\0';

    if (parseipaddr(ipMaskStr, dest))        // ipStr is not ok
        return 1;

    x++;
    return convertMaskStr(x, mask);      // mask is not used here

}

// return 0 - OK, !0 - Bad ip
static int parseGwIpAddr(char *ipStr)
{
    int ret = 0;
    if( *ipStr )
        ret = parseIpAddr(ipStr);
    return(ret);
}


int parsehwaddr(unsigned char *str,uint8_t *hwaddr)
{
    int digit1,digit2;
    int idx = 6;
    
    if (strlen(str) == (MAX_MAC_STR_LEN - 7)) {       // no ':' mac input format ie. 021800100801
        while (*str && (idx > 0)) {
	        digit1 = parsexdigit(*str);
	        if (digit1 < 0)
                return -1;
	        str++;
	        if (!*str)
                return -1;
	        digit2 = parsexdigit(*str);
	        if (digit2 < 0)
                return -1;
	        *hwaddr++ = (digit1 << 4) | digit2;
	        idx--;
            str++;
	    }
        return 0;
    }

    if (strlen(str) != MAX_MAC_STR_LEN-2)
        return -1;
    if (*(str+2) != ':' || *(str+5) != ':' || *(str+8) != ':' || *(str+11) != ':' || *(str+14) != ':')
        return -1;
    
    while (*str && (idx > 0)) {
	    digit1 = parsexdigit(*str);
	    if (digit1 < 0)
            return -1;
	    str++;
	    if (!*str)
            return -1;

	    if (*str == ':') {
	        digit2 = digit1;
	        digit1 = 0;
	    }
	    else {
	        digit2 = parsexdigit(*str);
	        if (digit2 < 0)
                return -1;
            str++;
	    }

	    *hwaddr++ = (digit1 << 4) | digit2;
	    idx--;

	    if (*str == ':') 
            str++;
	}
    return 0;
}


int parseMacAddr(char * macStr)
{
    unsigned char tmpBuf[MAX_PROMPT_LEN];
    
    return (parsehwaddr(macStr, tmpBuf));
}


int parseBoardIdStr(char *boardIdStr)
{
    int boardId = atoi(boardIdStr);
    
    if (boardId >= 0 && boardId < getNumberBoardIdNames())
        return 0;
    else
        return 1;
}


int parseMacAddrCount(char *ctStr)
{
    int count = atoi(ctStr);

    if (count >= 1 && count <= NVRAM_MAC_COUNT_MAX)
        return 0;
    else
        return 1;
}

//**************************************************************************
// Function Name: macNumToStr
// Description  : convert MAC address from array of 6 bytes to string.
//                Ex: 0a0b0c0d0e0f -> 0a:0b:0c:0d:0e:0f
// Returns      : status.
//**************************************************************************
int macNumToStr(unsigned char *macAddr, unsigned char *str)
{
   if (macAddr == NULL || str == NULL) 
       return ERROR;

   sprintf(str, "%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",
           macAddr[0], macAddr[1], macAddr[2], 
           macAddr[3], macAddr[4], macAddr[5]);
   return OK;
}

// port from ifconfig command in ui_netcmds.c
void enet_init(void)
{
    char devname[5] = "eth0";
    uint8_t ipaddr[IP_ADDR_LEN];
    uint8_t netmask[IP_ADDR_LEN];
    uint8_t gateway[IP_ADDR_LEN];
    uint8_t hwaddr[6];
    int res;
    char buf[80];
    char *x;
    /*
     * If the network is running and the device name is
     * different, uninit the net first.
     */

    x = (char *) net_getparam(NET_DEVNAME);

    if ((x != NULL) && (strcmp(x,devname) != 0)) 
	    net_uninit();

    /*
     * Okay, initialize the network if it is not already on.  If it
     * is OFF, the "net_devname" parameter will be NULL.
     */

    if (x == NULL) 
    {
	    res = net_init(devname);		/* turn interface on */
	    if (res < 0) 
        {
	        ui_showerror(res,"Could not activate network interface '%s'",devname);
	        return;
	    }
	}

    memcpy(hwaddr, nvramData.ucaBaseMacAddr, ETH_ALEN);
    net_setparam(NET_HWADDR,hwaddr);

    if (strlen(bootInfo.boardIp) > 0)
        strcpy(buf, bootInfo.boardIp);
    else
        strcpy(buf, DEFAULT_BOARD_IP);      // provide a default t ip
    parseipaddr(buf, ipaddr);
    net_setparam(NET_IPADDR,ipaddr);
    
    if (strlen(bootInfo.boardMask) > 0)
        strcpy(buf, bootInfo.boardMask);
    else
        strcpy(buf, DEFAULT_MASK);
    parseipaddr(buf, netmask);
    net_setparam(NET_NETMASK,netmask);

    if (strlen(bootInfo.gatewayIp) > 0)     // optional
    {
        strcpy(buf, " -gw=");
        strcpy(buf, bootInfo.gatewayIp);
        parseipaddr(buf, gateway);
        net_setparam(NET_GATEWAY,gateway);
    }

    net_setnetvars();

}

/***************************************************************************
// Function Name: getCrc32
// Description  : caculate the CRC 32 of the given data.
// Parameters   : pdata - array of data.
//                size - number of input data bytes.
//                crc - either CRC32_INIT_VALUE or previous return value.
// Returns      : crc.
****************************************************************************/
UINT32 getCrc32(byte *pdata, UINT32 size, UINT32 crc) 
{
    while (size-- > 0)
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];

    return crc;
}


// return 0, ok. return -1 = wrong chip
int checkChipId(char *strTagChipId, char *sig2)
{
    int tagChipId = 0;
	unsigned int chipId = (PERF->RevID & 0xFFFF0000) >> 16;
    int result = 0;

    if (memcmp(sig2, "Firmware", 8) == 0)
        return result;      // skip the pre 2_14L02 release, where the signiture_2 is "Firmware version 1.0"
	
    tagChipId = atoi(strTagChipId);

	switch (chipId) 
	{
		case 0x6338:
			if (tagChipId != 6338)
                result = -1;
            break;
		case 0x6348:
			if (tagChipId != 6348)
                result = -1;
            break;
		case 0x6358:
			if (tagChipId != 6358)
                result = -1;
            break;
        default:
			printf("Chip id %04x not supported.\n", chipId);
            result = -1;
            break;
	}

    if (result == -1)
	    printf("Chip Id error.  Image Chip Id = %d, Board Chip Id = %04x.\n", tagChipId, chipId);

    return result;
}

// return -1: fail.
//         0: OK.
int verifyTag( PFILE_TAG pTag, int verbose )
{
    UINT32 crc;
    FLASH_ADDR_INFO info;
    int tagVer, curVer, tagVerLast;

    kerSysFlashAddrInfoGet( &info );

    // only allow same or greater tag versions (up to tagVerLast - 26) to be uploaded
    tagVer = atoi(pTag->tagVersion);
    curVer = atoi(BCM_TAG_VER);
    tagVerLast = atoi(BCM_TAG_VER_LAST);

    if (tagVer < curVer || tagVer > tagVerLast)
    {
        if( verbose )
        {
            printf("Firmware tag version [%d] is not compatible with the current Tag version [%d].\n", \
                tagVer, curVer);
        }
        return -1;
    }

    if (checkChipId(pTag->chipId, pTag->signiture_2) != 0)
        return -1;

    // check tag validate token first
    crc = CRC32_INIT_VALUE;
    crc = getCrc32((byte *) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);      

    if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
    {
        if( verbose )
            printf("Illegal image ! Tag crc failed.\n");
        return -1;
    }
    return 0;
}

PFILE_TAG getTagFromPartition(int imageNumber)
{
    static unsigned char sectAddr1[sizeof(FILE_TAG)];
    static unsigned char sectAddr2[sizeof(FILE_TAG)];
    int blk = 0;
    UINT32 crc;
    PFILE_TAG pTag = NULL;
    unsigned char *pBase = flash_get_memptr(0);
    unsigned char *pSectAddr = NULL;

    /* The image tag for the first image is always after the boot loader.
     * The image tag for the second image, if it exists, is at one half
     * of the flash size.
     */
    if( imageNumber == 1 )
    {
        blk = flash_get_blk((int) (pBase + FLASH_LENGTH_BOOT_ROM));
        pSectAddr = sectAddr1;
    }
    else
        if( imageNumber == 2 )
        {
            blk = flash_get_blk((int) (pBase + (flash_get_total_size() / 2)));
            pSectAddr = sectAddr2;
        }

    if( blk )
    {
        memset(pSectAddr, 0x00, sizeof(FILE_TAG));
        flash_read_buf((unsigned short) blk, 0, pSectAddr, sizeof(FILE_TAG));
        crc = CRC32_INIT_VALUE;
        crc = getCrc32(pSectAddr, (UINT32)TAG_LEN-TOKEN_LEN, crc);      
        pTag = (PFILE_TAG) pSectAddr;
        if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
            pTag = NULL;
    }

    return( pTag );
}

int getPartitionFromTag( PFILE_TAG pTag )
{
    int ret = 0;

    if( pTag )
    {
        PFILE_TAG pTag1 = getTagFromPartition(1);
        PFILE_TAG pTag2 = getTagFromPartition(2);
        int sequence = atoi(pTag->imageSequence);
        int sequence1 = (pTag1) ? atoi(pTag1->imageSequence) : -1;
        int sequence2 = (pTag2) ? atoi(pTag2->imageSequence) : -1;

        if( pTag1 && sequence == sequence1 )
            ret = 1;
        else
            if( pTag2 && sequence == sequence2 )
                ret = 2;
    }

    return( ret );
}

PFILE_TAG getBootImageTag(void)
{
    PFILE_TAG pTag = NULL;
    PFILE_TAG pTag1 = getTagFromPartition(1);
    PFILE_TAG pTag2 = getTagFromPartition(2);

    if( pTag1 && pTag2 )
    {
        /* Two images are flashed. */
        int sequence1 = atoi(pTag1->imageSequence);
        int sequence2 = atoi(pTag2->imageSequence);

        getBootLine(gNumBootParams);
        convertBootInfo();

        if( bootInfo.bootPartition == BOOT_LATEST_IMAGE )
            pTag = (sequence2 > sequence1) ? pTag2 : pTag1;
        else /* Boot from the previous image. */
            pTag = (sequence2 < sequence1) ? pTag2 : pTag1;
    }
    else
        /* One image is flashed. */
        pTag = (pTag2) ? pTag2 : pTag1;

    return( pTag );
}

static void UpdateImageSequenceNumber( unsigned char *imageSequence )
{
    int newImageSequence = 0;
    PFILE_TAG pTag = getTagFromPartition(1);

    if( pTag )
        newImageSequence = atoi(pTag->imageSequence);

    pTag = getTagFromPartition(2);
    if( pTag && atoi(pTag->imageSequence) > newImageSequence )
        newImageSequence = atoi(pTag->imageSequence);

    newImageSequence++;
    sprintf(imageSequence, "%d", newImageSequence);
}

// return -1: fail.
//         0: OK.
int flashImage(uint8_t *imagePtr)
{
    UINT32 crc;
    FLASH_ADDR_INFO info;
    int totalImageSize = 0;
    int cfeSize;
    int cfeAddr, rootfsAddr, kernelAddr;
    int status = 0;
    PFILE_TAG pTag = (PFILE_TAG) imagePtr;

    if( verifyTag( pTag, 1 ) == -1 )
        return -1;

    kerSysFlashAddrInfoGet( &info );

    // check imageCrc
    totalImageSize = atoi(pTag->totalImageLen);
    crc = CRC32_INIT_VALUE;
    crc = getCrc32((imagePtr+TAG_LEN), (UINT32) totalImageSize, crc);      

    if (crc != (UINT32) (*(UINT32*)(pTag->imageValidationToken)))
    {
        printf(" Illegal image ! Image crc failed.\n");
        return -1;
    }
      
    cfeSize = cfeAddr = rootfsAddr = kernelAddr = 0;

    // check cfe's existence
    cfeAddr = atoi(pTag->cfeAddress);
    if (cfeAddr)
    {
        NVRAM_DATA saveNvramData;   
        cfeSize = atoi(pTag->cfeLen);
        if( (cfeSize <= 0) )
        {
            printf("Illegal cfe size [%d].\n", cfeSize );
            return -1;
        }
        
        // save NVRAM data into a local structure
        memcpy( &saveNvramData, &nvramData, sizeof(NVRAM_DATA) );

        // save the memory type
        BpGetSdramSize( (unsigned long *)
            (imagePtr+TAG_LEN+SDRAM_TYPE_ADDRESS_OFFSET) );

        // save the thread number
        BpGetCMTThread( (unsigned long *)
            (imagePtr+TAG_LEN+THREAD_NUM_ADDRESS_OFFSET) );

        printf("\nFlashing CFE: ");
        if ((status = kerSysBcmImageSet(cfeAddr+BOOT_OFFSET, imagePtr+TAG_LEN,
            cfeSize, 0)) != 0)
        {
            printf("Failed to flash CFE. Error: %d\n", status);
            return status;
        }

        // if nvram is not valid, restore the current nvram settings.
        if( readNvramData() && *(unsigned long *) &nvramData == NVRAM_DATA_ID )
        {
            unsigned long memType, threadNum;
        
            memcpy( &nvramData, &saveNvramData, sizeof(NVRAM_DATA) );
            writeNvramData();

            BpGetSdramSize( &memType );
            if( memType != kerSysMemoryTypeGet() )
                kerSysMemoryTypeSet(memType);

            BpGetCMTThread( &threadNum );
            if( threadNum != kerSysThreadNumGet() )
                kerSysThreadNumSet(threadNum);
        }
    }

    // check root filesystem and kernel existence
    rootfsAddr = atoi(pTag->rootfsAddress);
    kernelAddr = atoi(pTag->kernelAddress);

    if( rootfsAddr )
        rootfsAddr += BOOT_OFFSET;

    if( kernelAddr )
        kernelAddr += BOOT_OFFSET;
    
    if( rootfsAddr && kernelAddr )
    {
        char *p;
        char *tagFs = imagePtr;
        unsigned int baseAddr = (unsigned int) flash_get_memptr(0);
        unsigned int totalSize = (unsigned int) flash_get_total_size();
        unsigned int availableSizeOneImg = totalSize -
            ((unsigned int) rootfsAddr - baseAddr) - FLASH_RESERVED_AT_END;
        unsigned int newImgSize = atoi(pTag->rootfsLen)+atoi(pTag->kernelLen);
        PFILE_TAG pCurTag = getBootImageTag();
        UINT32 crc = CRC32_INIT_VALUE;
        unsigned int curImgSize = 0;

        if( pCurTag )
            curImgSize = atoi(pCurTag->rootfsLen) + atoi(pCurTag->kernelLen);

        if( newImgSize > availableSizeOneImg)
        {
            printf("Illegal image size %d.  Image size must not be greater "
                "than %d.\n", newImgSize, availableSizeOneImg);
            return -1;
        }

        // tag is alway at the sector start of fs
        if (cfeAddr)
        {
            // will trash cfe memory, but cfe is already flashed
            tagFs = imagePtr + cfeSize;
            memcpy(tagFs, imagePtr, TAG_LEN);
        }

        UpdateImageSequenceNumber( ((PFILE_TAG) tagFs)->imageSequence );
        crc = getCrc32((unsigned char *)tagFs, (UINT32)TAG_LEN-TOKEN_LEN, crc);      
        *(unsigned long *) &((PFILE_TAG) tagFs)->tagValidationToken[0] = crc;

        printf("\nFlashing root file system and kernel at 0x%8.8lx: ",
            rootfsAddr - TAG_LEN);
        if( (status = kerSysBcmImageSet((rootfsAddr-TAG_LEN), tagFs,
            TAG_LEN + newImgSize, 0)) != 0 )
        {
            printf("Failed to flash root file system. Error: %d\n", status);
            return status;
        }

        for( p = nvramData.szBootline; p[2] != '\0'; p++ )
            if( p[0] == 'p' && p[1] == '=' && p[2] != BOOT_LATEST_IMAGE )
            {
                // Change boot partition to boot from new image.
                p[2] = BOOT_LATEST_IMAGE;
                writeNvramData();
                break;
            }
    }

    printf(".\n*** Image flash done *** !\n");

    return status;
}


// return -1: fail.
//         0: OK.
int writeWholeImage(uint8_t *imagePtr, int wholeImageSize)
{
    UINT32 crc;
    int status = 0;
    int imageSize = wholeImageSize - TOKEN_LEN;
    unsigned char crcBuf[CRC_LEN];
    NVRAM_DATA saveNvramData;   

    // if whole image size (plus TOKEN_LEN of crc) is greater than total flash size, return error
    if (wholeImageSize > (flash_get_total_size() + TOKEN_LEN))
    {
        printf("Image size too big\n");
        return -1;
    }

    // check tag validate token first
    crc = CRC32_INIT_VALUE;
    crc = getCrc32(imagePtr, (UINT32)imageSize, crc);      
    memcpy(crcBuf, imagePtr+imageSize, CRC_LEN);
    if (memcmp(&crc, crcBuf, CRC_LEN) != 0)
    {
        printf("Illegal whole flash image\n");
        return -1;
    }

    // save NVRAM data into a local structure
    memcpy( &saveNvramData, &nvramData, sizeof(NVRAM_DATA) );

    if ((status = kerSysBcmImageSet(FLASH_BASE, imagePtr, imageSize, 1)) != 0)
    {
        printf("Failed to flash whole image. Error: %d\n", status);
        return status;
    }

    // if nvram is not valid, restore the current nvram settings
    if( readNvramData() && *(unsigned long *) &nvramData == NVRAM_DATA_ID )
    {
        unsigned long memType, threadNum;
        
        memcpy( &nvramData, &saveNvramData, sizeof(NVRAM_DATA) );
        writeNvramData();

        BpGetSdramSize( &memType );
        if( memType != kerSysMemoryTypeGet() )
            kerSysMemoryTypeSet(memType);

        BpGetCMTThread( &threadNum );
        if( threadNum != kerSysThreadNumGet() )
            kerSysThreadNumSet(threadNum);
    }

    return status;
}



int processPrompt(PPARAMETER_SETTING promptPtr, int promptCt)
{
    unsigned char tmpBuf[MAX_PROMPT_LEN];
    int i = 0;
    int bChange = FALSE;

    printf("Press:  <enter> to use current value\r\n");
    printf("        '-' to go previous parameter\r\n");
    printf("        '.' to clear the current value\r\n");
    printf("        'x' to exit this command\r\n");

	memset(tmpBuf, 0, MAX_PROMPT_LEN);
    while (i < promptCt)
    {
        if( (promptPtr+i)->enabled == FALSE )
        {
            if( tmpBuf[0] == '-' )
            {
                if( i > 0 )
                {
                    i--;
                    continue;
                }
            }
            else
            {
                i++;
                continue;
            }
        }

        if (strlen((promptPtr+i)->parameter) > 0)
            printf("%s  %s  ", (promptPtr+i)->promptName, (promptPtr+i)->parameter);
        else
            printf("%s  %s", (promptPtr+i)->promptName, (promptPtr+i)->parameter);

	    memset(tmpBuf, 0, MAX_PROMPT_LEN);
	    console_readline ("", tmpBuf, (promptPtr+i)->maxValueLength + 1);

        switch (tmpBuf[0])
        {
            case '-':          // go back one parameter
                if (i > 0)
                    i--;
                break;
            case 'x':          // get out the b command
                if ((promptPtr+i)->func != 0)  // validate function is supplied, do a check
                {
                    if ((promptPtr+i)->func((promptPtr+i)->parameter))
                    {
                        printf("\n%s;  Try again!\n", (promptPtr+i)->errorPrompt);
                        break;
                    }
                }
                i = promptCt;
                break;
            case '.':          // clear the current parameter and advance
                if ((promptPtr+i)->func != 0)  // validate function is supplied, do a check
                {
                    if ((promptPtr+i)->func(""))
                    {
                        printf("\n%s;  Try again!\n", (promptPtr+i)->errorPrompt);
                        break;
                    }
                }
                memset((promptPtr+i)->parameter, 0, MAX_PROMPT_LEN);
                i++;
                bChange = TRUE;
                break;
            case 0:            // no input; use default if it is OK
                if ((promptPtr+i)->func != 0)  // validate function is supplied, do a check
                {
                    if ((promptPtr+i)->func((promptPtr+i)->parameter))
                    {
                        printf("\n%s;  Try again!\n", (promptPtr+i)->errorPrompt);
                        break;
                    }
                }
                i++;
                break;
            default:            // new parameter
                if ((promptPtr+i)->func != 0)  // validate function is supplied, do a check
                {
                    if ((promptPtr+i)->func(tmpBuf))
                    {
                        printf("\n%s;  Try again!\n", (promptPtr+i)->errorPrompt);
                        break;
                    }
                }
                memset((promptPtr+i)->parameter, 0, MAX_PROMPT_LEN);
                memcpy((promptPtr+i)->parameter, tmpBuf, strlen(tmpBuf));
                i++;
                bChange = TRUE;
        }
    }

    return bChange;

} // processPrompt

// write the nvramData struct to nvram after CRC is calculated 
void writeNvramData(void)
{
    UINT32 crc = CRC32_INIT_VALUE;
    
    nvramData.ulCheckSum = 0;
    crc = getCrc32((char *)&nvramData, (UINT32) sizeof(NVRAM_DATA), crc);      
    nvramData.ulCheckSum = crc;
    kerSysNvRamSet((char *)&nvramData, sizeof(NVRAM_DATA), NVRAM_VERSION_NUMBER_ADDRESS);
}

// read the nvramData struct from nvram 
// return -1:  crc fail, 0 ok
int readNvramData(void)
{
    UINT32 crc = CRC32_INIT_VALUE, savedCrc;
    
    kerSysNvRamGet((char *)&nvramData, sizeof(NVRAM_DATA), NVRAM_VERSION_NUMBER_ADDRESS);
    savedCrc = nvramData.ulCheckSum;
    nvramData.ulCheckSum = 0;
    crc = getCrc32((char *)&nvramData, (UINT32) sizeof(NVRAM_DATA), crc);      
    if (savedCrc != crc)
    {
        memset(&nvramData.ucaBaseMacAddr, 0xff, sizeof(nvramData.ucaBaseMacAddr));
        nvramData.ulNumMacAddrs = 0xffffffff;
        return -1;
    }
    
    return 0;
}

void convertBootInfo(void)
{
    char *x;

    memset(&bootInfo, 0, sizeof(BOOT_INFO));
    strcpy(bootInfo.boardIp, gBootParam[PARAM_IDX_BOARD_IPADDR].parameter);

    if ((x = strchr(bootInfo.boardIp, ':')))        // has mask
    {
        *x = '\0';
        convertMaskStr((x+1), bootInfo.boardMask);
    }
    strcpy(bootInfo.hostIp, gBootParam[PARAM_IDX_HOST_IPADDR].parameter);
    if ((x = strchr(bootInfo.hostIp, ':')))        // ignore host mask
        *x = '\0';
    strcpy(bootInfo.gatewayIp, gBootParam[PARAM_IDX_GW_IPADDR].parameter);
    if ((x = strchr(bootInfo.gatewayIp, ':')))        // ignore gw mask
        *x = '\0';
    bootInfo.runFrom = gBootParam[PARAM_IDX_RUN_FROM].parameter[0];
    strcpy(bootInfo.hostFileName, gBootParam[PARAM_IDX_RUN_FILENAME].parameter);
    strcpy(bootInfo.flashFileName,
        gBootParam[PARAM_IDX_FLASH_FILENAME].parameter);
    bootInfo.bootDelay = (int)
        (gBootParam[PARAM_IDX_BOOT_DELAY].parameter[0] - '0');
    bootInfo.bootPartition = gBootParam[PARAM_IDX_BOOT_IMAGE].parameter[0];
}

static void getBootLine(int bootCt)
{
    int i;
    int blLen;
    char *curPtr = NULL;
    char *dPtr = NULL;

    // the struct is filled up in kerSysFlashInit
    if ((nvramData.szBootline[0] == 'b' && nvramData.szBootline[1] == 'c' && nvramData.szBootline[2] == 'm') ||
        (nvramData.szBootline[0] == (char)0xff))
        setDefaultBootline();

    do 
    {
        readNvramData();
        curPtr = nvramData.szBootline;
        blLen = NVRAM_BOOTLINE_LEN;

        for (i = 0; i < bootCt; i++)
        {
            if( curPtr )
		        curPtr = strnchr(curPtr, '=', blLen); 
            if (curPtr) // found '=' and get the param.
            {
                dPtr = strnchr(curPtr, ' ', blLen);   // find param. deliminator ' '
                if (dPtr == NULL)
                {
                    setDefaultBootline();
                    break;  //break for loop and start over again
                }
                memset(gBootParam[i].parameter, 0, MAX_PROMPT_LEN);
                memcpy(gBootParam[i].parameter, curPtr+1, dPtr-curPtr-1);
            }

            // move to next param.
            curPtr = dPtr;  
        } // for loop

    } while (i != bootCt);   // do until the all the parameters are accounted for

}

unsigned long inet_addr( char *ip_addr_str )
{
    unsigned long ret = 0;
    char *p1 = ip_addr_str;
    char *p2;
    int i;

    for( i = 3, p2 = p1; i > 0 && *p1; i--, p2 = p1 )
    {
        while( *p1 && *p1 != '.' )
            p1++;

        if( *p1 == '.' )
        {
            ret |= (unsigned long) atoi( p2 ) << (i *  8);
            p1++;
        }
    }

    if( *p1 ) /* success */
        ret |= (unsigned long) atoi( p2 );
    else /* error */
        ret = 0xffffffff;

    return( ret );
} /* inet_addr */


int bootLineChanged(void)
{
    getBootLine(gNumBootParams);
    convertBootInfo();
    return((inet_addr(bootInfo.boardIp) != inet_addr(DEFAULT_BOARD_IP)) ? 1: 0);
}


int runFromFlash(void)
{
    getBootLine(gNumBootParams);
    convertBootInfo();
    return((bootInfo.runFrom == 'f') ? 1 : 0);
}

// print the bootline and board parameter info and fill in the struct for later use
// 
int printSysInfo(void)
{
    int i;

    if( readNvramData() != 0 )
    {
        printf("*** Board is not initialized properly ***\n\n");
        setBoardParam(); /* does not return */
    }

    while ((BpSetBoardId( nvramData.szBoardId ) != BP_SUCCESS) || 
        (nvramData.ulNumMacAddrs < 1 && nvramData.ulNumMacAddrs > NVRAM_MAC_COUNT_MAX))
        
    {
        printf("*** Board is not initialized properly ***\n\n");
        setBoardParam(); /* does not return */
    }

    if( getTagFromPartition(1) == NULL || getTagFromPartition(2) == NULL )
        gBootParam[PARAM_IDX_BOOT_IMAGE].enabled = FALSE;

    // display the bootline info
    getBootLine(gNumBootParams);

    for (i = 0; i < gNumBootParams; i++)
        if( gBootParam[i].enabled )
            printf("%s %s  \n", gBootParam[i].promptName, gBootParam[i].parameter);

    // print the board param
    displayBoardParam();

    convertBootInfo();

    return 0;
}


//**************************************************************************
// Function Name: changeBootLine
// Description  : Use vxWorks bootrom style parameter input method:
//                Press <enter> to use default, '-' to go to previous parameter  
//                Note: Parameter is set to current value in the menu.
// Returns      : None.
//**************************************************************************
int changeBootLine(void)
{
    int i;
    int bChange = FALSE;
    char boardIpSaved[BOOT_IP_LEN];

    getBootLine(gNumBootParams);
    
    strcpy(boardIpSaved, bootInfo.boardIp);
    bChange = processPrompt(gBootParam, gNumBootParams);

    if (bChange)
    {
        char *blPtr = nvramData.szBootline;
        int paramLen;

        memset(blPtr, 0, NVRAM_BOOTLINE_LEN);
        for (i = 0; i < gNumBootParams; i++)
        {
            memcpy(blPtr, gBootParam[i].promptDefine, PROMPT_DEFINE_LEN);
            blPtr += PROMPT_DEFINE_LEN;
            paramLen = strlen(gBootParam[i].parameter);
            memcpy(blPtr, gBootParam[i].parameter, paramLen);
            blPtr += paramLen;
            if (!(gBootParam[i].parameter[0] == ' '))
            {
                memcpy(blPtr, " ", 1);
                blPtr += 1;
            }
        }
        writeNvramData();
    }

    readNvramData();
    convertBootInfo();

    // if board ip differs, do enet init
    if (strcmp(boardIpSaved, bootInfo.boardIp) != 0)
        enet_init();

    return 0;

}  

