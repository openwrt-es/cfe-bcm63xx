/*
<:copyright-broadcom 
 
 Copyright (c) 2005 Broadcom Corporation 
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
/*   MODULE:  6358_common.h                                             */
/*   DATE:    08/09/05                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6358                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6358_MAP_COMMON_H
#define __BCM6358_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define PERF_BASE            0xfffe0000  /* chip control registers */
#define TIMR_BASE            0xfffe0040  /* timer registers */
#define GPIO_BASE            0xfffe0080  /* gpio registers */
#define UART_BASE            0xfffe0100  /* uart registers */
#define SPI_BASE             0xfffe0800  /* SPI master controller registers */
#define MPI_BASE             0xfffe1000  /* MPI control registers */
#define MEMC_BASE            0xfffe1200  /* Memory control registers */
#define DDR_BASE             0xfffe12a0  /* DDR IO Buf Control registers */
#define USB_EHCI_BASE        0xfffe1300  /* USB host registers */
#define USB_OHCI_BASE        0xfffe1400  /* USB host registers */
#define USBH_CFG_BASE        0xfffe1500
#define PCM_BASE             0xfffe1600  /* PCM control registers */
#define PCM_IUDMA_BASE       0xfffe1800  /* PCM UIDMA register base */
#define ATM_BASE             0xfffe2000  /* ATM SAR control registers */
#define ADSL_BASE            0xfffe3000  /* ADSL core control registers */
#define EMAC1_BASE           0xfffe4000  /* EMAC1 control registers */
#define EMAC2_BASE           0xfffe4800  /* EMAC2 control registers */
#define USB_CTL_BASE         0xfffe5400  /* USB control registers */
#define EMAC_DMA_BASE        0xfffe5000  /* EMAC DMA control registers */
#define USB_DMA_BASE         0xfffe5000  /* USB DMA control registers */

/*
#####################################################################
# System PLL Control Register
#####################################################################
*/
#define SYSPLLCFG       0x08

#define M_MPI_MASK      0x00000018      // 4:3
#define M_MPI_SHFT      3
#define M_MPI_50MHZ     0
#define M_MPI_40MHZ     1
#define M_MPI_33MHZ     2
#define M_MPI_25MHZ     3

#define M_UTO_MASK      0x00000002      // 1:1
#define M_UTO_SHFT      1

#define SOFT_RESET      0x00000001

/*
#####################################################################
# Memory Control Registers
#####################################################################
*/
#define MEMC_CONTROL             0x0
#define MEMC_CONFIG              0x4
#define MEMC_REF_PD_CONTROL      0x8
#define MEMC_BIST_STATUS         0xc
#define MEMC_M_EM_BUF           0x10
#define MEMC_BANK_CLS_TIM       0x14
#define MEMC_PRIOR_INV_TIM      0x18
#define MEMC_DRAM_TIM           0x1c
#define MEMC_INT_STATUS         0x20
#define MEMC_INT_MASK           0x24
#define MEMC_INT_INFO           0x28
#define MEMC_BARRIER            0x50
#define MEMC_CORE_ID            0x54

#define DDR_REV_ID               0x0
#define DDR_PAD_SSTL_MODE        0x4
#define DDR_CMD_PAD_CNTL         0x8
#define DDR_DQ_PAD_CNTL          0xc
#define DDR_DQS_PAD_CNTL        0x10    
#define DDR_CLK_PAD_CNTL0       0x14
#define DDR_MIPSDDR_PLL_CONFIG  0x18
#define DDR_PLL_DESKEW_REG      0x1c

#define DDR_MIPS_PHASE_CNTL     0x20
#define DDR_DDR1_2_PHASE_CNTL0  0x24
#define DDR_DDR3_4_PHASE_CNTL0  0x28    
#define DDR_VCDL_PHASE_CNTL     0x2c
#define DDR_MISC                0x30
#define DDR_SPARE0              0x34
#define DDR_SPARE1              0x38
#define DDR_SPARE2              0x3c
#define DDR_CLBIST              0x40    
#define DDR_LBIST_CRC           0x44


// Some bit/field definitions for the MEMC_CONTROL register.
#define MEMC_DQSGATEEN          (1<<16)
#define MEMC_MIPS1HIPREN        (1<<11)
#define MEMC_MIPS0HIPREN        (1<<10)
#define MEMC_HIPRRTYQEN         (1<<9)

#define MEMC_11BIT_ROW          (0<<6)
#define MEMC_12BIT_ROW          (1<<6)
#define MEMC_13BIT_ROW          (2<<6)
#define MEMC_14BIT_ROW          (3<<6)

#define MEMC_8BIT_COL           (0<<3)
#define MEMC_9BIT_COL           (1<<3)
#define MEMC_10BIT_COL          (2<<3)
#define MEMC_11BIT_COL          (3<<3)

#define MEMC_SELPRIORITY        (1<<2)

#define MEMC_32BIT_BUS          (0<<1)
#define MEMC_16BIT_BUS          (1<<1)

#define MEMC_MEMTYPE_SDR        (0<<0)
#define MEMC_MEMTYPE_DDR        (1<<0)

/*
#####################################################################
# MPI Control Registers
#####################################################################
*/
#define CS0BASE         0x00
#define CS0CNTL         0x04

/*
# CSxBASE settings
#   Size in low 4 bits
#   Base Address for match in upper 24 bits
*/
#define EBI_SIZE_8K         0
#define EBI_SIZE_16K        1
#define EBI_SIZE_32K        2
#define EBI_SIZE_64K        3
#define EBI_SIZE_128K       4
#define EBI_SIZE_256K       5
#define EBI_SIZE_512K       6
#define EBI_SIZE_1M         7
#define EBI_SIZE_2M         8
#define EBI_SIZE_4M         9
#define EBI_SIZE_8M         10
#define EBI_SIZE_16M        11
#define EBI_SIZE_32M        12
#define EBI_SIZE_64M        13
#define EBI_SIZE_128M       14
#define EBI_SIZE_256M       15

/* CSxCNTL settings */
#define EBI_ENABLE          0x00000001  /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e  /* .. mask for wait states */
#define ZEROWT              0x00000000  /* ..  0 WS */
#define ONEWT               0x00000002  /* ..  1 WS */
#define TWOWT               0x00000004  /* ..  2 WS */
#define THREEWT             0x00000006  /* ..  3 WS */
#define FOURWT              0x00000008  /* ..  4 WS */
#define FIVEWT              0x0000000a  /* ..  5 WS */
#define SIXWT               0x0000000c  /* ..  6 WS */
#define SEVENWT             0x0000000e  /* ..  7 WS */
#define EBI_WORD_WIDE       0x00000010  /* .. 16-bit peripheral, else 8 */
#define EBI_POLARITY        0x00000040  /* .. set to invert chip select polarity */
#define EBI_TS_TA_MODE      0x00000080  /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100  /* .. drive tsize, not bs_b */
#define EBI_FIFO            0x00000200  /* .. enable fifo */
#define EBI_RE              0x00000400  /* .. Reverse Endian */

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define UART0CONTROL     0x01
#define UART0CONFIG      0x02
#define UART0RXTIMEOUT   0x03
#define UART0BAUD        0x04
#define UART0FIFOCFG     0x0a
#define UART0INTMASK     0x10
#define UART0INTSTAT     0x12
#define UART0DATA        0x17

#define BRGEN            0x80   /* Control register bit defs */
#define TXEN             0x40
#define RXEN             0x20
#define LOOPBK           0x10
#define TXPARITYEN       0x08
#define TXPARITYEVEN     0x04
#define RXPARITYEN       0x02
#define RXPARITYEVEN     0x01

#define XMITBREAK        0x40   /* Config register */
#define BITS5SYM         0x00
#define BITS6SYM         0x10
#define BITS7SYM         0x20
#define BITS8SYM         0x30
#define ONESTOP          0x07
#define TWOSTOP          0x0f

#define RSTTXFIFOS       0x80   /* Rx Timeout register */
#define RSTRXFIFOS       0x40

#define TX4              0x40   /* FIFO config register */
#define RX4              0x04

#define DELTAIP          0x0001 /* Interrupt Status and Mask registers */
#define TXUNDERR         0x0002
#define TXOVFERR         0x0004
#define TXFIFOTHOLD      0x0008
#define TXREADLATCH      0x0010
#define TXFIFOEMT        0x0020
#define RXUNDERR         0x0040
#define RXOVFERR         0x0080
#define RXTIMEOUT        0x0100
#define RXFIFOFULL       0x0200
#define RXFIFOTHOLD      0x0400
#define RXFIFONE         0x0800
#define RXFRAMERR        0x1000
#define RXPARERR         0x2000
#define RXBRK            0x4000
          
#ifdef __cplusplus
}
#endif

#endif
