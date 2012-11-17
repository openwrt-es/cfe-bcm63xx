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
/*   MODULE: 6348_common.h                                             */
/*   DATE:    04/12/19                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6348                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6348_MAP_COMMON_H
#define __BCM6348_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define PERF_BASE     0xfffe0000    /* chip control registers */
#define BB_BASE       0xfffe0100    /* bus bridge registers */
#define TIMR_BASE     0xfffe0200    /* timer registers */
#define UART_BASE     0xfffe0300    /* uart registers */
#define GPIO_BASE     0xfffe0400    /* gpio registers */
#define SPI_BASE      0xfffe0c00    /* SPI master controller registers */

#define USB_CTL_BASE  0xfffe1000    /* USB control registers */
#define USB_DMA_BASE  0xfffe1400    /* USB DMA control registers */
#define USB_OHCI_BASE 0xfffe1b00    /* USB host registers */
#define USBH_CFG_BASE 0xfffe1c00    /* USB host non-OHCI registers */

#define MPI_BASE      0xfffe2000    /* MPI control registers */
#define SDRAM_BASE    0xfffe2300    /* SDRAM control registers */

#define ADSL_BASE     0xfffe3000	/* ADSL core control registers */
#define ATM_BASE      0xfffe4000	/* ATM SAR control registers */
#define UBUS_BASE     0xfffe5000	/* UBUS status registers */
#define EMAC1_BASE    0xfffe6000    /* EMAC1 control registers */
#define EMAC2_BASE    0xfffe6800    /* EMAC2 control registers */
#define EMAC_DMA_BASE 0xfffe7000    /* EMAC DMA control registers */

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

#define SOFT_RESET	0x00000001

#define PLL_STRAP_VALUE  0x34

/*
#####################################################################
# SDRAM Control Registers
#####################################################################
*/
#define SDR_INIT_CTL        0x00
    /* Control Bits */
#define SDR_PFEN1           (1<<16)
#define SDR_PFEN0           (1<<15)
#define SDR_EMPRS           (1<<14)
#define SDR_2_BANKS         (1<<13)
#define SDR_1_BANK          (0<<13)
#define SDR_CS1_EN          (1<<12)
#define SDR_PEND            (1<<11)
#define SDR_32_BIT          (1<<10)
#define SDR_POWER_DOWN      (1<<9)
#define SDR_SELF_REFRESH    (1<<8)
#define SDR_11_COLS         (3<<6)
#define SDR_10_COLS         (2<<6)
#define SDR_9_COLS          (1<<6)
#define SDR_8_COLS          (0<<6)
#define SDR_13_ROWS         (2<<4)
#define SDR_12_ROWS         (1<<4)
#define SDR_11_ROWS         (0<<4)
#define SDR_MASTER_EN       (1<<3)
#define SDR_MRS_CMD         (1<<2)
#define SDR_PRE_CMD         (1<<1)
#define SDR_CBR_CMD         (1<<0)

#define SDR_CFG_REG         0x04
    /* Control Bits */
#define SDR_FULL_PG         0
#define SDR_BURST8          1
#define SDR_BURST4          2
#define SDR_BURST2          3
#define SDR_FAST_MEM        (1<<2)
#define SDR_SLOW_MEM        (0<<2)

#define SDR_REF_CTL         0x0C
    /* Control Bits */
#define SDR_REF_EN          (1<<15)

#define SDR_PRIOR           0x2C
    /* Control Bits */
#define SDR_EN_PRIOR        (1<<31)
#define SDR_ADSL_HPE        (1<<30)
#define SDR_MIPS_HPE        (1<<29)


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
