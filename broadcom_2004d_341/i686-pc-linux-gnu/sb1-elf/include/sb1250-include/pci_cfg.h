/*  *********************************************************************
    *  CFE Generic Board Support Package
    *  
    *  PCI configuration header formats			File: pci_cfg.h
    *  
    *  This module contains constants and macros to describe 
    *  the standard PCI/PCI-X/HyperTransport configuration header.
    *  PCI specification levels:
    *    PCI Local Bus Specification, Revisions 2.2 and 2.3
    *    PCI-to-PCI Bridge Architecture Specification, Revision 1.1
    *    PCI-X Addendum to the PCI Local Bus Specification, Revision 1.1
    *    HyperTransport I/O Link Specification, Revision 1.05
    *  
    *********************************************************************  
    *
    *  Copyright 2002,2003,2004
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


#ifndef _PCI_CFG_H
#define _PCI_CFG_H

#if !defined(__ASSEMBLER__)
#define _MAKE32(x) ((uint32_t)(x))
#else
#define _MAKE32(x) (x)
#endif

#define _PCI_MAKEMASK1(n)    (_MAKE32(1) << _MAKE32(n))

#define _PCI_MAKEMASK(v,n)   (_MAKE32((_MAKE32(1)<<(v))-1) << _MAKE32(n))
#define _PCI_MAKEVALUE(v,n)  (_MAKE32(v) << _MAKE32(n))
#define _PCI_GETVALUE(v,n,m) ((_MAKE32(v) & _MAKE32(m)) >> _MAKE32(n))


/*
 * The following definitions refer to 32-bit PCI Configuration Space.
 * Bit-field definitions assume that registers are accessed as 32-bit
 * words on natural boundaries, even when unrelated functions are
 * packed into the same 32 bits.
 */

/*
 * PCI Configuration Header Common Registers
 */

#define R_PCI_DEVICEID          0x0000
#define R_PCI_CMDSTATUS         0x0004
#define R_PCI_CLASSREV          0x0008
#define R_PCI_DEVHDR            0x000C

/*
 * PCI Configuration Header, Type 0 (Device)
 */

#define R_PCI_BAR0              0x0010
#define R_PCI_BAR1              0x0014
#define R_PCI_BAR2              0x0018
#define R_PCI_BAR3              0x001C
#define R_PCI_BAR4              0x0020
#define R_PCI_BAR5              0x0024
#define R_PCI_CARDBUSCIS        0x0028
#define R_PCI_SUBSYSID          0x002C
#define R_PCI_ROMBASE           0x0030
#define R_PCI_CAPPTR            0x0034
#define R_PCI_RESERVED1         0x0038
#define R_PCI_INTGRANT          0x003C

/*
 * PCI Configuration Header, Type 1 (Bridge)
 */

#define R_PPB_BAR0              0x0010
#define R_PPB_BAR1              0x0014
#define R_PPB_BUSID             0x0018
#define R_PPB_SECSTATUS         0x001C
#define R_PPB_MEM               0x0020
#define R_PPB_PREFETCH          0x0024
#define R_PPB_PREFBASE          0x0028
#define R_PPB_PREFLIMIT         0x002C
#define R_PPB_IOUPPER           0x0030
#define R_PPB_CAPPTR            0x0034
#define R_PPB_ROMBASE           0x0038
#define R_PPB_BRCTRL            0x003C

/*
 * PCI Device ID register
 */

#define S_PCI_DEVICE_VENDORID        0
#define M_PCI_DEVICE_VENDORID        _PCI_MAKEMASK(16,S_PCI_DEVICE_VENDORID)
#define V_PCI_DEVICE_VENDORID(x)     _PCI_MAKEVALUE(x,S_PCI_DEVICE_VENDORID)
#define G_PCI_DEVICE_VENDORID(x)     _PCI_GETVALUE(x,S_PCI_DEVICE_VENDORID,M_PCI_DEVICE_VENDORID)

#define S_PCI_DEVICE_DEVICEID        16
#define M_PCI_DEVICE_DEVICEID        _PCI_MAKEMASK(16,S_PCI_DEVICE_DEVICEID)
#define V_PCI_DEVICE_DEVICEID(x)     _PCI_MAKEVALUE(x,S_PCI_DEVICE_DEVICEID)
#define G_PCI_DEVICE_DEVICEID(x)     _PCI_GETVALUE(x,S_PCI_DEVICE_DEVICEID,M_PCI_DEVICE_DEVICEID)

/*
 * PCI Command Register
 */

#define M_PCI_CMD_IOSPACE_EN         _PCI_MAKEMASK1(0)
#define M_PCI_CMD_MEMSPACE_EN        _PCI_MAKEMASK1(1)
#define M_PCI_CMD_MASTER_EN          _PCI_MAKEMASK1(2)
#define M_PCI_CMD_SPECCYC_EN         _PCI_MAKEMASK1(3)
#define M_PCI_CMD_MEMWRINV_EN        _PCI_MAKEMASK1(4)
#define M_PCI_CMD_VGAPALSNP_EN       _PCI_MAKEMASK1(5)
#define M_PCI_CMD_PARERRRESP         _PCI_MAKEMASK1(6)
#define M_PCI_CMD_DATAERRRESP        M_PCI_CMD_PARERRRESP  /* HT */
#define M_PCI_CMD_STEPCTRL           _PCI_MAKEMASK1(7)
#define M_PCI_CMD_SERR_EN            _PCI_MAKEMASK1(8)
#define M_PCI_CMD_FASTB2B_EN         _PCI_MAKEMASK1(9)
#define M_PCI_CMD_INTR_DIS           _PCI_MAKEMASK1(10)    /* PCI 2.3 */

/*
 * PCI Status Register.  Note that these constants
 * assume you've read the command and status register together
 * (32-bit read at offset 0x04)
 */

#define M_PCI_STATUS_INTRSTAT        _PCI_MAKEMASK1(19)    /* PCI 2.3 */
#define M_PCI_STATUS_CAPLIST         _PCI_MAKEMASK1(20)
#define M_PCI_STATUS_66MHZCAP        _PCI_MAKEMASK1(21)
#define M_PCI_STATUS_FASTB2BCAP      _PCI_MAKEMASK1(23)
#define M_PCI_STATUS_MSTRDPARERR     _PCI_MAKEMASK1(24)
#define M_PCI_STATUS_MSTRDATAERR     M_PCI_STATUS_MSTRDPARERR /* HT */

#define S_PCI_STATUS_DEVSELTIMING    25
#define M_PCI_STATUS_DEVSELTIMING    _PCI_MAKEMASK(2,S_PCI_STATUS_DEVSELTIMING)
#define V_PCI_STATUS_DEVSELTIMING(x) _PCI_MAKEVALUE(x,S_PCI_STATUS_DEVSELTIMING)
#define G_PCI_STATUS_DEVSELTIMING(x) _PCI_GETVALUE(x,S_PCI_STATUS_DEVSELTIMING,M_PCI_STATUS_DEVSELTIMING)

#define M_PCI_STATUS_SIGDTGTABORT    _PCI_MAKEMASK1(27)
#define M_PCI_STATUS_RCVDTGTABORT    _PCI_MAKEMASK1(28)
#define M_PCI_STATUS_RCVDMSTRABORT   _PCI_MAKEMASK1(29)
#define M_PCI_STATUS_SIGDSERR        _PCI_MAKEMASK1(30)
#define M_PCI_STATUS_DETPARERR       _PCI_MAKEMASK1(31)
#define M_PCI_STATUS_DATAERRDET      M_PCI_STATUS_DETPARERR  /* HT */

/*
 * PCI Class/Revision Register
 */

#define S_PCI_CLASSREV_REVID         0
#define M_PCI_CLASSREV_REVID         _PCI_MAKEMASK(8,S_PCI_CLASSREV_REVID)
#define V_PCI_CLASSREV_REVID(x)      _PCI_MAKEVALUE(x,S_PCI_CLASSREV_REVID)
#define G_PCI_CLASSREV_REVID(x)      _PCI_GETVALUE(x,S_PCI_CLASSREV_REVID,M_PCI_CLASSREV_REVID)

#define S_PCI_CLASSREV_CLASS         8
#define M_PCI_CLASSREV_CLASS         _PCI_MAKEMASK(24,S_PCI_CLASSREV_CLASS)
#define V_PCI_CLASSREV_CLASS(x)      _PCI_MAKEVALUE(x,S_PCI_CLASSREV_CLASS)
#define G_PCI_CLASSREV_CLASS(x)      _PCI_GETVALUE(x,S_PCI_CLASSREV_CLASS,M_PCI_CLASSREV_CLASS)

/*
 * PCI Device Header Register
 */

#define S_PCI_DEVHDR_CLINESZ         0
#define M_PCI_DEVHDR_CLINESZ         _PCI_MAKEMASK(8,S_PCI_DEVHDR_CLINESZ)
#define V_PCI_DEVHDR_CLINESZ(x)      _PCI_MAKEVALUE(x,S_PCI_DEVHDR_CLINESZ)
#define G_PCI_DEVHDR_CLINESZ(x)      _PCI_GETVALUE(x,S_PCI_DEVHDR_CLINESZ,M_PCI_DEVHDR_CLINESZ)

#define S_PCI_DEVHDR_LATTIMER        8
#define M_PCI_DEVHDR_LATTIMER        _PCI_MAKEMASK(8,S_PCI_DEVHDR_LATTIMER)
#define V_PCI_DEVHDR_LATTIMER(x)     _PCI_MAKEVALUE(x,S_PCI_DEVHDR_LATTIMER)
#define G_PCI_DEVHDR_LATTIMER(x)     _PCI_GETVALUE(x,S_PCI_DEVHDR_LATTIMER,M_PCI_DEVHDR_LATTIMER)

#define S_PCI_DEVHDR_HDRTYPE         16
#define M_PCI_DEVHDR_HDRTYPE         _PCI_MAKEMASK(8,S_PCI_DEVHDR_HDRTYPE)
#define V_PCI_DEVHDR_HDRTYPE(x)      _PCI_MAKEVALUE(x,S_PCI_DEVHDR_HDRTYPE)
#define G_PCI_DEVHDR_HDRTYPE(x)      _PCI_GETVALUE(x,S_PCI_DEVHDR_HDRTYPE,M_PCI_DEVHDR_HDRTYPE)

#define K_PCI_DEVHDR_HDRTYPE_TYPE0   0

#define S_PCI_DEVHDR_BIST            24
#define M_PCI_DEVHDR_BIST            _PCI_MAKEMASK(8,S_PCI_DEVHDR_BIST)
#define V_PCI_DEVHDR_BIST(x)         _PCI_MAKEVALUE(x,S_PCI_DEVHDR_BIST)
#define G_PCI_DEVHDR_BIST(x)         _PCI_GETVALUE(x,S_PCI_DEVHDR_BIST,M_PCI_DEVHDR_BIST)


/*
 * PCI Device Interrupt/Grant Register (Type 0)
 */

#define S_PCI_INTGRANT_INTLINE       0
#define M_PCI_INTGRANT_INTLINE       _PCI_MAKEMASK(8,S_PCI_INTGRANT_INTLINE)
#define V_PCI_INTGRANT_INTLINE(x)    _PCI_MAKEVALUE(x,S_PCI_INTGRANT_INTLINE)
#define G_PCI_INTGRANT_INTLINE(x)    _PCI_GETVALUE(x,S_PCI_INTGRANT_INTLINE,M_PCI_INTGRANT_INTLINE)

#define S_PCI_INTGRANT_INTPIN        8
#define M_PCI_INTGRANT_INTPIN        _PCI_MAKEMASK(8,S_PCI_INTGRANT_INTPIN)
#define V_PCI_INTGRANT_INTPIN(x)     _PCI_MAKEVALUE(x,S_PCI_INTGRANT_INTPIN)
#define G_PCI_INTGRANT_INTPIN(x)     _PCI_GETVALUE(x,S_PCI_INTGRANT_INTPIN,M_PCI_INTGRANT_INTPIN)

#define S_PCI_INTGRANT_MINGNT        16
#define M_PCI_INTGRANT_MINGNT        _PCI_MAKEMASK(8,S_PCI_INTGRANT_MINGNT)
#define V_PCI_INTGRANT_MINGNT(x)     _PCI_MAKEVALUE(x,S_PCI_INTGRANT_MINGNT)
#define G_PCI_INTGRANT_MINGNT(x)     _PCI_GETVALUE(x,S_PCI_INTGRANT_MINGNT,M_PCI_INTGRANT_MINGNT)

#define S_PCI_INTGRANT_MAXLAT        24
#define M_PCI_INTGRANT_MAXLAT        _PCI_MAKEMASK(8,S_PCI_INTGRANT_MAXLAT)
#define V_PCI_INTGRANT_MAXLAT(x)     _PCI_MAKEVALUE(x,S_PCI_INTGRANT_MAXLAT)
#define G_PCI_INTGRANT_MAXLAT(x)     _PCI_GETVALUE(x,S_PCI_INTGRANT_MAXLAT,M_PCI_INTGRANT_MAXLAT)


/*
 * Bridge Bus Number/Sec Latency Register
 */

#define S_PPB_BUSID_PRI              0
#define M_PPB_BUSID_PRI              _PCI_MAKEMASK(8,S_PPB_BUSID_PRI)
#define V_PPB_BUSID_PRI(x)           _PCI_MAKEVALUE(x,S_PPB_BUSID_PRI)
#define G_PPB_BUSID_PRI(x)           _PCI_GETVALUE(x,S_PPB_BUSID_PRI,M_PPB_BUSID_PRI)

#define S_PPB_BUSID_SEC              8
#define M_PPB_BUSID_SEC              _PCI_MAKEMASK(8,S_PPB_BUSID_SEC)
#define V_PPB_BUSID_SEC(x)           _PCI_MAKEVALUE(x,S_PPB_BUSID_SEC)
#define G_PPB_BUSID_SEC(x)           _PCI_GETVALUE(x,S_PPB_BUSID_SEC,M_PPB_BUSID_SEC)

#define S_PPB_BUSID_SUBORD           16
#define M_PPB_BUSID_SUBORD           _PCI_MAKEMASK(8,S_PPB_BUSID_SUBORD)
#define V_PPB_BUSID_SUBORD(x)        _PCI_MAKEVALUE(x,S_PPB_BUSID_SUBORD)
#define G_PPB_BUSID_SUBORD(x)        _PCI_GETVALUE(x,S_PPB_BUSID_SUBORD,M_PPB_BUSID_SUBORD)

#define S_PPB_BUSID_SECLATTIMER      24
#define M_PPB_BUSID_SECLATTIMER      _PCI_MAKEMASK(8,S_PPB_BUSID_SECLATTIMER)
#define V_PPB_BUSID_SECLATTIMER(x)   _PCI_MAKEVALUE(x,S_PPB_BUSID_SECLATTIMER)
#define G_PPB_BUSID_SECLATTIMER(x)   _PCI_GETVALUE(x,S_PPB_BUSID_SECLATTIMER,M_PPB_BUSID_SECLATTIMER)

/*
 * Bridge I/O Base/Limit Register
 */

#define S_PPB_IO_BASE                0
#define M_PPB_IO_BASE                _PCI_MAKEMASK(8,S_PPB_IO_BASE)
#define V_PPB_IO_BASE(x)             _PCI_MAKEVALUE(x,S_PPB_IO_BASE)
#define G_PPB_IO_BASE(x)             _PCI_GETVALUE(x,S_PPB_IO_BASE,M_PPB_IO_BASE)

#define S_PPB_IO_LIMIT               8
#define M_PPB_IO_LIMIT               _PCI_MAKEMASK(8,S_PPB_IO_LIMIT)
#define V_PPB_IO_LIMIT(x)            _PCI_MAKEVALUE(x,S_PPB_IO_LIMIT)
#define G_PPB_IO_LIMIT(x)            _PCI_GETVALUE(x,S_PPB_IO_LIMIT,M_PPB_IO_LIMIT)

/*
 * Bridge Secondary Status Register.  Note that these constants
 * assume that the secondary status and i/o base/limit are read together
 * (32-bit read at offset 0x1C)
 */

#define M_PPB_SSTATUS_66MHZCAP       M_PPB_STATUS_66MHZCAP
#define M_PPB_SSTATUS_FASTB2BCAP     M_PPB_SSTATUS_FASTB2BCAP
#define M_PPB_SSTATUS_MSTRDATAERR    M_PPB_SSTATUS_MSTRDATAERR

#define S_PPB_SSTATUS_DEVSELTIMING   S_PPB_STATUS_DEVSELTIMING
#define M_PPB_SSTATUS_DEVSELTIMING   M_PPB_STATUS_DEVSELTIMING
#define V_PPB_SSTATUS_DEVSELTIMING(x) V_PPB_STATUS_DEVSELTIMING(x)
#define G_PPB_SSTATUS_DEVSELTIMING(x) G_PPB_STATUS_DEVSELTIMING(x)

#define M_PPB_SSTATUS_SIGDTGTABORT   M_PPB_STATUS_SIGDTGTABORT
#define M_PPB_SSTATUS_RCVDTGTABORT   M_PPB_STATUS_RCVDTGTABORT
#define M_PPB_SSTATUS_RCVDMSTRABORT  M_PPB_STATUS_RCVDMSTRABORT
#define M_PPB_SSTATUS_DETSERR        _PCI_MAKEMASK1(30)
#define M_PPB_SSTATUS_DATAERRDET     M_PPB_STATUS_DATAERRDET

/*
 * Bridge Interrupt Line and Pin Register
 */

#define S_PPB_INT_LINE               0
#define M_PPB_INT_LINE               _PCI_MAKEMASK(8,S_PPB_INT_LINE)
#define V_PPB_INT_LINE(x)            _PCI_MAKEVALUE(x,S_PPB_INT_LINE)
#define G_PPB_INT_LINE(x)            _PCI_GETVALUE(x,S_PPB_INT_LINE,M_PPB_INT_LINE)

#define S_PPB_INT_PIN                8
#define M_PPB_INT_PIN                _PCI_MAKEMASK(8,S_PPB_INT_PIN)
#define V_PPB_INT_PIN(x)             _PCI_MAKEVALUE(x,S_PPB_INT_PIN)
#define G_PPB_INT_PIN(x)             _PCI_GETVALUE(x,S_PPB_INT_PIN,M_PPB_INT_PIN)

/*
 * Bridge Control Register.  Note that these 
 * constants assume that the bridge control and interrupt registers
 * are read together (32-bit read at offset 0x3C)
 */

#define M_PPB_BRCTL_DATAERRRESP      _PCI_MAKEMASK1(16)
#define M_PPB_BRCTL_SERR_EN          _PCI_MAKEMASK1(17)
#define M_PPB_BRCTL_ISA_EN           _PCI_MAKEMASK1(18)
#define M_PPB_BRCTL_VGA_EN           _PCI_MAKEMASK1(19)
#define M_PPB_BRCTL_MSTRABORTMODE    _PCI_MAKEMASK1(21)
#define M_PPB_BRCTL_SECBUSRESET      _PCI_MAKEMASK1(22)
#define M_PPB_BRCTL_FASTB2B_EN       _PCI_MAKEMASK1(23)
#define M_PPB_BRCTL_PRIDISCARD       _PCI_MAKEMASK1(24)
#define M_PPB_BRCTL_SECDISCARD       _PCI_MAKEMASK1(25)
#define M_PPB_BRCTL_DISCARDSTAT      _PCI_MAKEMASK1(26)
#define M_PPB_BRCTL_DISCARDSERR_EN   _PCI_MAKEMASK1(27)


/*
 * PCI Capability IDs.
 * For each capability, byte 0 contains the ID and byte 1 is the base
 *  of the next capability (0 if none).
 */

#define PCI_CAP_PM                   0x01    /* Power Management */
#define PCI_CAP_AGP                  0x02    /* Accelerated Graphics Port */
#define PCI_CAP_VPD                  0x03    /* Vital Product Data */
#define PCI_CAP_SI                   0x04    /* Slot Identification */
#define PCI_CAP_MSI                  0x05    /* Message Signaled Interrupts */
#define PCI_CAP_HS                   0x06    /* cPCI Hot Swap */
#define PCI_CAP_PCIX                 0x07    /* PCI-X */
#define PCI_CAP_HT                   0x08    /* HyperTransport */


/*
 * Power Management Capability
 */

/* NYI */


/*
 * Message-Signaled Interrupt (MSI) Capability.
 * Note: For 32-bit message addresses, MSIDATA is at offset 0x08.
 */

#define R_PCI_MSICTRL           (0x00)
#define R_PCI_MSIADDR           (0x04)  /* lower, upper */
#define R_PCI_MSIDATA           (0x0C)

#define M_MSI_CTRL_MSI_EN       _PCI_MAKEMASK1(16)

#define S_MSI_CTRL_MULTCAP      17
#define M_MSI_CTRL_MULTCAP      _PCI_MAKEMASK(3,S_MSI_CTRL_MULTCAP)
#define V_MSI_CTRL_MULTCAP(x)   _PCI_MAKEVALUE(x,S_MSI_CTRL_MULTCAP)
#define G_MSI_CTRL_MULTCAP(x)   _PCI_GETVALUE(x,S_MSI_CTRL_MULTCAP,M_MSI_CTRL_MULTCAP)

#define S_MSI_CTRL_MULT_EN      20
#define M_MSI_CTRL_MULT_EN      _PCI_MAKEMASK(3,S_MSI_CTRL_MULT_EN)
#define V_MSI_CTRL_MULT_EN(x)   _PCI_MAKEVALUE(x,S_MSI_CTRL_MULT_EN)
#define G_MSI_CTRL_MULT_EN(x)   _PCI_GETVALUE(x,S_MSI_CTRL_MULT_EN,M_MSI_CTRL_MULT_EN)

#define M_MSI_CTRL_64BITCAP     _PCI_MAKEMASK1(23)

#define S_MSI_DATA_MSG          0
#define M_MSI_DATA_MSG          _PCI_MAKEMASK(16,S_MSI_DATA_MSG)
#define V_MSI_DATA_MSG(x)       _PCI_MAKEVALUE(x,S_MSI_DATA_MSG)
#define G_MSI_DATA_MSG(x)       _PCI_GETVALUE(x,S_MSI_DATA_MSG,M_MSI_DATA_MSG)


/*
 * PCI-X Capability
 */

#define R_PCI_PCIXCMD           (0x00)
#define R_PCI_PCIXSTAT          (0x04)

#define M_PCIX_CMD_DPERRREC_EN  _PCI_MAKEMASK1(0)
#define M_PCIX_CMD_RELAXORDER   _PCI_MAKEMASK1(1)

#define S_PCIX_CMD_MAXRD        2
#define M_PCIX_CMD_MAXRD        _PCI_MAKEMASK(2,S_PCIX_CMD_MAXRD)
#define V_PCIX_CMD_MAXRD(x)     _PCI_MAKEVALUE(x,S_PCIX_CMD_MAXRD)
#define G_PCIX_CMD_MAXRD(x)     _PCI_GETVALUE(x,S_PCIX_CMD_MAXRD,M_PCIX_CMD_MAXRD)

#define S_PCIX_CMD_MAXSPLIT     4
#define M_PCIX_CMD_MAXSPLIT     _PCI_MAKEMASK(3,S_PCIX_CMD_MAXSPLIT)
#define V_PCIX_CMD_MAXSPLIT(x)  _PCI_MAKEVALUE(x,S_PCIX_CMD_MAXSPLIT)
#define G_PCIX_CMD_MAXSPLIT(x)  _PCI_GETVALUE(x,S_PCIX_CMD_MAXSPLIT,M_PCIX_CMD_MAXSPLIT)


#define S_PCIX_STAT_FN          0
#define M_PCIX_STAT_FN          _PCI_MAKEMASK(3,S_PCIX_STAT_FN)
#define V_PCIX_STAT_FN(x)       _PCI_MAKEVALUE(x,S_PCIX_STAT_FN)
#define G_PCIX_STAT_FN(x)       _PCI_GETVALUE(x,S_PCIX_STAT_FN,M_PCIX_STAT_FN)

#define S_PCIX_STAT_DEV         3
#define M_PCIX_STAT_DEV         _PCI_MAKEMASK(5,S_PCIX_STAT_DEV)
#define V_PCIX_STAT_DEV(x)      _PCI_MAKEVALUE(x,S_PCIX_STAT_DEV)
#define G_PCIX_STAT_DEV(x)      _PCI_GETVALUE(x,S_PCIX_STAT_DEV,M_PCIX_STAT_DEV)

#define S_PCIX_STAT_BUS         8
#define M_PCIX_STAT_BUS         _PCI_MAKEMASK(8,S_PCIX_STAT_BUS)
#define V_PCIX_STAT_BUS(x)      _PCI_MAKEVALUE(x,S_PCIX_STAT_BUS)
#define G_PCIX_STAT_BUS(x)      _PCI_GETVALUE(x,S_PCIX_STAT_BUS,M_PCIX_STAT_BUS)

#define M_PCIX_STAT_64BDEVICE   _PCI_MAKEMASK1(16)
#define M_PCIX_STAT_133MHZCAP   _PCI_MAKEMASK1(17)
#define M_PCIX_STAT_SPLITDISC   _PCI_MAKEMASK1(18)
#define M_PCIX_STAT_UNEXPSPLIT  _PCI_MAKEMASK1(19)
#define M_PCIX_STAT_DEVCOMPLX   _PCI_MAKEMASK1(20)

#define S_PCIX_STAT_MAXRD       21
#define M_PCIX_STAT_MAXRD       _PCI_MAKEMASK(2,S_PCIX_STAT_MAXRD)
#define V_PCIX_STAT_MAXRD(x)    _PCI_MAKEVALUE(x,S_PCIX_STAT_MAXRD)
#define G_PCIX_STAT_MAXRD(x)    _PCI_GETVALUE(x,S_PCIX_STAT_MAXRD,M_PCIX_STAT_MAXRD)

#define S_PCIX_STAT_MAXSPLIT    23
#define M_PCIX_STAT_MAXSPLIT    _PCI_MAKEMASK(3,S_PCIX_STAT_MAXSPLIT)
#define V_PCIX_STAT_MAXSPLIT(x) _PCI_MAKEVALUE(x,S_PCIX_STAT_MAXSPLIT)
#define G_PCIX_STAT_MAXSPLIT(x) _PCI_GETVALUE(x,S_PCIX_STAT_MAXSPLIT,M_PCIX_STAT_MAXSPLIT)

#define S_PCIX_STAT_MAXCUMRD    26
#define M_PCIX_STAT_MAXCUMRD    _PCI_MAKEMASK(3,S_PCIX_STAT_MAXCUMRD)
#define V_PCIX_STAT_MAXCUMRD(x) _PCI_MAKEVALUE(x,S_PCIX_STAT_MAXCUMRD)
#define G_PCIX_STAT_MAXCUMRD(x) _PCI_GETVALUE(x,S_PCIX_STAT_MAXCUMRD,M_PCIX_STAT_MAXCUMRD)

#define M_PCIX_STAT_RCVSPLITERR _PCI_MAKEMASK1(29)

#define M_PCIX_STAT_RC          (M_PCIX_STAT_SPLITDISC | \
                                 M_PCIX_STAT_UNEXPSPLIT | \
                                 M_PCIX_STAT_RCVSPLITERR)


/*
 * HyperTransport Capabilities
 */

/*
 * The capability's Type is encoded in the high-order bits of the
 * first word (Command) and distinguishes among variant capabilities.
 */

#define S_HT_CMD_CAPTYPE             29
#define M_HT_CMD_CAPTYPE             _PCI_MAKEMASK(3,S_HT_CMD_CAPTYPE)
#define V_HT_CMD_CAPTYPE(x)          _PCI_MAKEVALUE(x,S_HT_CMD_CAPTYPE)
#define G_HT_CMD_CAPTYPE(x)          _PCI_GETVALUE(x,S_HT_CMD_CAPTYPE,M_HT_CMD_CAPTYPE)

#define K_HT_CAPTYPE_SLAVE           0x0
#define K_HT_CAPTYPE_HOST            0x1
#define K_HT_CAPTYPE_SWITCH          0x2

/* Switch subtypes */
#define S_HT_CMD_SUBTYPE             27
#define M_HT_CMD_SUBTYPE             _PCI_MAKEMASK(2,S_HT_CMD_SUBTYPE)
#define V_HT_CMD_SUBTYPE(x)          _PCI_MAKEVALUE(x,S_HT_CMD_SUBTYPE)
#define G_HT_CMD_SUBTYPE(x)          _PCI_GETVALUE(x,S_HT_CMD_SUBTYPE,M_HT_CMD_SUBTYPE)

#define K_HT_SUBTYPE_SWITCH          0x0
#define K_HT_SUBTYPE_VCSET           0x3

/*
 * Slave/Primary Interface Capability
 */

#define R_HTP_CMD               (0x00)
#define R_HTP_LINKCTRL0         (0x04)
#define R_HTP_LINKCTRL1         (0x08)
#define R_HTP_LINKFREQ0         (0x0C)
#define R_HTP_LINKFREQ1         (0x10)
#define R_HTP_ERROR             (0x14)
#define R_HTP_MEMUPPER          (0x18)

/*
 * Host/Secondary Interface Capability
 */

#define R_HTS_CMD               (0x00)
#define R_HTS_LINKCTRL          (0x04)
#define R_HTS_LINKFREQ          (0x08)
#define R_HTS_FEATURE           (0x0C)
#define R_HTS_ERROR             (0x10)
#define R_HTS_MEMUPPER          (0x14)


/*
 * HT Command Register.  Note that these constants assume
 * that the HT Command and Capability registers are read together
 * (32-bit read at relative offset 0x00)
 */

/* Primary/Device variant (relative offset 0x00) */

#define S_HT_CMD_BASEUNITID          16
#define M_HT_CMD_BASEUNITID          _PCI_MAKEMASK(5,S_HT_CMD_BASEUNITID)
#define V_HT_CMD_BASEUNITID(x)       _PCI_MAKEVALUE(x,S_HT_CMD_BASEUNITID)
#define G_HT_CMD_BASEUNITID(x)       _PCI_GETVALUE(x,S_HT_CMD_BASEUNITID,M_HT_CMD_BASEUNITID)

#define S_HT_CMD_UNITCOUNT           21
#define M_HT_CMD_UNITCOUNT           _PCI_MAKEMASK(5,S_HT_CMD_UNITCOUNT)
#define V_HT_CMD_UNITCOUNT(x)        _PCI_MAKEVALUE(x,S_HT_CMD_UNITCOUNT)
#define G_HT_CMD_UNITCOUNT(x)        _PCI_GETVALUE(x,S_HT_CMD_UNITCOUNT,M_HT_CMD_UNITCOUNT)

#define M_HT_CMD_MASTERHOST          _PCI_MAKEMASK1(26)
#define M_HT_CMD_DEFAULTDIR          _PCI_MAKEMASK1(27)
#define M_HT_CMD_DROPUNINIT          _PCI_MAKEMASK1(28)

/* Secondary/Host variant (relative offset 0x00) */

#define M_HTS_CMD_WARMRESET          _SB_MAKEMASK1_32(16)
#define M_HTS_CMD_DOUBLEENDED        _SB_MAKEMASK1_32(17)

#define S_HTS_CMD_DEVICENUM          18
#define M_HTS_CMD_DEVICENUM          _SB_MAKEMASK_32(5,S_HTS_CMD_DEVICENUM)
#define V_HTS_CMD_DEVICENUM(x)       _SB_MAKEVALUE_32(x,S_HTS_CMD_DEVICENUM)
#define G_HTS_CMD_DEVICENUM(x)       _SB_GETVALUE_32(x,S_HTS_CMD_DEVICENUM,M_HTS_CMD_DEVICENUM)

#define M_HTS_CMD_CHAINSIDE          _SB_MAKEMASK1_32(23)
#define M_HTS_CMD_HOSTHIDE           _SB_MAKEMASK1_32(24)
#define M_HTS_CMD_ACTASSLAVE         _SB_MAKEMASK1_32(26)
#define M_HTS_CMD_HOSTEOCERR         _SB_MAKEMASK1_32(27)
#define M_HTS_CMD_DROPUNINIT         _SB_MAKEMASK1_32(28)

/*
 * HT Link Control Register
 *    Primary/Device: relative offsets 0x04, 0x08
 *    Secondary/host: relative offset  0x04
 */

#define M_HT_LINKCTRL_CRCSYNCFLOOD_EN _PCI_MAKEMASK1(1)
#define M_HT_LINKCTRL_CRCSTARTTEST   _PCI_MAKEMASK1(2)
#define M_HT_LINKCTRL_CRCFORCEERR    _PCI_MAKEMASK1(3)
#define M_HT_LINKCTRL_LINKFAIL       _PCI_MAKEMASK1(4)
#define M_HT_LINKCTRL_INITCOMPLETE   _PCI_MAKEMASK1(5)
#define M_HT_LINKCTRL_EOC            _PCI_MAKEMASK1(6)
#define M_HT_LINKCTRL_XMITOFF        _PCI_MAKEMASK1(7)

#define S_HT_LINKCTRL_CRCERR         8
#define M_HT_LINKCTRL_CRCERR         _PCI_MAKEMASK(4,S_HT_LINKCTRL_CRCERR)
#define V_HT_LINKCTRL_CRCERR(x)      _PCI_MAKEVALUE(x,S_HT_LINKCTRL_CRCERR)
#define G_HT_LINKCTRL_CRCERR(x)      _PCI_GETVALUE(x,S_HT_LINKCTRL_CRCERR,M_HT_LINKCTRL_CRCERR)

#define M_HT_LINKCTRL_ISOCFC_EN      _PCI_MAKEMASK1(12)
#define M_HT_LINKCTRL_LDTSTOPTRI_EN  _PCI_MAKEMASK1(13)
#define M_HT_LINKCTRL_EXTCTLTIME     _PCI_MAKEMASK1(14)
#define M_HT_LINKCTRL_64BADDR_EN     _PCI_MAKEMASK1(15)

/* 
 * HT Link Configuration Register. Note that these
 * constants assume that the HT Link Configuration and Control
 * registers are read together.
 *    Primary/Device: relative offsets 0x04, 0x08
 *    Secondary/host: relative offset  0x04
 */

#define S_HT_LINKCFG_MAXIN           16
#define M_HT_LINKCFG_MAXIN           _PCI_MAKEMASK(3,S_HT_LINKCFG_MAXIN)
#define V_HT_LINKCFG_MAXIN(x)        _PCI_MAKEVALUE(x,S_HT_LINKCFG_MAXIN)
#define G_HT_LINKCFG_MAXIN(x)        _PCI_GETVALUE(x,S_HT_LINKCFG_MAXIN,M_HT_LINKCFG_MAXIN)

#define M_HT_LINKCFG_DWFCIN          _PCI_MAKEMASK1(19)

#define S_HT_LINKCFG_MAXOUT          20
#define M_HT_LINKCFG_MAXOUT          _PCI_MAKEMASK(3,S_HT_LINKCFG_MAXOUT)
#define V_HT_LINKCFG_MAXOUT(x)       _PCI_MAKEVALUE(x,S_HT_LINKCFG_MAXOUT)
#define G_HT_LINKCFG_MAXOUT(x)       _PCI_GETVALUE(x,S_HT_LINKCFG_MAXOUT,M_HT_LINKCFG_MAXOUT)

#define M_HT_LINKCFG_DWFCOUT         _PCI_MAKEMASK1(23)

#define S_HT_LINKCFG_WIDTHIN         24
#define M_HT_LINKCFG_WIDTHIN         _PCI_MAKEMASK(3,S_HT_LINKCFG_WIDTHIN)
#define V_HT_LINKCFG_WIDTHIN(x)      _PCI_MAKEVALUE(x,S_HT_LINKCFG_WIDTHIN)
#define G_HT_LINKCFG_WIDTHIN(x)      _PCI_GETVALUE(x,S_HT_LINKCFG_WIDTHIN,M_HT_LINKCFG_WIDTHIN)

#define M_HT_LINKCFG_DWFCIN_EN       _PCI_MAKEMASK1(27)

#define S_HT_LINKCFG_WIDTHOUT        28
#define M_HT_LINKCFG_WIDTHOUT        _PCI_MAKEMASK(3,S_HT_LINKCFG_WIDTHOUT)
#define V_HT_LINKCFG_WIDTHOUT(x)     _PCI_MAKEVALUE(x,S_HT_LINKCFG_WIDTHOUT)
#define G_HT_LINKCFG_WIDTHOUT(x)     _PCI_GETVALUE(x,S_HT_LINKCFG_WIDTHOUT,M_HT_LINKCFG_WIDTHOUT)

#define M_HT_LINKCFG_DWFCOUT_EN      _PCI_MAKEMASK1(31)

/*
 * HT Link Frequency/Error Register
 *    Primary/Device: relative offsets 0x0C, 0x10
 *    Secondary/host: relative offset  0x08, 0x0C
 */

/* Link0 only */
#define S_HT_LINKFREQ_HTREVID        8
#define M_HT_LINKFREQ_HTREVID        _PCI_MAKEMASK(8,S_HT_LINKFREQ_HTREVID)
#define V_HT_LINKFREQ_HTREVID(x)     _PCI_MAKEVALUE(x,S_HT_LINKFREQ_HTREVID)
#define G_HT_LINKFREQ_HTREVID(x)     _PCI_GETVALUE(x,S_HT_LINKFREQ_HTREVID,M_HT_LINKFREQ_HTREVID)

/* Link1 only (HTFeature) */
#define M_HT_LINKFREQ_ISOCFC         _PCI_MAKEMASK1(0)
#define M_HT_LINKFREQ_LDTSTOP        _PCI_MAKEMASK1(1)
#define M_HT_LINKFREQ_CRCTST         _PCI_MAKEMASK1(2)
#define M_HT_LINKFREQ_EXTCTL         _PCI_MAKEMASK1(3)
#define M_HT_LINKFREQ_64BADDR        _PCI_MAKEMASK1(4)
#define M_HT_LINKFREQ_UIDREODIS      _PCI_MAKEMASK1(5)
/* Secondary/host only */
#define M_HT_LINKFREQ_EXTREGSET      _PCI_MAKEMASK1(8)
#define M_HT_LINKFREQ_UPSTCFGEN      _PCI_MAKEMASK1(9)

/* Secondary/host Link0 only */
#define S_HT_LINKFREQ_FREQ           8
#define M_HT_LINKFREQ_FREQ           _PCI_MAKEMASK(4,S_HT_LINKFREQ_FREQ)
#define V_HT_LINKFREQ_FREQ(x)        _PCI_MAKEVALUE(x,S_HT_LINKFREQ_FREQ)
#define G_HT_LINKFREQ_FREQ(x)        _PCI_GETVALUE(x,S_HT_LINKFREQ_FREQ,M_HT_LINKFREQ_FREQ)

#define K_HT_LINKFREQ_200MHZ         0
#define K_HT_LINKFREQ_300MHZ         1
#define K_HT_LINKFREQ_400MHZ         2
#define K_HT_LINKFREQ_500MHZ         3
#define K_HT_LINKFREQ_600MHZ         4
#define K_HT_LINKFREQ_800MHZ         5
#define K_HT_LINKFREQ_1000MHZ        6
#define K_HT_LINKFREQ_VENDOR         15

#define M_HT_LINKFREQ_PROTERR        _PCI_MAKEMASK1(12)
#define M_HT_LINKFREQ_OVFLERR        _PCI_MAKEMASK1(13)
#define M_HT_LINKFREQ_EOCERR         _PCI_MAKEMASK1(14)
#define M_HT_LINKFREQ_CTLTO          _PCI_MAKEMASK1(15)

/* Secondary/host Link0 only */
#define S_HT_LINKFREQ_CAP            16
#define M_HT_LINKFREQ_CAP            _PCI_MAKEMASK(16,S_HT_LINKFREQ_CAP)
#define V_HT_LINKFREQ_CAP(x)         _PCI_MAKEVALUE(x,S_HT_LINKFREQ_CAP)
#define G_HT_LINKFREQ_CAP(x)         _PCI_GETVALUE(x,S_HT_LINKFREQ_CAP,M_HT_LINKFREQ_CAP)

/*
 * HT Error Handling Register
 *    Primary/Device: relative offset  0x14
 *    Secondary/host: relative offset  0x10
 */

#define S_HT_SCRATCHPAD               0
#define M_HT_SCRATCHPAD               _PCI_MAKEMASK(16,S_HT_SCRATCHPAD)
#define V_HT_SCRATCHPAD(x)            _PCI_MAKEVALUE(x,S_HT_SCRATCHPAD)
#define G_HT_SCRATCHPAD(x)            _PCI_GETVALUE(x,S_HT_SCRATCHPAD,M_HT_SCRATCHPAD)

#define M_HT_ERRHNDL_PROFL_EN        _PCI_MAKEMASK1(16)
#define M_HT_ERRHNDL_OVFFL_EN        _PCI_MAKEMASK1(17)
#define M_HT_ERRHNDL_PROFT_EN        _PCI_MAKEMASK1(18)
#define M_HT_ERRHNDL_OVFFT_EN        _PCI_MAKEMASK1(19)
#define M_HT_ERRHNDL_EOCFT_EN        _PCI_MAKEMASK1(20)
#define M_HT_ERRHNDL_RSPFT_EN        _PCI_MAKEMASK1(21)
#define M_HT_ERRHNDL_CRCFT_EN        _PCI_MAKEMASK1(22)
#define M_HT_ERRHNDL_SYSFTL_EN       _PCI_MAKEMASK1(23)
#define M_HT_ERRHNDL_CHNFAIL         _PCI_MAKEMASK1(24)
#define M_HT_ERRHNDL_RSPERR          _PCI_MAKEMASK1(25)
#define M_HT_ERRHNDL_PRONF_EN        _PCI_MAKEMASK1(26)
#define M_HT_ERRHNDL_OVFNF_EN        _PCI_MAKEMASK1(27)
#define M_HT_ERRHNDL_EOCNF_EN        _PCI_MAKEMASK1(28)
#define M_HT_ERRHNDL_RSPNF_EN        _PCI_MAKEMASK1(29)
#define M_HT_ERRHNDL_CRCNF_EN        _PCI_MAKEMASK1(30)
#define M_HT_ERRHNDL_SYSNF_EN        _PCI_MAKEMASK1(31)


/*
 * Switch Command Register
 */

#define M_HT_SWCMD_VIBERR            _PCI_MAKEMASK1(23)
#define M_HT_SWCMD_VIBFI_EN          _PCI_MAKEMASK1(23)
#define M_HT_SWCMD_VIBFT_EN          _PCI_MAKEMASK1(23)
#define M_HT_SWCMD_VIBNF_EN          _PCI_MAKEMASK1(23)

/* Note: includes CapType and SubType above */
#define S_HT_SWCMD_CAPTYPE           27
#define M_HT_SWCMD_CAPTYPE           _PCI_MAKEMASK(5,S_HT_SWCMD_CAPTYPE)
#define V_HT_SWCMD_CAPTYPE(x)        _PCI_MAKEVALUE(x,S_HT_SWCMD_CAPTYPE)
#define G_HT_SWCMD_CAPTYPE(x)        _PCI_GETVALUE(x,S_HT_SWCMD_CAPTYPE,M_HT_SWCMD_CAPTYPE)

/*
 * Switch Partition Register
 */

#define M_HT_SWPAR_PORT0INPARTITION  _PCI_MAKEMASK1(0)
#define M_HT_SWPAR_PORT1INPARTITION  _PCI_MAKEMASK1(1)
#define M_HT_SWPAR_PORT2INPARTITION  _PCI_MAKEMASK1(2)

/*
 * Switch Info Register
 */

#define S_HT_SWINFO_DEFPORT          0
#define M_HT_SWINFO_DEFPORT          _PCI_MAKEMASK(5,S_HT_SWINFO_DEFPORT)
#define V_HT_SWINFO_DEFPORT(x)       _PCI_MAKEVALUE(x,S_HT_SWINFO_DEFPORT)
#define G_HT_SWINFO_DEFPORT(x)       _PCI_GETVALUE(x,S_HT_SWINFO_DEFPORT,M_HT_SWINFO_DEFPORT)

#define M_HT_SWINFO_DECODEEN         _PCI_MAKEMASK1(5)
#define M_HT_SWINFO_COLDRESET        _PCI_MAKEMASK1(6)

#define S_HT_SWINFO_PERFINDEX        8
#define M_HT_SWINFO_PERFINDEX        _PCI_MAKEMASK(4,S_HT_SWINFO_PERFINDEX)
#define V_HT_SWINFO_PERFINDEX(x)     _PCI_MAKEVALUE(x,S_HT_SWINFO_PERFINDEX)
#define G_HT_SWINFO_PERFINDEX(x)     _PCI_GETVALUE(x,S_HT_SWINFO_PERFINDEX,M_HT_SWINFO_PERFINDEX)

#define S_HT_SWINFO_BLRINDEX         16
#define M_HT_SWINFO_BLRINDEX         _PCI_MAKEMASK(6,S_HT_SWINFO_BLRINDEX)
#define V_HT_SWINFO_BLRINDEX(x)      _PCI_MAKEVALUE(x,S_HT_SWINFO_BLRINDEX)
#define G_HT_SWINFO_BLRINDEX(x)      _PCI_GETVALUE(x,S_HT_SWINFO_BLRINDEX,M_HT_SWINFO_BLRINDEX)

#define M_HT_SWINFO_HOTPLUG          _PCI_MAKEMASK1(22)
#define M_HT_SWINFO_HIDEPORT         _PCI_MAKEMASK1(23)


/*
 * VCSet Command Register
 */

/* Note: includes CapType and SubType above */
#define S_HT_VCCMD_CAPTYPE           27
#define M_HT_VCCMD_CAPTYPE           _PCI_MAKEMASK(5,S_HT_VCCMD_CAPTYPE)
#define V_HT_VCCMD_CAPTYPE(x)        _PCI_MAKEVALUE(x,S_HT_VCCMD_CAPTYPE)
#define G_HT_VCCMD_CAPTYPE(x)        _PCI_GETVALUE(x,S_HT_VCCMD_CAPTYPE,M_HT_VCCMD_CAPTYPE)

/*
 * VCSet Configuration and Capability Register
 */

#define S_HT_VCCFG_VCSETSUP          0
#define M_HT_VCCFG_VCSETSUP          _PCI_MAKEMASK(8,S_HT_VCCFG_VCSETSUP)
#define V_HT_VCCFG_VCSETSUP(x)       _PCI_MAKEVALUE(x,S_HT_VCCFG_VCSETSUP)
#define G_HT_VCCFG_VCSETSUP(x)       _PCI_GETVALUE(x,S_HT_VCCFG_VCSETSUP,M_HT_VCCFG_VCSETSUP)

#define S_HT_VCCFG_L1ENBVCSET        8
#define M_HT_VCCFG_L1ENBVCSET        _PCI_MAKEMASK(8,S_HT_VCCFG_L1ENBVCSET)
#define V_HT_VCCFG_L1ENBVCSET(x)     _PCI_MAKEVALUE(x,S_HT_VCCFG_L1ENBVCSET)
#define G_HT_VCCFG_L1ENBVCSET(x)     _PCI_GETVALUE(x,S_HT_VCCFG_L1ENBVCSET,M_HT_VCCFG_L1ENBVCSET)

#define S_HT_VCCFG_L0ENBVCSET        16
#define M_HT_VCCFG_L0ENBVCSET        _PCI_MAKEMASK(8,S_HT_VCCFG_L0ENBVCSET)
#define V_HT_VCCFG_L0ENBVCSET(x)     _PCI_MAKEVALUE(x,S_HT_VCCFG_L0ENBVCSET)
#define G_HT_VCCFG_L0ENBVCSET(x)     _PCI_GETVALUE(x,S_HT_VCCFG_L0ENBVCSET,M_HT_VCCFG_L0ENBVCSET)

#define S_HT_VCCFG_VCSETEOC          24
#define M_HT_VCCFG_VCSETEOC          _PCI_MAKEMASK(8,S_HT_VCCFG_VCSETEOC)
#define V_HT_VCCFG_VCSETEOC(x)       _PCI_MAKEVALUE(x,S_HT_VCCFG_VCSETEOC)
#define G_HT_VCCFG_VCSETEOC(x)       _PCI_GETVALUE(x,S_HT_VCCFG_VCSETEOC,M_HT_VCCFG_VCSETEOC)

/*
 * VCSet Stream Configuration Register
 */

#define S_HT_VCSTRM_STRMBUCKDEPTH    0
#define M_HT_VCSTRM_STRMBUCKDEPTH    _PCI_MAKEMASK(8,S_HT_VCSTRM_STRMBUCKDEPTH)
#define V_HT_VCSTRM_STRMBUCKDEPTH(x) _PCI_MAKEVALUE(x,S_HT_VCSTRM_STRMBUCKDEPTH)
#define G_HT_VCSTRM_STRMBUCKDEPTH(x) _PCI_GETVALUE(x,S_HT_VCSTRM_STRMBUCKDEPTH,M_HT_VCSTRM_STRMBUCKDEPTH)

#define S_HT_VCSTRM_STRMINTERVAL     8
#define M_HT_VCSTRM_STRMINTERVAL     _PCI_MAKEMASK(8,S_HT_VCSTRM_STRMINTERVAL)
#define V_HT_VCSTRM_STRMINTERVAL(x)  _PCI_MAKEVALUE(x,S_HT_VCSTRM_STRMINTERVAL)
#define G_HT_VCSTRM_STRMINTERVAL(x)  _PCI_GETVALUE(x,S_HT_VCSTRM_STRMINTERVAL,M_HT_VCSTRM_STRMINTERVAL)

#define S_HT_VCSTRM_STRMSUP          16
#define M_HT_VCSTRM_STRMSUP          _PCI_MAKEMASK(8,S_HT_VCSTRM_STRMSUP)
#define V_HT_VCSTRM_STRMSUP(x)       _PCI_MAKEVALUE(x,S_HT_VCSTRM_STRMSUP)
#define G_HT_VCSTRM_STRMSUP(x)       _PCI_GETVALUE(x,S_HT_VCSTRM_STRMSUP,M_HT_VCSTRM_STRMSUP)

/*
 * VCSet NonFC Configuration Register
 */

#define S_HT_VCNONFC_NONFCBUCKDEPTH  0
#define M_HT_VCNONFC_NONFCBUCKDEPTH  _PCI_MAKEMASK(8,S_HT_VCNONFC_NONFCBUCKDEPTH)
#define V_HT_VCNONFC_NONFCBUCKDEPTH(x) _PCI_MAKEVALUE(x,S_HT_VCNONFC_NONFCBUCKDEPTH)
#define G_HT_VCNONFC_NONFCBUCKDEPTH(x) _PCI_GETVALUE(x,S_HT_VCNONFC_NONFCBUCKDEPTH,M_HT_VCNONFC_NONFCBUCKDEPTH)

#define S_HT_VCNONFC_NONFCINTERVAL   8
#define M_HT_VCNONFC_NONFCINTERVAL   _PCI_MAKEMASK(8,S_HT_VCNONFC_NONFCINTERVAL)
#define V_HT_VCNONFC_NONFCINTERVAL(x) _PCI_MAKEVALUE(x,S_HT_VCNONFC_NONFCINTERVAL)
#define G_HT_VCNONFC_NONFCINTERVAL(x) _PCI_GETVALUE(x,S_HT_VCNONFC_NONFCINTERVAL,M_HT_VCNONFC_NONFCINTERVAL)

#endif /* _PCI_CFG_H */
