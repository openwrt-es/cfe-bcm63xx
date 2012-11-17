/*
<:copyright-gpl 
 Copyright 2006 Broadcom Corp. All Rights Reserved. 
 
 This program is free software; you can distribute it and/or modify it 
 under the terms of the GNU General Public License (Version 2) as 
 published by the Free Software Foundation. 
 
 This program is distributed in the hope it will be useful, but WITHOUT 
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 for more details. 
 
 You should have received a copy of the GNU General Public License along 
 with this program; if not, write to the Free Software Foundation, Inc., 
 59 Temple Place - Suite 330, Boston MA 02111-1307, USA. 
:>
*/

#ifndef __BCM6338_CPU_H
#define __BCM6338_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

/*
#************************************************************************
#* Coprocessor 0 Register Names
#************************************************************************
*/
#define C0_BCM_CONFIG          $22

/*
# Select 1
#  Bit  31:   unused
#  Bits 30:25 MMU Size (Num TLB entries-1)
#  Bits 24:22 ICache sets/way (2^n * 64)
#  Bits 21:19 ICache Line size (2^(n+1) bytes) 0=No Icache
#  Bits 18:16 ICache Associativity (n+1) way                    
#  Bits 15:13 DCache sets/way (2^n * 64)
#  Bits 12:10 DCache Line size (2^(n+1) bytes) 0=No Dcache
#  Bits 9:7   DCache Associativity (n+1) way                    
#  Bits 6:4   unused
#  Bit  3:    1=At least 1 watch register
#  Bit  2:    1=MIPS16 code compression implemented
#  Bit  1:    1=EJTAG implemented                   
#  Bit  0:    1=FPU implemented                   
*/
#define CP0_CFG_ISMSK      (0x7 << 22)
#define CP0_CFG_ISSHF      22
#define CP0_CFG_ILMSK      (0x7 << 19)
#define CP0_CFG_ILSHF      19
#define CP0_CFG_IAMSK      (0x7 << 16)
#define CP0_CFG_IASHF      16
#define CP0_CFG_DSMSK      (0x7 << 13)
#define CP0_CFG_DSSHF      13
#define CP0_CFG_DLMSK      (0x7 << 10)
#define CP0_CFG_DLSHF      10
#define CP0_CFG_DAMSK      (0x7 << 7)
#define CP0_CFG_DASHF      7

/*
#************************************************************************
#* Coprocessor 0 Broadcom Config Register Bits
#************************************************************************
*/
#define CP0_CFG_ICSHEN         (0x1 << 31)
#define CP0_CFG_DCSHEN         (0x1 << 30)
#define CP0_CFG_OWBE           (0x1 << 19)

/*
#************************************************************************
#* MIPS Registers
#************************************************************************
*/

#define MIPS_BASE       0xff400000
#define RAC_CR0         0x00
#define RAC_PWR         (1 << 31)
#define RAC_BRR_PF      (1 << 30)
#define RAC_FLH         (1 << 8)
#define RAC_DPF         (1 << 6)
#define RAC_NCH         (1 << 5)
#define RAC_C_INV       (1 << 4)
#define RAC_PF_D        (1 << 3)
#define RAC_PF_I        (1 << 2)
#define RAC_D           (1 << 1)
#define RAC_I           (1 << 0)

#define RAC_CR1         0x04
#define RAC_UPB_SHFT    16
#define RAC_LWB_SHFT    0

#ifdef __cplusplus
}
#endif

#endif
