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
/*   MODULE:  6358_map.h                                               */
/*   DATE:    08/09/05                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6358                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6358_MAP_H
#define __BCM6358_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bcmtypes.h"
#include "6358_common.h"
#include "6358_intr.h"

/* macro to convert logical data addresses to physical */
/* DMA hardware must see physical address */
#define LtoP( x )       ( (uint32)x & 0x1fffffff )
#define PtoL( x )       ( LtoP(x) | 0xa0000000 )

typedef struct MemoryControl
{
    uint32 Control;             /* (00) */
#define MEMC_SELF_REFRESH    (1<<6) // enable self refresh mode
#define MEMC_MRS             (1<<4) // generate a mode register select cycle
#define MEMC_PRECHARGE       (1<<3) // generate a precharge cycle
#define MEMC_REFRESH         (1<<2) // generate an auto refresh cycle
#define MEMC_SEQUENCE_ENABLE (1<<1) // enable memory controller sequencer
#define MEMC_MASTER_ENABLE   (1<<0) // enable accesses to external sdram

    uint32  Config;             /* (04) */
#define MEMC_SYS_PORT_CMD_MODE  (1<<18)
#define MEMC_PAD_OP_MODE        (1<<17)
#define MEMC_DQS_GATE_EN        (1<<16)
#define MEMC_PRED_RD_STROBE_EN  (1<<15)
#define MEMC_PRED_RD_LATENCY_SEL (1<<14)
#define MEMC_MIPS1HIPREN        (1<<11)
#define MEMC_MIPS0HIPREN        (1<<10)
#define MEMC_HIPRRTYQEN         (1<<9)
#define MEMC_UBUS_CLF_EN        (1<<8)

#define MEMC_11_BIT_ROW         (0<<6)
#define MEMC_12_BIT_ROW         (1<<6)
#define MEMC_13_BIT_ROW         (2<<6)
#define MEMC_14_BIT_ROW         (3<<6)

#define MEMC_8_BIT_COL          (0<<3)
#define MEMC_9_BIT_COL          (1<<3)
#define MEMC_10_BIT_COL         (2<<3)
#define MEMC_11_BIT_COL         (3<<3)

#define MEMC_SEL_PRIORITY       (1<<2)

#define MEMC_32BIT_BUS          (0<<1)
#define MEMC_16BIT_BUS          (1<<1)

#define MEMC_MEMTYPE_SDR        (0<<0)
#define MEMC_MEMTYPE_DDR        (1<<0)

    uint32  RefreshPdControl;   /* (08) */ 
    uint32  BistStatus;         /* (0C) */
    uint32  ExtendedModeBuffer; /* (10) */
    uint32  BankClosingTimer;   /* (14) */
    uint32  PriorityInversionTimer; /* (18) */

    uint32  DramTiming;         /* (1c) */
#define MEMC_WR_NOP_RD        (1<<23)
#define MEMC_WR_NOP_WR        (1<<22)
#define MEMC_RD_NOP_WR        (1<<21)
#define MEMC_RD_NOP_RD        (1<<20)
#define MEMC_CAS_LATENCY_2    (0)
#define MEMC_CAS_LATENCY_2_5  (1)
#define MEMC_CAS_LATENCY_3    (2)

    uint32  IntStatus;          /* (20) */
    uint32  IntMask;            /* (24) */
#define MEMC_INT3             (1<<3)
#define MEMC_INT2             (2<<3)
#define MEMC_INT1             (1<<3)
#define MEMC_INT0             (0<<3)

    uint32  IntInfo;            /* (28) */
    uint8   unused5[0x50-0x2c]; /* (2c) */
    uint32  Barrier;            /* (50) */
    uint32  CoreId;             /* (54) */
} MemoryControl;

#define MEMC ((volatile MemoryControl * const) MEMC_BASE)

typedef struct DDRControl {
    uint32 RevID;
    uint32 PadSSTLMode;
    uint32 CmdPadCntl;
    uint32 DQPadCntl;
    uint32 DQSPadCntl;
    uint32 ClkPadCntl0;
    uint32 MIPSDDRPLLConfig;
#define MIPSDDR_N2_SHFT         29
#define MIPSDDR_N2_MASK         (0x7<<MIPSDDR_N2_SHFT)
#define MIPSDDR_N1_SHFT         23
#define MIPSDDR_N1_MASK         (0x3f<<MIPSDDR_N1_SHFT)
#define DDR_MDIV_SHFT           8
#define DDR_MDIV_MASK           (0xff<<DDR_MDIV_SHFT)
#define MIPS_MDIV_SHFT          0
#define MIPS_MDIV_MASK          (0xff<<MIPS_MDIV_SHFT)

    uint32 PLLDeskew;
    uint32 MIPSPhaseCntl;
    uint32 DDR1_2PhaseCntl0;
    uint32 DDR3_4PhaseCntl0;
    uint32 VCDLPhaseCntl;
    uint32 Misc;
    uint32 Spare0;
    uint32 Spare1;
    uint32 Spare2;
    uint32 CLBist;
    uint32 LBistCRC;
} DDRControl;

#define DDR ((volatile DDRControl * const) DDR_BASE)
    
/*
** Peripheral Controller
*/
typedef struct PerfControl {
  uint32        RevID;          /* (00) */
  uint32        blkEnables;     /* (04) */

#define EPHY_CLK_EN     (1 << 21)
#define USBSU_CLK_EN    (1 << 20)
#define EMAC2_CLK_EN    (1 << 19)
#define EMAC1_CLK_EN    (1 << 18)
#define EMUSB_CLK_EN    (1 << 17)
#define EMAC_CLK_EN     (EPHY_CLK_EN | EMAC1_CLK_EN | EMAC2_CLK_EN | EMUSB_CLK_EN | USBSU_CLK_EN)
#define SAR_CLK_EN      (1 << 11)
#define USBS_CLK_EN     (1 << 10)
#define SPI_CLK_EN      (1 << 9)
#define PCM_CLK_EN      (1 << 8)
#define ADSL_CLK_EN     (1 << 5)

  uint32        pll_control;    /* (08) */
#define SOFT_RESET 0x00000001
#define SOFT_RESET       0x00000001
#define MPI_UTOPIA_25MHZ 0x00000002
#define MPI_UTOPIA_50MHZ 0x00000000
#define MPI_UTOPIA_MASK  0x00000002
#define MPI_CLK_50MHZ    0x00000000
#define MPI_CLK_40MHZ    0x00000008
#define MPI_CLK_33MHZ    0x00000010
#define MPI_CLK_25MHZ    0x00000018
#define MPI_CLK_MASK     0x00000018

  uint32        IrqMask;        /* (0c) */
  uint32        IrqStatus;      /* (10) */

  uint32        ExtIrqCfg;      /* (14) */
#define EI_SENSE_SHFT   0
#define EI_STATUS_SHFT  4
#define EI_CLEAR_SHFT   8
#define EI_MASK_SHFT    12
#define EI_INSENS_SHFT  16
#define EI_LEVEL_SHFT   20

  uint32        unused1;        /* (18) */

  uint32        ExtIrqCfg1;     /* (1c) */

  uint32        unused2[5];     /* (20) */
  uint32        BlockSoftReset; /* (34) */
#define BSR_SPI             (1 << 0)
#define BSR_EMAC            (1 << 2)
#define BSR_MPI             (1 << 3)
#define BSR_FPM             (1 << 5)
#define BSR_EPHY            (1 << 6)
#define BSR_SAR             (1 << 7)
#define BSR_USBH            (1 << 12)
#define BSR_PCM             (1 << 13)
#define BSR_ADSL            (1 << 14)
  uint32        IrqMask1;       /* (38) */
  uint32        IrqStatus1;     /* (3c) */
} PerfControl;

#define PERF ((volatile PerfControl * const) PERF_BASE)

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
  uint32    GPIODir_high; /* bits 36:32 */
  uint32    GPIODir;      /* bits 31:00 */
  uint32    GPIOio_high;  /* bits 36:32 */
  uint32    GPIOio;       /* bits 31:00 */
  uint32    LEDCtrl;
#define LED3_STROBE             0x08000000
#define LED2_STROBE             0x04000000
#define LED1_STROBE             0x02000000
#define LED0_STROBE             0x01000000
#define LED_TEST                0x00010000
#define LED3_DISABLE_LINK_ACT   0x00008000
#define LED2_DISABLE_LINK_ACT   0x00004000
#define LED1_DISABLE_LINK_ACT   0x00002000
#define LED0_DISABLE_LINK_ACT   0x00001000
#define LED_INTERVAL_SET_MASK   0x00000f00
#define LED_INTERVAL_SET_320MS  0x00000500
#define LED_INTERVAL_SET_160MS  0x00000400
#define LED_INTERVAL_SET_80MS   0x00000300
#define LED_INTERVAL_SET_40MS   0x00000200
#define LED_INTERVAL_SET_20MS   0x00000100
#define LED3_ON                 0x00000080
#define LED2_ON                 0x00000040
#define LED1_ON                 0x00000020
#define LED0_ON                 0x00000010
#define LED3_ENABLE             0x00000008
#define LED2_ENABLE             0x00000004
#define LED1_ENABLE             0x00000002
#define LED0_ENABLE             0x00000001
  uint32    SpiSlaveCfg;
#define SPI_SLAVE_RESET         0x00010000
#define SPI_RESTRICT            0x00000400
#define SPI_DELAY_DISABLE       0x00000200
#define SPI_PROBE_MUX_SEL_MASK  0x000001e0
#define SPI_SER_ADDR_CFG_MASK   0x0000000c
#define SPI_MODE                0x00000001

  uint32    GPIOMode;
#define GPIO_MODE_SYS_IRQ               (1<<15)
#define GPIO_MODE_PWM_SYNC_CLK          (1<<14)
#define GPIO_MODE_CLKRST                (1<<13)
#define GPIO_MODE_UTOPIA_OVERLAY        (1<<12)
#define GPIO_MODE_LED_OVERLAY           (1<<11)
#define GPIO_MODE_SERIAL_LED_OVERLAY    (1<<10)
#define GPIO_MODE_LEGACY_LED_OVERLAY    (1<<9)
#define GPIO_MODE_ASYNC_MODEM_OVERLAY   (1<<8)
#define GPIO_MODE_SPI_SS_OVERLAY        (1<<7)
#define GPIO_MODE_UART1_OVERLAY         (1<<6)
#define GPIO_MODE_EXTRA_CS_OVERLAY      (1<<5)

  uint32    VregConfig;
  uint32    AuxLedInterval;
#define AUX_LED_IN_7            0x80000000
#define AUX_LED_IN_6            0x40000000
#define AUX_LED_IN_5            0x20000000
#define AUX_LED_IN_4            0x10000000
#define AUX_LED_IN_MASK         0xf0000000
#define LED_IN_3                0x08000000
#define LED_IN_2                0x04000000
#define LED_IN_1                0x02000000
#define LED_IN_0                0x01000000
#define AUX_LED_TEST            0x00400000
#define USE_NEW_INTV            0x00200000
#define LED7_LNK_ORAND          0x00100000
#define LED7_LNK_MASK           0x000f0000
#define LED7_LNK_MASK_SHFT      16
#define LED7_ACT_MASK           0x0000f000
#define LED7_ACT_MASK_SHFT      12
#define AUX_FLASH_INTV          0x00000fc0
#define AUX_FLASH_INTV_SHFT     6
#define AUX_BLINK_INTV          0x0000003f

  uint32    AuxLedCtrl;
#define AUX_HW_DISAB_7          0x80000000
#define AUX_STROBE_7            0x40000000
#define AUX_MODE_7              0x30000000
#define AUX_MODE_SHFT_7         28
#define AUX_HW_DISAB_6          0x08000000
#define AUX_STROBE_6            0x04000000
#define AUX_MODE_6              0x03000000
#define AUX_MODE_SHFT_6         24
#define AUX_HW_DISAB_5          0x00800000
#define AUX_STROBE_5            0x00400000
#define AUX_MODE_5              0x00300000
#define AUX_MODE_SHFT_5         20
#define AUX_HW_DISAB_4          0x00080000
#define AUX_STROBE_4            0x00040000
#define AUX_MODE_4              0x00030000
#define AUX_MODE_SHFT_4         16
#define AUX_HW_DISAB_3          0x00008000
#define AUX_STROBE_3            0x00004000
#define AUX_MODE_3              0x00003000
#define AUX_MODE_SHFT_3         12
#define AUX_HW_DISAB_2          0x00000800
#define AUX_STROBE_2            0x00000400
#define AUX_MODE_2              0x00000300
#define AUX_MODE_SHFT_2         8
#define AUX_HW_DISAB_1          0x00000080
#define AUX_STROBE_1            0x00000040
#define AUX_MODE_1              0x00000030
#define AUX_MODE_SHFT_1         4
#define AUX_HW_DISAB_0          0x00000008
#define AUX_STROBE_0            0x00000004
#define AUX_MODE_0              0x00000003
#define AUX_MODE_SHFT_0         0

#define LED_STEADY_OFF          0x0
#define LED_FLASH               0x1
#define LED_BLINK               0x2
#define LED_STEADY_ON           0x3

  uint32    TestControl;
  uint32    unused[5];
  uint32    StrapBus;
#define UTOPIA_MASTER_ON        (1<<14)
  uint32    StrapOverride;
  uint32    unused1[2];
  uint32    SerialLed;
  uint32    SerialLedCtrl;
#define SER_LED_BUSY            (1<<3)
#define SER_LED_POLARITY        (1<<2)
#define SER_LED_DIV_1           0
#define SER_LED_DIV_2           1
#define SER_LED_DIV_4           2
#define SER_LED_DIV_8           3
#define SER_LED_DIV_MASK        0x3
#define SER_LED_DIV_SHIFT       0
  uint32    DiagMemCtrl;
  uint32    DiagMemSize;
  uint32    SrcAddr;
  uint32    DstAddr;
  uint32    DieRevID;
} GpioControl;

#define GPIO ((volatile GpioControl * const) GPIO_BASE)

/* Number to mask conversion macro used for GPIODir and GPIOio */
#define GPIO_NUM_TOTAL_BITS_MASK        0x3f
#define GPIO_NUM_MAX_BITS_MASK          0x1f
#define GPIO_NUM_TO_MASK(X)             ( (((X) & GPIO_NUM_TOTAL_BITS_MASK) < 32) ? (1 << ((X) & GPIO_NUM_MAX_BITS_MASK)) : (0) )

/* Number to mask conversion macro used for GPIODir_high and GPIOio_high */
#define GPIO_NUM_MAX_BITS_MASK_HIGH     0x07
#define GPIO_NUM_TO_MASK_HIGH(X)        ( (((X) & GPIO_NUM_TOTAL_BITS_MASK) >= 32) ? (1 << ((X-32) & GPIO_NUM_MAX_BITS_MASK_HIGH)) : (0) )


/*
** Spi Controller
*/

typedef struct SpiControl {
  uint16        spiMsgCtl;              /* (0x0) control byte */
#define FULL_DUPLEX_RW                  0
#define HALF_DUPLEX_W                   1
#define HALF_DUPLEX_R                   2
#define SPI_MSG_TYPE_SHIFT              14
#define SPI_BYTE_CNT_SHIFT              0
  byte          spiMsgData[0x21e];      /* (0x02 - 0x21f) msg data */
  byte          unused0[0x1e0];
  byte          spiRxDataFifo[0x220];   /* (0x400 - 0x61f) rx data */
  byte          unused1[0xe0];

  uint16        spiCmd;                 /* (0x700): SPI command */
#define SPI_CMD_NOOP                    0
#define SPI_CMD_SOFT_RESET              1
#define SPI_CMD_HARD_RESET              2
#define SPI_CMD_START_IMMEDIATE         3

#define SPI_CMD_COMMAND_SHIFT           0
#define SPI_CMD_COMMAND_MASK            0x000f

#define SPI_CMD_DEVICE_ID_SHIFT         4
#define SPI_CMD_PREPEND_BYTE_CNT_SHIFT  8
#define SPI_CMD_ONE_BYTE_SHIFT          11
#define SPI_CMD_ONE_WIRE_SHIFT          12
#define SPI_DEV_ID_0                    0
#define SPI_DEV_ID_1                    1
#define SPI_DEV_ID_2                    2
#define SPI_DEV_ID_3                    3

  byte          spiIntStatus;           /* (0x702): SPI interrupt status */
  byte          spiMaskIntStatus;       /* (0x703): SPI masked interrupt status */

  byte          spiIntMask;             /* (0x704): SPI interrupt mask */
#define SPI_INTR_CMD_DONE               0x01
#define SPI_INTR_RX_OVERFLOW            0x02
#define SPI_INTR_INTR_TX_UNDERFLOW      0x04
#define SPI_INTR_TX_OVERFLOW            0x08
#define SPI_INTR_RX_UNDERFLOW           0x10
#define SPI_INTR_CLEAR_ALL              0x1f

  byte          spiStatus;              /* (0x705): SPI status */
#define SPI_RX_EMPTY                    0x02
#define SPI_CMD_BUSY                    0x04
#define SPI_SERIAL_BUSY                 0x08

  byte          spiClkCfg;              /* (0x706): SPI clock configuration */
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

  byte          spiFillByte;            /* (0x707): SPI fill byte */
  byte          unused2; 
  byte          spiMsgTail;             /* (0x709): msgtail */
  byte          unused3; 
  byte          spiRxTail;              /* (0x70B): rxtail */
} SpiControl;

#define SPI ((volatile SpiControl * const) SPI_BASE)

typedef struct PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  PCM_ENABLE              0x80000000     // PCM block master enable
#define  PCM_ENABLE_SHIFT        31
#define  PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  PCM_SLAVE_SEL_SHIFT     30
#define  PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  PCM_CLOCK_INV_SHIFT     29
#define  PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  PCM_FS_INVERT_SHIFT     28
#define  PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  PCM_FS_FREQ_16_8_SHIFT  27
#define  PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  PCM_FS_LONG_SHIFT       26
#define  PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  PCM_FS_TRIG_SHIFT       25
#define  PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  PCM_DATA_OFF_SHIFT      24
#define  PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  PCM_DATA_16_8_SHIFT     23
#define  PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  PCM_LSB_FIRST_SHIFT     18
#define  PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  PCM_LOOPBACK_SHIFT      17
#define  PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select
#define  PCM_EXTCLK_SEL_SHIFT    16
#define  PCM_AP_SEL              0x00000001     // set to give pads to PCM, reset for AP to use pads

    uint32 pcm_chan_ctrl;                       // 04
#define  PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  PCM_TDM_FRAME           0x00000004     // PCM frame boundary

    uint32 pcm_pll_ctrl1;                       // 10
#define  PCM_PLL_RESET           0x80000000     // Digital reset
#define  PCM_PLL_FILRST          0x40000000     // VCO/loop filter reset
#define  PCM_PLL_PWRDN           0x20000000     // Powerdown enable for PLL
#define  PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
                                                // Do not deassert until PCM PLL is out of reset and locked
#define  PCM_PLL_SRCSEL          0x00030000     // Clock input to PLL
#define  PCM_PLL_SRCSEL_SHIFT    16             
#define  PCM_PLL_DIVRETIME       0x00000010     // retime for 8-bit dividers (0 - high speed retime enabled, 1 - no retiming)
#define  PCM_PLL_PI              0x00000001     // Phase interpolator (0 - 5-bit PI, 1 - 6-bit PI)

    uint32 pcm_pll_ctrl2;                       // 14
    uint32 pcm_pll_ctrl3;                       // 18

    uint32 pcm_pll_stat;                        // 1c
#define  PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    uint32 unused[8];

#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} PcmControlRegisters;

#define PCM ((volatile PcmControlRegisters * const) PCM_BASE)

typedef struct PcmIudmaRegisters
{
	uint16 numChp;
#define BCM6358_IUDMA_REGS_NUM_CHP_MASK     0x0f00
#define BCM6358_IUDMA_REGS_NUM_CHP_SHIFT    8
   uint16 ctrlConfig;
#define BCM6358_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6358_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6358_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004

	// Flow control Ch1
   uint16 reserved1;
   uint16 ch1_FC_Low_Thr;

   uint16 reserved2;
   uint16 ch1_FC_High_Thr;

   uint16 reserved3;
   uint16 ch1_Buff_Alloc;

	// Flow control Ch3
	uint16 reserved4;
	uint16 ch3_FC_Low_Thr;

	uint16 reserved5;
	uint16 ch3_FC_High_Thr;

	uint16 reserved6;
	uint16 ch3_Buff_Alloc;

} PcmIudmaRegisters;

typedef struct PcmIudmaChannelCtrl
{
   uint16 reserved1;
	uint16 config;
#define BCM6358_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6358_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6358_IUDMA_CONFIG_BURSTHALT   0x0004

	uint16 reserved2;
	uint16 intStat;
#define BCM6358_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6358_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6358_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6358_IUDMA_INTSTAT_MASK    0x0007
#define BCM6358_IUDMA_INTSTAT_ALL     BCM6358_IUDMA_INTSTAT_MASK

	uint16 reserved3;
	uint16 intMask;
#define BCM6358_IUDMA_INTMASK_BDONE   0x0001
#define BCM6358_IUDMA_INTMASK_PDONE   0x0002
#define BCM6358_IUDMA_INTMASK_NOTVLD  0x0004

	uint16 reserved4;
	uint16 maxBurst;
#define BCM6358_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} PcmIudmaChannelCtrl;


typedef struct PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6358_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} PcmIudmaStateRam;

#define BCM6358_MAX_PCM_DMA_CHANNELS 6

typedef struct PcmIudma
{
   PcmIudmaRegisters regs;
   uint16 reserved1[114];
   PcmIudmaChannelCtrl ctrl[BCM6358_MAX_PCM_DMA_CHANNELS];
   uint16 reserved2[80];
   PcmIudmaStateRam stram[BCM6358_MAX_PCM_DMA_CHANNELS];

} PcmIudma;

#define PCM_IUDMA ((volatile PcmIudma * const) PCM_IUDMA_BASE)


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
#define EBI_TS_TA_MODE      0x00000080      /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100      /* .. drive tsize, not bs_b */
#define EBI_FIFO            0x00000200      /* .. use fifo */
#define EBI_RE              0x00000400      /* .. Reverse Endian */
#define EBI_SETUP_SHIFT     16
#define EBI_HOLD_SHIFT      20
#define EBI_SETUP_STATES    0x0f0000
#define EBI_HOLD_STATES     0xf00000
} EbiChipSelect;

typedef struct MpiRegisters {
  EbiChipSelect cs[7];                  /* size chip select configuration */
#define EBI_CS0_BASE            0
#define EBI_CS1_BASE            1
#define EBI_CS2_BASE            2
#define EBI_CS3_BASE            3
#define PCMCIA_COMMON_BASE      4
#define PCMCIA_ATTRIBUTE_BASE   5
#define PCMCIA_IO_BASE          6
  uint32        unused0[2];             /* reserved */
  uint32        ebi_control;            /* ebi control */
  uint32        unused1[4];             /* reserved */
#define EBI_ACCESS_TIMEOUT      0x000007FF
  uint32        pcmcia_cntl1;           /* pcmcia control 1 */
#define PCCARD_CARD_RESET       0x00040000
#define CARDBUS_ENABLE          0x00008000
#define PCMCIA_ENABLE           0x00004000
#define PCMCIA_GPIO_ENABLE      0x00002000
#define CARDBUS_IDSEL           0x00001F00
#define VS2_OEN                 0x00000080
#define VS1_OEN                 0x00000040
#define VS2_OUT                 0x00000020
#define VS1_OUT                 0x00000010
#define VS2_IN                  0x00000008
#define VS1_IN                  0x00000004
#define CD2_IN                  0x00000002
#define CD1_IN                  0x00000001
#define VS_MASK                 0x0000000C
#define CD_MASK                 0x00000003
  uint32        unused2;                /* reserved */
  uint32        pcmcia_cntl2;           /* pcmcia control 2 */
#define PCMCIA_BYTESWAP_DIS     0x00000002
#define PCMCIA_HALFWORD_EN      0x00000001
#define RW_ACTIVE_CNT_BIT       2
#define INACTIVE_CNT_BIT        8
#define CE_SETUP_CNT_BIT        16
#define CE_HOLD_CNT_BIT         24
  uint32        unused3[40];            /* reserved */

  uint32        sp0range;               /* PCI to internal system bus address space */
#define ADDR_SPACE_MASK         0xFFFF0000
  uint32        sp0remap;
  uint32        sp0cfg;
  uint32        sp1range;
  uint32        sp1remap;
  uint32        sp1cfg;

  uint32        EndianCfg;

  uint32        l2pcfgctl;              /* internal system bus to PCI IO/Cfg control */
#define DIR_CFG_SEL             0x80000000 /* change from PCI I/O access to PCI config access */
#define DIR_CFG_USEREG          0x40000000 /* use this register info for PCI configuration access */
#define DEVICE_NUMBER           0x00007C00 /* device number for the PCI configuration access */
#define FUNC_NUMBER             0x00000300 /* function number for the PCI configuration access */
#define REG_NUMBER              0x000000FC /* register number for the PCI configuration access */
#define CONFIG_TYPE             0x00000003 /* configuration type for the PCI configuration access */

  uint32        l2pmrange1;             /* internal system bus to PCI memory space */
#define PCI_SIZE_64K            0xFFFF0000
#define PCI_SIZE_128K           0xFFFE0000
#define PCI_SIZE_256K           0xFFFC0000
#define PCI_SIZE_512K           0xFFF80000
#define PCI_SIZE_1M             0xFFF00000
#define PCI_SIZE_2M             0xFFE00000
#define PCI_SIZE_4M             0xFFC00000
#define PCI_SIZE_8M             0xFF800000
#define PCI_SIZE_16M            0xFF000000
#define PCI_SIZE_32M            0xFE000000
  uint32        l2pmbase1;              /* kseg0 or kseg1 address & 0x1FFFFFFF */
  uint32        l2pmremap1;
#define CARDBUS_MEM             0x00000004
#define MEM_WINDOW_EN           0x00000001
  uint32        l2pmrange2;
  uint32        l2pmbase2;
  uint32        l2pmremap2;
  uint32        l2piorange;             /* internal system bus to PCI I/O space */
  uint32        l2piobase;
  uint32        l2pioremap;

  uint32        pcimodesel;
#define PCI2_INT_BUS_RD_PREFECH 0x000000F0
#define PCI_BAR2_NOSWAP         0x00000002 /* BAR at offset 0x20 */
#define PCI_BAR1_NOSWAP         0x00000001 /* BAR at affset 0x1c */

  uint32        pciintstat;             /* PCI interrupt mask/status */
#define MAILBOX1_SENT           0x08
#define MAILBOX0_SENT           0x04
#define MAILBOX1_MSG_RCV        0x02
#define MAILBOX0_MSG_RCV        0x01
  uint32        locbuscntrl;            /* internal system bus control */
#define DIR_U2P_NOSWAP          0x00000002
#define EN_PCI_GPIO             0x00000001
  uint32        locintstat;             /* internal system bus interrupt mask/status */
#define CSERR                   0x0200
#define SERR                    0x0100
#define EXT_PCI_INT             0x0080
#define DIR_FAILED              0x0040
#define DIR_COMPLETE            0x0020
#define PCI_CFG                 0x0010
  uint32        unused4[7];

  uint32        mailbox0;
  uint32        mailbox1;

  uint32        pcicfgcntrl;            /* internal system bus PCI configuration control */
#define PCI_CFG_REG_WRITE_EN    0x00000080
#define PCI_CFG_ADDR            0x0000003C
  uint32        pcicfgdata;             /* internal system bus PCI configuration data */

  uint32        locch2ctl;              /* PCI to interrnal system bus DMA (downstream) local control */
#define MPI_DMA_HALT            0x00000008  /* idle after finish current memory burst */
#define MPI_DMA_PKT_HALT        0x00000004  /* idle after an EOP flag is detected */
#define MPI_DMA_STALL           0x00000002  /* idle after an EOP flag is detected */
#define MPI_DMA_ENABLE          0x00000001  /* set to enable channel */
  uint32        locch2intStat;
#define MPI_DMA_NO_DESC         0x00000004  /* no valid descriptors */
#define MPI_DMA_DONE            0x00000002  /* packet xfer complete */
#define MPI_DMA_BUFF_DONE       0x00000001  /* buffer done */
  uint32        locch2intMask;
  uint32        unused5;
  uint32        locch2descaddr;
  uint32        locch2status1;
#define LOCAL_DESC_STATE        0xE0000000
#define PCI_DESC_STATE          0x1C000000
#define BYTE_DONE               0x03FFC000
#define RING_ADDR               0x00003FFF
  uint32        locch2status2;
#define BUFPTR_OFFSET           0x1FFF0000
#define PCI_MASTER_STATE        0x000000C0
#define LOC_MASTER_STATE        0x00000038
#define CONTROL_STATE           0x00000007
  uint32        unused6;

  uint32        locch1Ctl;              /*internal system bus to PCI DMA (upstream) local control */
#define DMA_U2P_LE              0x00000200  /* local bus is little endian */
#define DMA_U2P_NOSWAP          0x00000100  /* lccal bus is little endian but no data swapped */
  uint32        locch1intstat;
  uint32        locch1intmask;
  uint32        unused7;
  uint32        locch1descaddr;
  uint32        locch1status1;
  uint32        locch1status2;
  uint32        unused8;

  uint32        pcich1ctl;              /* internal system bus to PCI DMA PCI control */
  uint32        pcich1intstat;
  uint32        pcich1intmask;
  uint32        pcich1descaddr;
  uint32        pcich1status1;
  uint32        pcich1status2;

  uint32        pcich2Ctl;              /* PCI to internal system bus DMA PCI control */
  uint32        pcich2intstat;
  uint32        pcich2intmask;
  uint32        pcich2descaddr;
  uint32        pcich2status1;
  uint32        pcich2status2;

  uint32        perm_id;                /* permanent device and vendor id */
  uint32        perm_rev;               /* permanent revision id */
} MpiRegisters;

#define MPI ((volatile MpiRegisters * const) MPI_BASE)

/* PCI configuration address space start offset 0x40 */
#define BRCM_PCI_CONFIG_TIMER               0x40
#define BRCM_PCI_CONFIG_TIMER_RETRY_MASK    0x0000FF00
#define BRCM_PCI_CONFIG_TIMER_TRDY_MASK     0x000000FF

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
#define EMAC2 ((volatile EmacRegisters * const) EMAC2_BASE)

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
    uint32 fifo_status;  // (see bcm6358 data sheet for definition)

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

typedef struct USBControl {
    uint32 SwapControl;
#define EHCI_LOGICAL_ADDRESS_EN (1<<5)
#define EHCI_ENDIAN_SWAP        (1<<4)
#define EHCI_DATA_SWAP          (1<<3)
#define OHCI_LOGICAL_ADDRESS_EN (1<<2)
#define OHCI_ENDIAN_SWAP        (1<<1)
#define OHCI_DATA_SWAP          (1<<0)
    uint32 unused[8];
    uint32 TestPortControl;
} USBControl;

#define USBH ((volatile USBControl * const) USBH_CFG_BASE)

#ifdef __cplusplus
}
#endif

#endif

