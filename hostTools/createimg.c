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
/***************************************************************************
 * File Name  : createimg.c
 *
 * Description: This program creates a complete flash image for a BCM963xx
 *              board from an input file that is created by the
 *              bcmImageBuilder utility.
 *
 * Build      : $ gcc -o createimg createimg.c boardparms.c
 *
 * Example    : createimg -b 96348GW2 -n 4 -m 02:10:18:18:12:01 -f 4
 *              -i bcm96348GW_cfe_fs_kernel -o bcm96348GW_cfe_fs_kernel.img 
 *              where:
 *              -b = board id
 *              -n = number of mac address
 *              -m = base mac address
 *              -s = sdram size in MB
 *              -i = input file name
 *              -o = output file name
 *
 * Updates    : 06/05/2002  lat.  Created.
 *              08/18/2002  seanl.  Move the image tag to the front of the kernel (was in nvram).
 *              12/08/2002  seanl.  Add support for 2MG 96345R and 4MG 96345GW flash image
 *
 ***************************************************************************/

/* Includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
typedef unsigned char byte;
typedef unsigned int UINT32;
#define  BCMTAG_EXE_USE
#include "bcmTag.h"
#include "boardparms.h"
#define MAX_MAC_STR_LEN     19         // mac address string 18+1 in regular format - 02:18:10:11:aa:bb 

#include "board.h"


static struct option longopts[] = 
{
    { "boardid",    1, 0, 'b'},
    { "numbermac",  1, 0, 'n'},
    { "macaddr",    1, 0, 'm'},
    { "sdramsize",  1, 0, 's'},
    { "inputfile",  1, 0, 'i'},
    { "outputfile", 1, 0, 'o'},
    { "help",       0, 0, 'h'},
    { "version",    0, 0, 'v'},
    { 0, 0, 0, 0 }
};

static char version[] = "Broadcom Flash Image Creator version 0.3";

static char usage[] = 
"Usage:\t%s [-Ibhvnmpsfio] [--help] [--version]\n"
"\texample:\n"
"\tcreateimg -b 96348GW2 -n 12 -m 02:10:18:18:12:01 -f 4 -e y "
"-i cfe_fs_kernel -o cfe_fs_kernel.img\n"
"\twhere:\n"
"\t[-b] board id name\n"
"\t[-n] number of mac address\n"
"\t[-m] base mac address\n"
"\t[-s] memory size in MB\n"
"\t[-i] input image name\n"
"\t[-o] ouput image name\n";

static char *progname;

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

/***************************************************************************
// Function Name: parsexdigit
// Description  : parse hex digit
// Parameters   : input char
// Returns      : hex int
****************************************************************************/
static int parsexdigit(char str)
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

/***************************************************************************
// Function Name: parsehwaddr
// Description  : convert in = "255.255.255.0" to fffffff00
// Parameters   : str = "255.255.255.0" ; 
// Return       : if not -1,  hwaddr = fffffff00
****************************************************************************/
int parsehwaddr(unsigned char *str, unsigned char *hwaddr)
{
    int digit1,digit2;
    int idx = 6;

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

/***************************************************************************
 * Function Name: fillInOutputBuffer
 * Description  : Fills the output buffer with the flash image contents.
 * Returns      : len of output buffer - OK, -1 - failed.
 ***************************************************************************/
int fillInOutputBuffer(int numOfMac, unsigned char *ucaBaseMac,
    int sdramType, char *szBoardId, unsigned long ulOutputSize, char *pszIn,
    char *pszOut)
{
    int nLen;
    PFILE_TAG pTag = (PFILE_TAG) pszIn;
    unsigned long ulCfeOffset = 0;
    unsigned long ulFsOffset = 0;
    unsigned long ulKernelOffset = 0;
    unsigned long ulCfeLen = 0;
    unsigned long ulFsLen = 0;
    unsigned long ulKernelLen = 0;
    NVRAM_DATA nvramData;  
    UINT32 crc = CRC32_INIT_VALUE; 
    unsigned long spaceLeft = 0;
    unsigned long ulCmtThread = 0;

    ulCfeOffset = strtoul(pTag->cfeAddress, NULL, 10) - IMAGE_BASE;
    ulFsOffset = strtoul(pTag->rootfsAddress, NULL, 10) - IMAGE_BASE;           
    ulKernelOffset = strtoul(pTag->kernelAddress, NULL, 10) - IMAGE_BASE;
    ulCfeLen = strtoul(pTag->cfeLen, NULL, 10);
    ulFsLen = strtoul(pTag->rootfsLen, NULL, 10);
    ulKernelLen = strtoul(pTag->kernelLen, NULL, 10);
    
    printf("\tImage components offsets\n");
    printf("\tcfe offset              : 0x%8.8lx    -- Length: %d\n", 
        ulCfeOffset + IMAGE_BASE, ulCfeLen);
    printf("\tfile tag offset         : 0x%8.8lx    -- Length: %d\n", 
        ulFsOffset - TAG_LEN + IMAGE_BASE, TAG_LEN);
    printf("\trootfs offset           : 0x%8.8lx    -- Length: %d\n", 
        ulFsOffset + IMAGE_BASE, ulFsLen);
    printf("\tkernel offset           : 0x%8.8lx    -- Length: %d\n\n",
        ulKernelOffset + IMAGE_BASE, ulKernelLen);
    
    nLen = ulKernelOffset + ulKernelLen + FLASH_RESERVED_AT_END;

    if( nLen > (int) ulOutputSize )
    {
        printf( "\nERROR: The image size is greater than the output buffer "
            "allocated by this program.\n" );
        return( -1 );
    }

    printf( "\tThe size of the entire flash image is %d bytes.\n", nLen );
    if( nLen < 1 * 1024 * 1024 )
        printf( "\tA 1 MB or greater flash part is needed.\n\n" );
    else if( nLen < 2 * 1024 * 1024 )
        printf( "\tA 2 MB or greater flash part is needed.\n\n" );
    else if( nLen < 4 * 1024 * 1024 )
        printf( "\tA 4 MB or greater flash part is needed.\n\n" );
    else if( nLen < 8 * 1024 * 1024 )
        printf( "\tA 8 MB or greater flash part is needed.\n\n" );

    printf( "\tThe flash space remaining for a 2 MB flash part: %ld bytes.\n", 2*ONEK*ONEK - nLen);
    printf( "\tThe flash space remaining for a 4 MB flash part: %ld bytes.\n\n", 4*ONEK*ONEK - nLen);

    memcpy( pszOut + ulCfeOffset, pszIn + TAG_LEN, ulCfeLen ); 
    // the tag is before fs now...    
    memcpy( pszOut + ulFsOffset - TAG_LEN, pszIn, TAG_LEN );
    memcpy( pszOut + ulFsOffset, pszIn + TAG_LEN + ulCfeLen, ulFsLen ); 
    memcpy( pszOut + ulKernelOffset, pszIn + TAG_LEN + ulCfeLen + ulFsLen, ulKernelLen ); 

    // add board thread number in the CFE space
    BpGetCMTThread(&ulCmtThread);
    memcpy(pszOut + ulCfeOffset + THREAD_NUM_ADDRESS_OFFSET, (char*)&ulCmtThread, sizeof(int));

    // add memory size type in the CFE space
    sdramType = htonl(sdramType);
    memcpy(pszOut + ulCfeOffset + SDRAM_TYPE_ADDRESS_OFFSET, (char*)&sdramType, sizeof(int));

    // create nvram data. No default bootline.  CFE will create one on the first boot
    memset((char *)&nvramData, 0, sizeof(NVRAM_DATA));
    nvramData.ulVersion = htonl(NVRAM_VERSION_NUMBER);
    strncpy(nvramData.szBoardId, szBoardId, NVRAM_BOARD_ID_STRING_LEN);
    nvramData.ulNumMacAddrs = htonl(numOfMac);
    memcpy(nvramData.ucaBaseMacAddr, ucaBaseMac, NVRAM_MAC_ADDRESS_LEN);
    nvramData.ulCheckSum = 0;
    crc = getCrc32((char *)&nvramData, (UINT32) sizeof(NVRAM_DATA), crc);      
    nvramData.ulCheckSum = htonl(crc);
 
    memcpy(pszOut + NVRAM_DATA_OFFSET, (char *)&nvramData, sizeof(NVRAM_DATA));

    return nLen;

} /*fillInOutputBuffer */


/***************************************************************************
 * Function Name: verifyTag
 * Description  : check on the input file by tag verification
 * Returns      : 0 - ok, -1 bad input image file
 ***************************************************************************/
int verifyTag(PFILE_TAG pTag)
{
    UINT32 crc;
    int status = 0;

    // check tag validate token first
    crc = CRC32_INIT_VALUE;
    crc = getCrc32((char*) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    crc = htonl(crc);

    if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
    {
        printf("Illegal image ! Tag crc failed.\n");
        status = -1;
    }

    return status;
}

/***************************************************************************
 * Function Name: createImage
 * Description  : Creates the flash image file.
 * Returns      : None.
 ***************************************************************************/
void createImage(int numOfMac, unsigned char *ucaBaseMac,
    int sdramType, char *szBoardId, char *inputFile,
    char *outputFile)
{
    FILE *hInput = NULL;
    struct stat StatBuf;
    char *pszIn = NULL, *pszOut = NULL;
    unsigned long ulOutputSize = 4 * 1024 * 1024;

    if (stat(inputFile, &StatBuf ) == 0 && (hInput = fopen(inputFile, "rb" )) == NULL)
    {
        printf( "createimg: Error opening input file %s.\n\n", inputFile);
        return;
    }

    /* Allocate buffers to read the entire input file and to write the
     * entire flash.
     */
    pszIn = (char *) malloc(StatBuf.st_size);
    pszOut = (char *) malloc(ulOutputSize);
    if (!pszIn || !pszOut)
    {
        printf( "Memory allocation error, in=0x%8.8lx, out=0x%8.8lx.\n\n",
            (unsigned long) pszIn, (unsigned long) pszOut );
        fclose( hInput );
        return;
    }

    /* Read the input file into memory. */
    if (fread( pszIn, sizeof(char), StatBuf.st_size, hInput ) == StatBuf.st_size)
    {
        /* Verify that FILE_TAG fields are OK. */
        if( verifyTag((PFILE_TAG)pszIn) == 0)
        {
            FILE *hOutput;
            int nLen;

            /* Fill in the output buffer with the flash image. */
            memset( (unsigned char *) pszOut, 0xff, ulOutputSize );
            if ((nLen = fillInOutputBuffer(numOfMac, ucaBaseMac,
                 sdramType, szBoardId, ulOutputSize, pszIn, pszOut)) > 0)
            {
                /* Open output file. */
                if ((hOutput = fopen(outputFile, "w+" )) != NULL)
                {
                    /* Write the output buffer to the output file. */
                    if (fwrite(pszOut, sizeof(char), nLen, hOutput) == nLen)
                        printf( "\t%s flash image file is\n\tsuccessfully created.\n\n", outputFile);
                    else
                        printf( "createimg: Error writing to output file.\n\n" );
                    fclose( hOutput );
                }
                else
                    printf ("createimg: Error opening output file %s.\n\n", outputFile);
            }
        }
    }

    if( pszIn )
        free( pszIn );

    if( pszOut )
        free( pszOut );
    
    fclose( hInput );

} /* createImage */



/*************************************************************
 * Function Name: main
 * Description  : Program entry point that parses command line parameters
 *                and calls a function to create the image.
 * Returns      : 0
 ***************************************************************************/
int main (int argc, char **argv)
{
    int optc;
    int option_index = 0;
    int h = 0, v = 0, lose = 0;
    char inputFile[256], outputFile[256], macString[256];
    char szBoardId[BP_BOARD_ID_LEN] = "";
    int numOfMac = 0, sdramSize = 0, enetExtPhy = 0;
    unsigned long sdramType = 0;
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];

    /* Parse command line options. */
    progname = argv[0];
    inputFile[0] = outputFile[0] = macString[0] = '\0';
  
    while ((optc = getopt_long (argc, argv, "hvp:n:m:s:b:i:o:", longopts, &option_index)) != EOF) 
    {
        switch (optc)
        {
            case 'v':
                v = 1;
                break;
            case 'h':
                h = 1;
                break;
            case 'n':
                numOfMac = strtoul(optarg, NULL, 10);
                break;
            case 'm':
                strcpy(macString, optarg);
                break;
            case 's':
                sdramSize = strtoul(optarg, NULL, 10);
                break;
            case 'i':
                strcpy(inputFile, optarg);
                break;
            case 'o':
                strcpy(outputFile, optarg);
                break;
            case 'b':
                strcpy(szBoardId, optarg);
                break;
            default:
                lose = 1;
                break;
         }
    }
    if (v) 
    {
        /* Print version number.  */
        fprintf (stderr, "%s\n", version);
        if (!h)
            exit(0);
    }
  
    if (h) 
    {
        /* Print help info and exit.  */
        fprintf(stderr, "%s\n", version);
        fprintf(stderr, usage, progname);
        exit(0);
    }

    if (lose || optind < argc || argc < 2 || 
        inputFile[0] == '\0' || outputFile[0] == '\0' ||
        (parsehwaddr(macString, ucaBaseMacAddr) == -1))
    {
        fprintf (stderr, usage, progname);
        exit(1);
    }

    if (sdramSize == 0 && szBoardId[0] != '\0')
    {
        BpSetBoardId( szBoardId );
        if( BpGetSdramSize( (unsigned long *) &sdramType ) == BP_SUCCESS )
        {
            switch (sdramType)
            {
            case BP_MEMORY_8MB_1_CHIP:
                sdramSize = 8;
                break;
            case BP_MEMORY_16MB_1_CHIP:
            case BP_MEMORY_16MB_2_CHIP:
                sdramSize = 16;
                break;
            case BP_MEMORY_32MB_1_CHIP:
            case BP_MEMORY_32MB_2_CHIP:
                sdramSize = 32;
                break;
            case BP_MEMORY_64MB_2_CHIP:
                sdramSize = 64;
                break;
            }
        }
    }

    printf("createimg: Creating image with the following inputs:\n" );
    printf("\tBoard id                : %s\n", szBoardId);
    printf("\tNumber of Mac Addresses : %d\n", numOfMac);
    printf("\tBase Mac Address:       : %s\n", macString);
    printf("\tMemory size             : %dMb\n", sdramSize);
    printf("\tInput File Name         : %s\n", inputFile);
    printf("\tOutput File Name        : %s\n\n", outputFile);
        
    createImage(numOfMac, ucaBaseMacAddr, sdramType, szBoardId,
        inputFile, outputFile);

    return(0);

} /* main */

