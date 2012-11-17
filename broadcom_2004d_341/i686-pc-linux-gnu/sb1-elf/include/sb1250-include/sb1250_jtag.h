/*  *********************************************************************
    *  SB1250 Board Support Package
    *  
    *  JTAG Constants and Macros		File: sb1250_jtag.h
    *  
    *  This module contains constants and macros useful for
    *  manipulating the System Control and Debug module on the 1250.
    *  
    *  SB1250 specification level:  User's manual 1/02/02
    *  
    *  Author:  Kip Walker
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

#ifndef _SB1250_JTAG_H
#define _SB1250_JTAG_H

#include "sb1250_defs.h"

#define SB1250_IDCODE_VAL      0x112502a1
#define SIBYTE_IMPCODE_VAL     0x21404001
#define SIBYTE_IMPCODE_VAL_OLD 0x20814001

/*
 * JTAG Memory region
 */

#define K_SCD_JTAG_MEMBASE        0x0010000000
#define K_SCD_JTAG_MEMSIZE        0x0000020000
#define K_SCD_JTAG_MEMTOP         (K_SCD_JTAG_MEMBASE+K_SCD_JTAG_MEMSIZE)


/*
 * JTAG Instruction Register values
 */

#define SIBYTE_EXTEST     0x00
#define SIBYTE_IDCODE     0x01
#define SIBYTE_IMPCODE    0x03
#define SIBYTE_ADDRESS    0x08
#define SIBYTE_DATA       0x09
#define SIBYTE_CONTROL    0x0A
#define SIBYTE_EJTAGALL   0x0B
#define SIBYTE_EJTAGBOOT  0x0C
#define SIBYTE_NORMALBOOT 0x0D
#define SIBYTE_SYSCTRL    0x20
#define SIBYTE_TRACE      0x21
#define SIBYTE_PERF       0x22
#define SIBYTE_TRCTRL     0x23
#define SIBYTE_WAFERID    0x24
#define SIBYTE_PMON       0x25
#define SB1250_CPU0OSC    0x26
#define SB1250_CPU0DSC    0x27
#define SB1250_CPU0TSC    0x28
#define SB1250_CPU1OSC    0x2A
#define SB1250_CPU1DSC    0x2B
#define SB1250_CPU1TSC    0x2C
#define SB1250_SCANIOB0   0x2E
#define SB1250_SCANIOB1   0x30
#define SB1250_SCANL2C    0x32
#define SB1250_SCANMC     0x34
#define SB1250_SCANSCD    0x36
#define SB1250_SCANALL    0x38
#define SB1250_BSRMODE    0x3A
#define SB1250_SCANTRCCNT 0x3B
#define SB1250_CLAMP      0x3C
#define SB1250_SAMPLE     0x3D
#define SB1250_INTEST     0x3E
#define SB1250_BYPASS     0x3F

/* 
 * IDCODE
 */

#define S_JTAG_REVISION              _SB_MAKE32(28)
#define M_JTAG_REVISION              _SB_MAKEMASK(4,S_JTAG_REVISION)
#define V_JTAG_REVISION(x)           _SB_MAKEVALUE(x,S_JTAG_REVISION)
#define G_JTAG_REVISION(x)           _SB_GETVALUE(x,S_JTAG_REVISION,M_JTAG_REVISION)

#define S_JTAG_PARTNUM               _SB_MAKE32(12)
#define M_JTAG_PARTNUM               _SB_MAKEMASK(16,S_JTAG_PARTNUM)
#define V_JTAG_PARTNUM(x)            _SB_MAKEVALUE(x,S_JTAG_PARTNUM)
#define G_JTAG_PARTNUM(x)            _SB_GETVALUE(x,S_JTAG_PARTNUM,M_JTAG_PARTNUM)

/* These _PART_ fields assume PARTNUM has been extracted (and shifted
 * to bit 0).  PART_TYPE matches the SYS_SOC_TYPE from sb1250_scd.h */
#define S_JTAG_PART_TYPE             _SB_MAKE32(0)
#define M_JTAG_PART_TYPE             _SB_MAKEMASK(4,S_JTAG_PART_TYPE)
#define V_JTAG_PART_TYPE(x)          _SB_MAKEVALUE(x,S_JTAG_PART_TYPE)
#define G_JTAG_PART_TYPE(x)          _SB_GETVALUE(x,S_JTAG_PART_TYPE,M_JTAG_PART_TYPE)

#define S_JTAG_PART_L2               _SB_MAKE32(4)
#define M_JTAG_PART_L2               _SB_MAKEMASK(4,S_JTAG_PART_L2)
#define V_JTAG_PART_L2(x)            _SB_MAKEVALUE(x,S_JTAG_PART_L2)
#define G_JTAG_PART_L2(x)            _SB_GETVALUE(x,S_JTAG_PART_L2,M_JTAG_PART_L2)

#define K_JTAG_PART_L2_1024          0
#define K_JTAG_PART_L2_512           5
#define K_JTAG_PART_L2_256           2
#define K_JTAG_PART_L2_128           1

#define S_JTAG_PART_CPUS             _SB_MAKE32(8)
#define M_JTAG_PART_CPUS             _SB_MAKEMASK(4,S_JTAG_PART_CPUS)
#define V_JTAG_PART_CPUS(x)          _SB_MAKEVALUE(x,S_JTAG_PART_CPUS)
#define G_JTAG_PART_CPUS(x)          _SB_GETVALUE(x,S_JTAG_PART_CPUS,M_JTAG_PART_CPUS)

#define S_JTAG_PART_CORE             _SB_MAKE32(12)
#define M_JTAG_PART_CORE             _SB_MAKEMASK(4,S_JTAG_PART_CORE)
#define V_JTAG_PART_CORE(x)          _SB_MAKEVALUE(x,S_JTAG_PART_CORE)
#define G_JTAG_PART_CORE(x)          _SB_GETVALUE(x,S_JTAG_PART_CORE,M_JTAG_PART_CORE)

#define K_JTAG_PART_CORE_SB1         1


/*
 * EJTAG Control Register (Table 15-14)
 */

#define S_JTAG_1250_CR_DM0             0
#define M_JTAG_1250_CR_DM0             _SB_MAKEMASK1(0)
#define M_JTAG_1250_CR_DM1             _SB_MAKEMASK1(1)
#define S_JTAG_1250_CR_EJTAGBreak0     2
#define M_JTAG_1250_CR_EJTAGBreak0     _SB_MAKEMASK1(2)
#define M_JTAG_1250_CR_EJTAGBreak1     _SB_MAKEMASK1(3)
#define S_JTAG_1250_CR_PrTrap0         4
#define M_JTAG_1250_CR_PrTrap0         _SB_MAKEMASK1(4)
#define M_JTAG_1250_CR_PrTrap1         _SB_MAKEMASK1(5)
#define S_JTAG_1250_CR_ProbEn          6
#define M_JTAG_1250_CR_ProbEn          _SB_MAKEMASK1(6)
#define M_JTAG_1250_CR_PrAcc           _SB_MAKEMASK1(7)
#define M_JTAG_1250_CR_PW              _SB_MAKEMASK1(8)
#define M_JTAG_1250_CR_PbAcc           _SB_MAKEMASK1(9)
#define M_JTAG_1250_CR_MaSl            _SB_MAKEMASK1(10)
#define M_JTAG_1250_CR_ClkStopped      _SB_MAKEMASK1(11)

#define G_JTAG_1250_CR_EJTAGBreak(cpu) (M_JTAG_1250_CR_EJTAGBreak0 << (cpu))
#define G_JTAG_1250_CR_PrTrap(cpu)     (M_JTAG_1250_CR_PrTrap0 << (cpu))

#define M_SYSCFG_1250_BIG_ENDIAN       _SB_MAKEMASK1(22)

#define S_SYSCFG_1250_CPU_RESET_0      54
#define M_SYSCFG_1250_CPU_RESET_0      _SB_MAKEMASK1(54)
#define M_SYSCFG_1250_CPU_RESET_1      _SB_MAKEMASK1(55)

#define M_SYSCFG_1250_CPU_RESET(n)     (M_SYSCFG_1250_CPU_RESET_0 << (n))

/*
 * System Config "extension" bits 104:64 (Table 15-5)
 */

#define M_SYSCFG_1250_CLKSTOP          _SB_MAKEMASK1(32)
#define M_SYSCFG_1250_CLKSTEP          _SB_MAKEMASK1(33)
				       
#define S_SYSCFG_1250_CLKCOUNT         34
#define M_SYSCFG_1250_CLKCOUNT         _SB_MAKEMASK(8,S_SYSCFG_1250_CLKCOUNT)
#define V_SYSCFG_1250_CLKCOUNT(x)      _SB_MAKEVALUE(x,S_SYSCFG_1250_CLKCOUNT)
#define G_SYSCFG_1250_CLKCOUNT(x)      _SB_GETVALUE(x,S_SYSCFG_1250_CLKCOUNT,M_SYSCFG_1250_CLKCOUNT)
				       
#define M_SYSCFG_1250_PLL_BYPASS       _SB_MAKEMASK1(42)

#define M_SYSCFG_1250_SB_SOFTRES       _SB_MAKEMASK1(58)
#define M_SYSCFG_1250_SYSTEM_RESET     _SB_MAKEMASK1(60)
				       
/* These masks are in the second double-word */
#define S_SYSCFG_1250_PLLPHASE         0
#define M_SYSCFG_1250_PLLPHASE         _SB_MAKEMASK(2,S_SYSCFG_1250_PLLPHASE)
#define V_SYSCFG_1250_PLLPHASE(x)      _SB_MAKEVALUE(x,S_SYSCFG_1250_PLLPHASE)
#define G_SYSCFG_1250_PLLPHASE(x)      _SB_GETVALUE(x,S_SYSCFG_1250_PLLPHASE,M_SYSCFG_1250_PLLPHASE)
				       
#define K_SYSCFG_1250_PLLPHASE_A       1
#define K_SYSCFG_1250_PLLPHASE_B       3
				       
#define V_SYSCFG_1250_PLLPHASE_A       V_SYSCFG_1250_PLLPHASE(K_SYSCFG_1250_PLLPHASE_A)
#define V_SYSCFG_1250_PLLPHASE_B       V_SYSCFG_1250_PLLPHASE(K_SYSCFG_1250_PLLPHASE_B)
				       
#define S_SYSCFG_1250_PLLCOUNT         2
#define M_SYSCFG_1250_PLLCOUNT         _SB_MAKEMASK(30,S_SYSCFG_1250_PLLCOUNT)
#define V_SYSCFG_1250_PLLCOUNT(x)      _SB_MAKEVALUE(x,S_SYSCFG_1250_PLLCOUNT)
#define G_SYSCFG_1250_PLLCOUNT(x)      _SB_GETVALUE(x,S_SYSCFG_1250_PLLCOUNT,M_SYSCFG_1250_PLLCOUNT)
				       
#define M_SYSCFG_1250_PLLSTOP          _SB_MAKEMASK1(32)
#define M_SYSCFG_1250_STOPSTRETCH      _SB_MAKEMASK1(33)
#define M_SYSCFG_1250_STARTCOND        _SB_MAKEMASK1(34)
#define M_SYSCFG_1250_STOPPING         _SB_MAKEMASK1(35)
#define M_SYSCFG_1250_STOPSTRDONE      _SB_MAKEMASK1(36)
#define M_SYSCFG_1250_SERZB_ARD        _SB_MAKEMASK1(37)
#define M_SYSCFG_1250_SERZB_AR         _SB_MAKEMASK1(38)
				       
#define S_SYSCFG_1250_STRETCHMODE      39
#define M_SYSCFG_1250_STRETCHMODE      _SB_MAKEMASK(2,S_SYSCFG_1250_STRETCHMODE)
#define V_SYSCFG_1250_STRETCHMODE(x)   _SB_MAKEVALUE(x,S_SYSCFG_1250_STRETCHMODE)
#define G_SYSCFG_1250_STRETCHMODE(x)   _SB_GETVALUE(x,S_SYSCFG_1250_STRETCHMODE,M_SYSCFG_1250_STRETCHMODE)

/*
 * ZBbus definitions
 */

#define K_ZB_CMD_READ_SHD	0
#define K_ZB_CMD_READ_EXC	1
#define K_ZB_CMD_WRITE		2
#define K_ZB_CMD_WRITEINV	3
#define K_ZB_CMD_INV		4
#define K_ZB_CMD_NOP		7

#define K_ZB_L1CA_CNCOH		0
#define K_ZB_L1CA_CCOH		1
#define K_ZB_L1CA_UNC		2
#define K_ZB_L1CA_UNC1		3

#define K_ZB_L2CA_NOALLOC	0
#define K_ZB_L2CA_ALLOC		1

#define K_ZB_DMOD_CLEAN		0
#define K_ZB_DMOD_DIRTY		1

#define K_ZB_DCODE_NOP		0
#define K_ZB_DCODE_VLD		1
#define K_ZB_DCODE_VLD_TCORR	2
#define K_ZB_DCODE_VLD_DCORR	3
#define K_ZB_DCODE_BUSERR	4
#define K_ZB_DCODE_FATAL_BUSERR	5
#define K_ZB_DCODE_TAG_UNCORR	6
#define K_ZB_DCODE_DATA_UNCORR	7

#endif
