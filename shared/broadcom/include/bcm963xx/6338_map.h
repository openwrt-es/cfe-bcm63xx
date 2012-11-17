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
/*   MODULE:  6338_map.h                                               */
/*   DATE:    05/10/04                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6338                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6338_MAP_H
#define __BCM6338_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bcmtypes.h"
#include "6338_common.h"
#include "6338_intr.h"

/* macro to convert logical data addresses to physical */
/* DMA hardware must see physical address */
#define LtoP( x )       ( (uint32)x & 0x1fffffff )
#define PtoL( x )       ( LtoP(x) | 0xa0000000 )

/*
** Interrupt Controller
*/
typedef struct PerfControl {
  uint32        RevID;          /* (00) */
  uint16        testControl;    /* (04) */
  uint16        blkEnables;     /* (06) */

#define ADSL_CLK_EN     0x0001
#define MPI_CLK_EN      0x0002
#define DRAM_CLK_EN     0x0004
#define EMAC_CLK_EN     0x0010
#define USBS_CLK_EN     0x0010
#define SAR_CLK_EN      0x0020
#define SPI_CLK_EN      0x0200

  uint32        pll_control;    /* (08) */
#define SOFT_RESET 0x00000001

  uint32        IrqMask;        /* (0c) */
  uint32        IrqStatus;      /* (10) */

  uint32        ExtIrqCfg;
#define EI_SENSE_SHFT   0
#define EI_STATUS_SHFT  5
#define EI_CLEAR_SHFT   10
#define EI_MASK_SHFT    15
#define EI_INSENS_SHFT  20
#define EI_LEVEL_SHFT   25

  uint32        unused[4];      /* (18) */
  uint32        BlockSoftReset; /* (28) */
#define BSR_SPI             0x00000001
#define BSR_EMAC            0x00000004
#define BSR_USBH            0x00000008
#define BSR_USBS            0x00000010
#define BSR_ADSL            0x00000020
#define BSR_DMAMEM          0x00000040
#define BSR_SAR             0x00000080
#define BSR_ACLC            0x00000100
#define BSR_ADSL_MIPS_PLL   0x00000400
#define BSR_ALL_BLOCKS      \
    (BSR_SPI | BSR_EMAC | BSR_USBH | BSR_USBS | BSR_ADSL | BSR_DMAMEM | \
     BSR_SAR | BSR_ACLC | BSR_ADSL_MIPS_PLL) 
} PerfControl;

#define PERF ((volatile PerfControl * const) PERF_BASE)

/*
** Bus Bridge Registers
*/
typedef struct BusBridge {
  uint16    status;
#define BB_BUSY     0x8000      /* posted operation in progress */
#define BB_RD_PND   0x4000      /* read pending */
#define BB_RD_CMPLT 0x2000      /* read complete */
#define BB_ERROR    0x1000      /* posted write error */
#define BB_TEA      0x0800      /* transfer aborted */
  uint16    abortTimeoutCnt;    /* abort timeout value */

  byte      writePostEnable;
#define BB_POST_TIMR_EN 0x08        /* post writes to timer regs */
#define BB_POST_GPIO_EN 0x04        /* post writes to gpio regs */
#define BB_POST_INTC_EN 0x02        /* post writes to interrupt controller regs */
#define BB_POST_UART_EN 0x01        /* post writes to uart regs */
  byte      unused1[5];
  uint16    postAddr;       /* posted read address (lower half) */
  byte      unused2[3];
  byte      postData;       /* posted read data */
} BusBridge;

/* register offsets (needed for EBI master access) */
#define BB_STATUS       0
#define BB_ABORT_TO_CNT     2
#define BB_WR_POST_EN       4
#define BB_RD_POST_ADDR     10
#define BB_RD_POST_DATA     12

#define BRIDGE *bridge ((volatile BusBridge * const) BB_BASE)

/*
** Timer
*/
typedef struct Timer {
  uint16        unused0;
  byte          TimerMask;
#define TIMER0EN        0x01
#define TIMER1EN        0x02
#define TIMER2EN        0x04
  byte          TimerInts;
#define TIMER0          0x01
#define TIMER1          0x02
#define TIMER2          0x04
#define WATCHDOG        0x08
  uint32        TimerCtl0;
  uint32        TimerCtl1;
  uint32        TimerCtl2;
#define TIMERENABLE     0x80000000
#define RSTCNTCLR       0x40000000      
  uint32        TimerCnt0;
  uint32        TimerCnt1;
  uint32        TimerCnt2;
  uint32        WatchDogDefCount;

  /* Write 0xff00 0x00ff to Start timer
   * Write 0xee00 0x00ee to Stop and re-load default count
   * Read from this register returns current watch dog count
   */
  uint32        WatchDogCtl;

  /* Number of 40-MHz ticks for WD Reset pulse to last */
  uint32        WDResetCount;
} Timer;

#define TIMER ((volatile Timer * const) TIMR_BASE)

/*
** UART
*/
typedef struct UartChannel {
  byte          unused0;
  byte          control;
#define BRGEN           0x80    /* Control register bit defs */
#define TXEN            0x40
#define RXEN            0x20
#define LOOPBK          0x10
#define TXPARITYEN      0x08
#define TXPARITYEVEN    0x04
#define RXPARITYEN      0x02
#define RXPARITYEVEN    0x01

  byte          config;
#define XMITBREAK       0x40
#define BITS5SYM        0x00
#define BITS6SYM        0x10
#define BITS7SYM        0x20
#define BITS8SYM        0x30
#define ONESTOP         0x07
#define TWOSTOP         0x0f
  /* 4-LSBS represent STOP bits/char
   * in 1/8 bit-time intervals.  Zero
   * represents 1/8 stop bit interval.
   * Fifteen represents 2 stop bits.
   */
  byte          fifoctl;
#define RSTTXFIFOS      0x80
#define RSTRXFIFOS      0x40
  /* 5-bit TimeoutCnt is in low bits of this register.
   *  This count represents the number of characters 
   *  idle times before setting receive Irq when below threshold
   */
  uint32        baudword;
  /* When divide SysClk/2/(1+baudword) we should get 32*bit-rate
   */

  byte          txf_levl;       /* Read-only fifo depth */
  byte          rxf_levl;       /* Read-only fifo depth */
  byte          fifocfg;        /* Upper 4-bits are TxThresh, Lower are
                                 *      RxThreshold.  Irq can be asserted
                                 *      when rx fifo> thresh, txfifo<thresh
                                 */
  byte          prog_out;       /* Set value of DTR (Bit0), RTS (Bit1)
                                 *  if these bits are also enabled to GPIO_o
                                 */
#define	DTREN	0x01
#define	RTSEN	0x02

  byte          unused1;
  byte          DeltaIPEdgeNoSense;     /* Low 4-bits, set corr bit to 1 to 
                                         * detect irq on rising AND falling 
                                         * edges for corresponding GPIO_i
                                         * if enabled (edge insensitive)
                                         */
  byte          DeltaIPConfig_Mask;     /* Upper 4 bits: 1 for posedge sense
                                         *      0 for negedge sense if
                                         *      not configured for edge
                                         *      insensitive (see above)
                                         * Lower 4 bits: Mask to enable change
                                         *  detection IRQ for corresponding
                                         *  GPIO_i
                                         */
  byte          DeltaIP_SyncIP;         /* Upper 4 bits show which bits
                                         *  have changed (may set IRQ).
                                         *  read automatically clears bit
                                         * Lower 4 bits are actual status
                                         */

  uint16        intMask;				/* Same Bit defs for Mask and status */
  uint16        intStatus;
#define DELTAIP         0x0001
#define TXUNDERR        0x0002
#define TXOVFERR        0x0004
#define TXFIFOTHOLD     0x0008
#define TXREADLATCH     0x0010
#define TXFIFOEMT       0x0020
#define RXUNDERR        0x0040
#define RXOVFERR        0x0080
#define RXTIMEOUT       0x0100
#define RXFIFOFULL      0x0200
#define RXFIFOTHOLD     0x0400
#define RXFIFONE        0x0800
#define RXFRAMERR       0x1000
#define RXPARERR        0x2000
#define RXBRK           0x4000

  uint16        unused2;
  uint16        Data;                   /* Write to TX, Read from RX */
                                        /* bits 11:8 are BRK,PAR,FRM errors */

  uint32		unused3;
  uint32		unused4;
} Uart;

#define UART ((volatile Uart * const) UART_BASE)

/*
** Gpio Controller
*/

typedef struct GpioControl {
  uint32        unused0;
  uint32        GPIODir;      /* bits 7:0 */
  uint32        unused1;      /* bits 36:32 */
  uint32        GPIOio;
  uint32        LEDCtrl;
#define         LED3_STROBE             0x08000000
#define         LED2_STROBE             0x04000000
#define         LED1_STROBE             0x02000000
#define         LED0_STROBE             0x01000000
#define         LED_TEST                0x00010000
#define         LED3_DISABLE_LINK_ACT   0x00008000
#define         LED2_DISABLE_LINK_ACT   0x00004000
#define         LED1_DISABLE_LINK_ACT   0x00002000
#define         LED0_DISABLE_LINK_ACT   0x00001000
#define         LED_INTERVAL_SET_MASK   0x00000f00
#define         LED_INTERVAL_SET_320MS  0x00000500
#define         LED_INTERVAL_SET_160MS  0x00000400
#define         LED_INTERVAL_SET_80MS   0x00000300
#define         LED_INTERVAL_SET_40MS   0x00000200
#define         LED_INTERVAL_SET_20MS   0x00000100
#define         LED3_ON                 0x00000080
#define         LED2_ON                 0x00000040
#define         LED1_ON                 0x00000020
#define         LED0_ON                 0x00000010
#define         LED3_ENABLE             0x00000008
#define         LED2_ENABLE             0x00000004
#define         LED1_ENABLE             0x00000002
#define         LED0_ENABLE             0x00000001
  uint32        SpiSlaveCfg;
#define         SPI_SLAVE_RESET         0x00010000
#define         SPI_RESTRICT            0x00000400
#define         SPI_DELAY_DISABLE       0x00000200
#define         SPI_PROBE_MUX_SEL_MASK  0x000001e0
#define         SPI_SER_ADDR_CFG_MASK   0x0000000c
#define         SPI_MODE                0x00000001
  uint32        vRegConfig;
} GpioControl;

#define GPIO ((volatile GpioControl * const) GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define GPIO_NUM_MAX_BITS_MASK          0x0f
#define GPIO_NUM_TO_MASK(X)             (1 << ((X) & GPIO_NUM_MAX_BITS_MASK))

/*
** Spi Controller
*/

typedef struct SpiControl {
  uint16        spiCmd;                 /* (0x0): SPI command */
#define SPI_CMD_NOOP                    0
#define SPI_CMD_SOFT_RESET              1
#define SPI_CMD_HARD_RESET              2
#define SPI_CMD_START_IMMEDIATE         3

#define SPI_CMD_COMMAND_SHIFT           0
#define SPI_CMD_DEVICE_ID_SHIFT         4
#define SPI_CMD_PREPEND_BYTE_CNT_SHIFT  8
#define SPI_CMD_ONE_BYTE_SHIFT          11
#define SPI_CMD_ONE_WIRE_SHIFT          12
#define SPI_DEV_ID_0                    0
#define SPI_DEV_ID_1                    1
#define SPI_DEV_ID_2                    2
#define SPI_DEV_ID_3                    3

  byte          spiIntStatus;           /* (0x2): SPI interrupt status */
  byte          spiMaskIntStatus;       /* (0x3): SPI masked interrupt status */

  byte          spiIntMask;             /* (0x4): SPI interrupt mask */
#define SPI_INTR_CMD_DONE               0x01
#define SPI_INTR_RX_OVERFLOW            0x02
#define SPI_INTR_INTR_TX_UNDERFLOW      0x04
#define SPI_INTR_TX_OVERFLOW            0x08
#define SPI_INTR_RX_UNDERFLOW           0x10
#define SPI_INTR_CLEAR_ALL              0x1f

  byte          spiStatus;              /* (0x5): SPI status */
#define SPI_RX_EMPTY                    0x02
#define SPI_CMD_BUSY                    0x04
#define SPI_SERIAL_BUSY                 0x08

  byte          spiClkCfg;              /* (0x6): SPI clock configuration */
#define SPI_CLK_0_391MHZ                1
#define SPI_CLK_0_781MHZ                2 /* default */
#define SPI_CLK_1_563MHZ                3
#define SPI_CLK_3_125MHZ                4
#define SPI_CLK_6_250MHZ                5
#define SPI_CLK_12_50MHZ                6
#define SPI_CLK_MASK                    0x07
#define SPI_SSOFFTIME_MASK              0x38
#define SPI_SSOFFTIME_SHIFT             3
#define SPI_BYTE_SWAP                   0x80

  byte          spiFillByte;            /* (0x7): SPI fill byte */

  byte          unused0; 
  byte          spiMsgTail;             /* (0x9): msgtail */
  byte          unused1; 
  byte          spiRxTail;              /* (0xB): rxtail */

  uint32        unused2[13];            /* (0x0c - 0x3c) reserved */

  byte          spiMsgCtl;              /* (0x40) control byte */
#define FULL_DUPLEX_RW                  0
#define HALF_DUPLEX_W                   1
#define HALF_DUPLEX_R                   2
#define SPI_MSG_TYPE_SHIFT              6
#define SPI_BYTE_CNT_SHIFT              0
  byte          spiMsgData[63];         /* (0x41 - 0x7f) msg data */
  byte          spiRxDataFifo[64];      /* (0x80 - 0xbf) rx data */
  byte          unused3[64];            /* (0xc0 - 0xff) reserved */
} SpiControl;

#define SPI ((volatile SpiControl * const) SPI_BASE)

#define IUDMA_MAX_CHANNELS      16

/*
** DMA Channel Configuration (1 .. 16)
*/
typedef struct DmaChannelCfg {
  uint32        cfg;                    /* (00) assorted configuration */
#define         DMA_BURST_HALT  0x00000004  /* idle after finish current memory burst */
#define         DMA_PKT_HALT    0x00000002  /* idle after an EOP flag is detected */
#define         DMA_ENABLE  0x00000001      /* set to enable channel */
  uint32        intStat;                /* (04) interrupts control and status */
  uint32        intMask;                /* (08) interrupts mask */
#define         DMA_BUFF_DONE   0x00000001  /* buffer done */
#define         DMA_DONE        0x00000002  /* packet xfer complete */
#define         DMA_NO_DESC     0x00000004  /* no valid descriptors */
  uint32        maxBurst;               /* (0C) max burst length permitted */
} DmaChannelCfg;

/*
** DMA State RAM (1 .. 16)
*/
typedef struct DmaStateRam {
  uint32        baseDescPtr;            /* (00) descriptor ring start address */
  uint32        state_data;             /* (04) state/bytes done/ring offset */
  uint32        desc_len_status;        /* (08) buffer descriptor status and len */
  uint32        desc_base_bufptr;       /* (0C) buffer descrpitor current processing */
} DmaStateRam;

/*
** DMA Registers
*/
typedef struct DmaRegs {
#define DMA_MASTER_EN           0x00000001
#define DMA_FLOWC_CH1_EN        0x00000002
#define DMA_FLOWC_CH3_EN        0x00000004
#define DMA_NUM_CHS_MASK        0x0f000000
#define DMA_NUM_CHS_SHIFT       24
#define DMA_FLOWCTL_MASK        0x30000000
#define DMA_FLOWCTL_CH1         0x10000000
#define DMA_FLOWCTL_CH3         0x20000000
#define DMA_FLOWCTL_SHIFT       28
    uint32 controller_cfg;              /* (00) controller configuration */

    // Flow control Ch1
    uint32 flowctl_ch1_thresh_lo;       /* (04) EMAC1 RX DMA channel */
    uint32 flowctl_ch1_thresh_hi;       /* (08) EMAC1 RX DMA channel */
    uint32 flowctl_ch1_alloc;           /* (0C) EMAC1 RX DMA channel */
#define DMA_BUF_ALLOC_FORCE     0x80000000

    // Flow control Ch3
    uint32 flowctl_ch3_thresh_lo;       /* (10) EMAC2 RX DMA channel */
    uint32 flowctl_ch3_thresh_hi;       /* (14) EMAC2 RX DMA channel */
    uint32 flowctl_ch3_alloc;           /* (18) EMAC2 RX DMA channel */

    // Unused words
    uint32 resv[57];

    // Per channel registers/state ram
    DmaChannelCfg chcfg[IUDMA_MAX_CHANNELS]; /* (100) Channel configuration */
    union {
        DmaStateRam     s[IUDMA_MAX_CHANNELS];
        uint32          u32[4 * IUDMA_MAX_CHANNELS];
    } stram;                                /* (200) state ram */
} DmaRegs;

/*
** DMA Buffer 
*/
typedef struct DmaDesc {
  uint16        length;                 /* in bytes of data in buffer */
#define          DMA_DESC_USEFPM    0x8000
#define          DMA_DESC_MULTICAST 0x4000
#define          DMA_DESC_BUFLENGTH 0x0fff
  uint16        status;                 /* buffer status */
#define          DMA_OWN        0x8000  /* cleared by DMA, set by SW */
#define          DMA_EOP        0x4000  /* last buffer in packet */
#define          DMA_SOP        0x2000  /* first buffer in packet */
#define          DMA_WRAP       0x1000  /* */
#define          DMA_APPEND_CRC 0x0100

/* EMAC Descriptor Status definitions */
#define          EMAC_MISS      0x0080  /* framed address recognition failed (promiscuous) */
#define          EMAC_BRDCAST   0x0040  /* DA is Broadcast */
#define          EMAC_MULT      0x0020  /* DA is multicast */
#define          EMAC_LG        0x0010  /* frame length > RX_LENGTH register value */
#define          EMAC_NO        0x0008  /* Non-Octet aligned */
#define          EMAC_RXER      0x0004  /* RX_ERR on MII while RX_DV assereted */
#define          EMAC_CRC_ERROR 0x0002  /* CRC error */
#define          EMAC_OV        0x0001  /* Overflow */

/* HDLC Descriptor Status definitions */
#define          DMA_HDLC_TX_ABORT      0x0100
#define          DMA_HDLC_RX_OVERRUN    0x4000
#define          DMA_HDLC_RX_TOO_LONG   0x2000
#define          DMA_HDLC_RX_CRC_OK     0x1000
#define          DMA_HDLC_RX_ABORT      0x0100

  uint32        address;                /* address of data */
} DmaDesc;

/*
** Sdram Controller
*/
typedef struct SdramControllerRegs {
  uint16        unused1;
  uint16        initControl;    /* 02 */
#define SD_POWER_DOWN           0x200   /* put sdram into power down */
#define SD_SELF_REFRESH         0x100   /* enable self refresh mode */
#define SD_SOFT_RESET           0x080   /* soft reset all sdram controller regs */
#define SD_EDO_SELECT           0x040   /* select EDO mode */
#define SD_EDO_WAIT_STATE       0x020   /* add an EDO wait state */
#define SD_8MEG                 0x010   /* map sdram to 8 megs */
#define SD_MASTER_ENABLE        0x008   /* enable accesses to external sdram */
#define SD_MRS                  0x004   /* generate a mode register select cycle */
#define SD_PRECHARGE            0x002   /* generate a precharge cycle */
#define SD_CBR                  0x001   /* generate a refresh cycle */
  uint8         unused2[3];
  uint8         config;         /* 07 */
#define SD_FAST_MEM             0x04    /* 1=CAS latency of 2, 0 = CAS latency of 3 */
#define SD_BURST_LEN            0x03    /* set burst length */
#define SD_BURST_FULL_PAGE      0x00    /* .. full page */
#define SD_BURST_8              0x01    /* .. 8 words */
#define SD_BURST_4              0x02    /* .. 4 words */
#define SD_BURST_2              0x03    /* .. 2 words */
  uint16        unused3;
  uint16        refreshControl; /* 0a */
#define SD_REFRESH_ENABLE       0x8000  /* refresh enable */
#define SD_REFRESH_PERIOD       0x00ff  /* refresh period (16 x n x clock_period) */

  uint32        memoryBase;     /* 0c */
#define SD_MEMBASE_MASK         0xffffe000      /* base address mask */
#define SD_MEMSIZE_8MEG         0x00000001      /* memory is 8 meg */
#define SD_MEMSIZE_2MEG         0x00000001      /* memory is 2 meg */

} SdramControllerRegs;

/*
** External Bus Interface
*/
typedef struct EbiChipSelect {
  uint32        base;                   /* base address in upper 24 bits */
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
  uint32        config;
#define EBI_ENABLE          0x00000001      /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e      /* .. mask for wait states */
#define EBI_WTST_SHIFT      1               /* .. for shifting wait states */
#define EBI_WORD_WIDE       0x00000010      /* .. 16-bit peripheral, else 8 */
#define EBI_WREN            0x00000020      /* enable posted writes */
#define EBI_POLARITY        0x00000040      /* .. set to invert something, 
                                        **    don't know what yet */
#define EBI_FIFO            0x00000200      /* .. use fifo */
#define EBI_RE              0x00000400      /* .. Reverse Endian */
} EbiChipSelect;

typedef struct MpiRegisters {
  EbiChipSelect cs[1];                  /* size chip select configuration */
} MpiRegisters;

#define MPI ((volatile MpiRegisters * const) MPI_BASE)

/*
** EMAC transmit MIB counters
*/
typedef struct EmacTxMib {
  uint32        tx_good_octets;         /* (200) good byte count */
  uint32        tx_good_pkts;           /* (204) good pkt count */
  uint32        tx_octets;              /* (208) good and bad byte count */
  uint32        tx_pkts;                /* (20c) good and bad pkt count */
  uint32        tx_broadcasts_pkts;     /* (210) good broadcast packets */
  uint32        tx_multicasts_pkts;     /* (214) good mulitcast packets */
  uint32        tx_len_64;              /* (218) RMON tx pkt size buckets */
  uint32        tx_len_65_to_127;       /* (21c) */
  uint32        tx_len_128_to_255;      /* (220) */
  uint32        tx_len_256_to_511;      /* (224) */
  uint32        tx_len_512_to_1023;     /* (228) */
  uint32        tx_len_1024_to_max;     /* (22c) */
  uint32        tx_jabber_pkts;         /* (230) > 1518 with bad crc */
  uint32        tx_oversize_pkts;       /* (234) > 1518 with good crc */
  uint32        tx_fragment_pkts;       /* (238) < 63   with bad crc */
  uint32        tx_underruns;           /* (23c) fifo underrun */
  uint32        tx_total_cols;          /* (240) total collisions in all tx pkts */
  uint32        tx_single_cols;         /* (244) tx pkts with single collisions */
  uint32        tx_multiple_cols;       /* (248) tx pkts with multiple collisions */
  uint32        tx_excessive_cols;      /* (24c) tx pkts with excessive cols */
  uint32        tx_late_cols;           /* (250) tx pkts with late cols */
  uint32        tx_defered;             /* (254) tx pkts deferred */
  uint32        tx_carrier_lost;        /* (258) tx pkts with CRS lost */
  uint32        tx_pause_pkts;          /* (25c) tx pause pkts sent */
#define NumEmacTxMibVars        24
} EmacTxMib;

/*
** EMAC receive MIB counters
*/
typedef struct EmacRxMib {
  uint32        rx_good_octets;         /* (280) good byte count */
  uint32        rx_good_pkts;           /* (284) good pkt count */
  uint32        rx_octets;              /* (288) good and bad byte count */
  uint32        rx_pkts;                /* (28c) good and bad pkt count */
  uint32        rx_broadcasts_pkts;     /* (290) good broadcast packets */
  uint32        rx_multicasts_pkts;     /* (294) good mulitcast packets */
  uint32        rx_len_64;              /* (298) RMON rx pkt size buckets */
  uint32        rx_len_65_to_127;       /* (29c) */
  uint32        rx_len_128_to_255;      /* (2a0) */
  uint32        rx_len_256_to_511;      /* (2a4) */
  uint32        rx_len_512_to_1023;     /* (2a8) */
  uint32        rx_len_1024_to_max;     /* (2ac) */
  uint32        rx_jabber_pkts;         /* (2b0) > 1518 with bad crc */
  uint32        rx_oversize_pkts;       /* (2b4) > 1518 with good crc */
  uint32        rx_fragment_pkts;       /* (2b8) < 63   with bad crc */
  uint32        rx_missed_pkts;         /* (2bc) missed packets */
  uint32        rx_crc_align_errs;      /* (2c0) both or either */
  uint32        rx_undersize;           /* (2c4) < 63   with good crc */
  uint32        rx_crc_errs;            /* (2c8) crc errors (only) */
  uint32        rx_align_errs;          /* (2cc) alignment errors (only) */
  uint32        rx_symbol_errs;         /* (2d0) pkts with RXERR assertions (symbol errs) */
  uint32        rx_pause_pkts;          /* (2d4) MAC control, PAUSE */
  uint32        rx_nonpause_pkts;       /* (2d8) MAC control, not PAUSE */
#define NumEmacRxMibVars        23
} EmacRxMib;

typedef struct EmacRegisters {
  uint32        rxControl;              /* (00) receive control */
#define          EMAC_PM_REJ    0x80    /*      - reject DA match in PMx regs */
#define          EMAC_UNIFLOW   0x40    /*      - accept cam match fc */
#define          EMAC_FC_EN     0x20    /*      - enable flow control */
#define          EMAC_LOOPBACK  0x10    /*      - loopback */
#define          EMAC_PROM      0x08    /*      - promiscuous */
#define          EMAC_RDT       0x04    /*      - ignore transmissions */
#define          EMAC_ALL_MCAST 0x02    /*      - ignore transmissions */
#define          EMAC_NO_BCAST  0x01    /*      - ignore transmissions */


  uint32        rxMaxLength;            /* (04) receive max length */
  uint32        txMaxLength;            /* (08) transmit max length */
  uint32        unused1[1];
  uint32        mdioFreq;               /* (10) mdio frequency */
#define          EMAC_MII_PRE_EN 0x00000080 /* prepend preamble sequence */
#define          EMAC_MDIO_PRE   0x00000080 /*      - enable MDIO preamble */
#define          EMAC_MDC_FREQ   0x0000007f /*      - mdio frequency */

  uint32        mdioData;               /* (14) mdio data */
#define          MDIO_WR        0x50020000 /*   - write framing */
#define          MDIO_RD        0x60020000 /*   - read framing */
#define          MDIO_PMD_SHIFT  23
#define          MDIO_REG_SHIFT  18

  uint32        intMask;                /* (18) int mask */
  uint32        intStatus;              /* (1c) int status */
#define          EMAC_FLOW_INT  0x04    /*      - flow control event */
#define          EMAC_MIB_INT   0x02    /*      - mib event */
#define          EMAC_MDIO_INT  0x01    /*      - mdio event */

  uint32        unused2[3];
  uint32        config;                 /* (2c) config */
#define          EMAC_ENABLE    0x001   /*      - enable emac */
#define          EMAC_DISABLE   0x002   /*      - disable emac */
#define          EMAC_SOFT_RST  0x004   /*      - soft reset */
#define          EMAC_SOFT_RESET 0x004  /*      - emac soft reset */
#define          EMAC_EXT_PHY   0x008   /*      - external PHY select */

  uint32        txControl;              /* (30) transmit control */
#define          EMAC_FD        0x001   /*      - full duplex */
#define          EMAC_FLOWMODE  0x002   /*      - flow mode */
#define          EMAC_NOBKOFF   0x004   /*      - no backoff in  */
#define          EMAC_SMALLSLT  0x008   /*      - small slot time */

  uint32        txThreshold;            /* (34) transmit threshold */
  uint32        mibControl;             /* (38) mib control */
#define          EMAC_NO_CLEAR  0x001   /* don't clear on read */

  uint32        unused3[7];

  uint32        pm0DataLo;              /* (58) perfect match 0 data lo */
  uint32        pm0DataHi;              /* (5C) perfect match 0 data hi (15:0) */
  uint32        pm1DataLo;              /* (60) perfect match 1 data lo */
  uint32        pm1DataHi;              /* (64) perfect match 1 data hi (15:0) */
  uint32        pm2DataLo;              /* (68) perfect match 2 data lo */
  uint32        pm2DataHi;              /* (6C) perfect match 2 data hi (15:0) */
  uint32        pm3DataLo;              /* (70) perfect match 3 data lo */
  uint32        pm3DataHi;              /* (74) perfect match 3 data hi (15:0) */
#define          EMAC_CAM_V   0x10000  /*      - cam index */
#define          EMAC_CAM_VALID 0x00010000

  uint32        unused4[98];            /* (78-1fc) */

  EmacTxMib     tx_mib;                 /* (200) emac tx mib */
  uint32        unused5[8];             /* (260-27c) */

  EmacRxMib     rx_mib;                 /* (280) rx mib */

} EmacRegisters;

/* register offsets for subrouting access */
#define EMAC_RX_CONTROL         0x00
#define EMAC_RX_MAX_LENGTH      0x04
#define EMAC_TX_MAC_LENGTH      0x08
#define EMAC_MDIO_FREQ          0x10
#define EMAC_MDIO_DATA          0x14
#define EMAC_INT_MASK           0x18
#define EMAC_INT_STATUS         0x1C
#define EMAC_CAM_DATA_LO        0x20
#define EMAC_CAM_DATA_HI        0x24
#define EMAC_CAM_CONTROL        0x28
#define EMAC_CONTROL            0x2C
#define EMAC_TX_CONTROL         0x30
#define EMAC_TX_THRESHOLD       0x34
#define EMAC_MIB_CONTROL        0x38


#define EMAC1 ((volatile EmacRegisters * const) EMAC1_BASE)

/*
** USB Registers
*/
typedef struct UsbRegisters {
    byte inttf_setting;
    byte current_config;
    uint16 status_frameNum;
#define USB_LINK        0x2000 
#define USB_BUS_RESET   0x1000 
#define USB_SUSPENDED   0x0800 
    byte unused1;
    byte endpt_prnt;
    byte endpt_dirn;
    byte endpt_status;
#define USB_ENDPOINT_0  0x01
#define USB_ENDPOINT_1  0x02
#define USB_ENDPOINT_2  0x04
#define USB_ENDPOINT_3  0x08
#define USB_ENDPOINT_4  0x10
#define USB_ENDPOINT_5  0x20
#define USB_ENDPOINT_6  0x40
#define USB_ENDPOINT_7  0x80
    uint32 unused2;
    byte conf_mem_ctl;
#define USB_CONF_MEM_RD     0x80
#define USB_CONF_MEM_RDY    0x40
    byte unused2a;
    byte conf_mem_read_address;
    byte conf_mem_write_address;

    byte unused3;
    byte dev_req_bytesel;
    uint16 ext_dev_data;

    byte unused4;
    byte clr_fifo;
    byte endpt_stall_reset;  // use same endpoint #'s from above
    byte usb_cntl;
#define USB_FORCE_ERR       0x20
#define USB_SOFT_RESET      0x10
#define USB_RESUME          0x08
#define USB_COMMAND_ERR     0x04
#define USB_COMMAND_OVER    0x02
    byte irq_addr;
    byte iso_out_in_addr;
    byte blk_out_in_addr;
    byte cntl_addr;
    uint32 unusedx[2];
    uint32 tx_ram_write_port;
    uint32 fifo_status;  // (see bcm6338 data sheet for definition)

    uint32 irq_status;
    uint32 irq_mask;
#define USB_NEW_CONFIG              0x00000001   
#define USB_SETUP_COMMAND_RECV      0x00000002 // non-standard setup cmd rcvd
#define USB_OUT_FIFO_OV             0x00000004   
#define USB_RESET_RECV              0x00000008   
#define USB_SUSPEND_RECV            0x00000010   
#define USB_FIFO_REWIND             0x00000020   
#define USB_RX_BULK_FIFO_DATA_AVAIL 0x00000040   
#define USB_RX_ISO_FIFO_DATA_AVAIL  0x00000080   
#define USB_LINK_CHANGE             0x00010000   
    uint32 endpt_cntl;
#define USB_R_WK_EN                 0x0100   
#define USB_TX_EOP                  0x0200   
#define USB_TX_CNTL_DMA_EN          0x0400
#define USB_TX_BULK_DMA_EN          0x0800
#define USB_TX_ISO_DMA_EN           0x1000
#define USB_RX_CNTL_DMA_EN          0x2000
#define USB_RX_BULK_DMA_EN          0x4800
#define USB_RX_ISO_DMA_EN           0x8000
    uint32 rx_status_read_port;
    uint32 confmem_read_port;
    uint32 confmem_write_port;
    uint32 fifo_ovf_count;
    uint32 fifo_rewind_cnt;
    uint32 terminal_count;
} UsbRegisters;

#define USB ((volatile UsbRegisters * const) USB_CTL_BASE)

/*
** ADSL core Registers
*/

#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)

typedef struct AdslRegisters {
    uint32 core_control;
#define ADSL_RESET		0x01
   
    uint32 core_status;
#define ADSL_HOST_MSG	0x01

    uint32 PAD;
    uint32 bist_status;
    uint32 PAD[4];
    uint32 int_status_i; /* 0x20 */
    uint32 int_mask_i;
    uint32 int_status_f;
    uint32 int_mask_f;
#define ADSL_INT_HOST_MSG		0x00000020
#define ADSL_INT_DESC_ERR		0x00000400
#define ADSL_INT_DATA_ERR		0x00000800
#define ADSL_INT_DESC_PROTO_ERR	0x00001000
#define ADSL_INT_RCV_DESC_UF	0x00002000
#define ADSL_INT_RCV_FIFO_OF	0x00004000
#define ADSL_INT_XMT_FIFO_UF	0x00008000
#define ADSL_INT_RCV			0x00010000
#define ADSL_INT_XMT			0x01000000

    uint32 PAD[116];

	uint32	xmtcontrol_intr; /* 0x200 */
#define ADSL_DMA_XMT_EN			0x00000001
#define ADSL_DMA_XMT_LE			0x00000004
	uint32	xmtaddr_intr;
#define ADSL_DMA_ADDR_MASK		0xFFFFF000
	uint32	xmtptr_intr;
#define ADSL_DMA_LAST_DESC_MASK	0x00000FFF
	uint32	xmtstatus_intr;
#define ADSL_DMA_CURR_DESC_MASK	0x00000FFF
#define ADSL_DMA_XMT_STATE_MASK	0x0000F000
#define ADSL_DMA_XMT_STATE_DIS	0x00000000
#define ADSL_DMA_XMT_STATE_ACT	0x00001000
#define ADSL_DMA_XMT_STATE_IDLE	0x00002000
#define ADSL_DMA_XMT_STATE_STOP	0x00003000

#define ADSL_DMA_XMT_ERR_MASK	0x000F0000
#define ADSL_DMA_XMT_ERR_NONE	0x00000000
#define ADSL_DMA_XMT_ERR_DPE	0x00010000
#define ADSL_DMA_XMT_ERR_FIFO	0x00020000
#define ADSL_DMA_XMT_ERR_DTE	0x00030000
#define ADSL_DMA_XMT_ERR_DRE	0x00040000

	uint32	rcvcontrol_intr;
#define ADSL_DMA_RCV_EN			0x00000001
#define ADSL_DMA_RCV_FO			0x000000FE
	uint32	rcvaddr_intr;
	uint32	rcvptr_intr;
	uint32	rcvstatus_intr;
#define ADSL_DMA_RCV_STATE_MASK	0x0000F000
#define ADSL_DMA_RCV_STATE_DIS	0x00000000
#define ADSL_DMA_RCV_STATE_ACT	0x00001000
#define ADSL_DMA_RCV_STATE_IDLE	0x00002000
#define ADSL_DMA_RCV_STATE_STOP	0x00003000

#define ADSL_DMA_RCV_ERR_MASK	0x000F0000
#define ADSL_DMA_RCV_ERR_NONE	0x00000000
#define ADSL_DMA_RCV_ERR_DPE	0x00010000
#define ADSL_DMA_RCV_ERR_FIFO	0x00020000
#define ADSL_DMA_RCV_ERR_DTE	0x00030000
#define ADSL_DMA_RCV_ERR_DRE	0x00040000

	uint32	xmtcontrol_fast;
	uint32	xmtaddr_fast;
	uint32	xmtptr_fast;
	uint32	xmtstatus_fast;
	uint32	rcvcontrol_fast;
	uint32	rcvaddr_fast;
	uint32	rcvptr_fast;
	uint32	rcvstatus_fast;
	uint32	PAD[48];

	uint32	host_message; /* 0x300 */
	uint32	PAD[805];
    uint32 core_reset;
    uint32 core_error;
    uint32 core_revision;

#define	ADSL_CORE_REV			0x0000000F
#define	ADSL_CORE_TYPE			0x0000FFF0
} AdslRegisters;

#define ADSL ((volatile AdslRegisters * const) ADSL_BASE)

#ifdef __cplusplus
}
#endif

#endif

