/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  Main Module				File: bcm63xxBoot_main.c       
    *  
    *  This module contains the main "C" routine for CFE bootstrap loader 
    *  and decompressor to decompress the real CFE to ram and jump over.
    *
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  Revised: seanl
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  Broadcom Corporation. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of Broadcom Corporation.  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     Broadcom Corporation.
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */


#include "lib_types.h"
#include "lib_string.h"
#include "lib_malloc.h"
#include "lib_printf.h"

#include "cfe_iocb.h"
#include "cfe_device.h"
#include "cfe_console.h"
#include "cfe_timer.h"

#include "env_subr.h"
#include "ui_command.h"
#include "cfe_mem.h"
#include "cfe.h"

#include "bsp_config.h"
#include "board.h"
#include "bcm_map.h"

#include "exception.h"

#include "segtable.h"

#include "initdata.h"

#if CFG_PCI
#include "pcivar.h"
#endif


/*  *********************************************************************
    *  Constants
    ********************************************************************* */

#ifndef CFG_STACK_SIZE
#define STACK_SIZE	8192
#else
#define STACK_SIZE	((CFG_STACK_SIZE+1023) & ~1023)
#endif

// fake functions for not to modifying init_mips.S
void _exc_entry(void);
void cfe_command_restart(void);
void cfe_doxreq(void);

void _exc_entry(void)
{
}

void cfe_command_restart(void)
{
}
void cfe_doxreq(void)
{
}

/*  *********************************************************************
    *  Externs
    ********************************************************************* */

void cfe_main(int,int);

/*  *********************************************************************
    *  Globals
    ********************************************************************* */

void cfe_ledstr(const char *leds)
{
}

/*  *********************************************************************
    *  cfe_main(a,b)
    *  
    *  It's gotta start somewhere.
    *  Input parameters: 
    *  	   a,b - not used
    *  	   
    *  Return value:
    *  	   does not return
    ********************************************************************* */

extern void _binArrayStart(void);
extern void _binArrayEnd(void);
extern int decompress_lzma_7z(unsigned char* in_data, unsigned in_size, unsigned char* out_data, unsigned out_size);

void cfe_main(int a,int b)
{
    unsigned char *pucSrc;
    unsigned char *pucDst;
    unsigned int *entryPoint;   
    unsigned int binArrayStart = (unsigned int) _binArrayStart;
    unsigned int binArrayEnd = (unsigned int) _binArrayEnd;
    unsigned int dataLen = binArrayEnd - binArrayStart - 4;
    int ret;

    binArrayStart += BOOT_OFFSET;
    binArrayEnd += BOOT_OFFSET;

    KMEMINIT((unsigned char *) (uint32_t) mem_heapstart,
         ((CFG_HEAP_SIZE)*1024));

    entryPoint = (unsigned int*) binArrayStart;
    pucSrc = (unsigned char *) (binArrayStart + 4);

    pucDst = (unsigned char *) *entryPoint;
     ret = decompress_lzma_7z((unsigned char*)pucSrc,
        (unsigned int)dataLen,
        (unsigned char *) pucDst,
        23*1024*1024);

    if (ret != 0) 
        while (1);          // if not decompressed ok, loop for EJTAG

    cfe_launch((unsigned long) pucDst); // never return...
}

