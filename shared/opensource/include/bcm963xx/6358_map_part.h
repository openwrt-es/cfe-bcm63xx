/*
<:copyright-gpl 
 Copyright 2002 Broadcom Corp. All Rights Reserved. 
 
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

#ifndef __BCM6358_MAP_H
#define __BCM6358_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bcmtypes.h"

#define PERF_BASE     0xfffe0000  /* chip control registers */
#define TIMR_BASE     0xfffe0040  /* timer registers */
#define GPIO_BASE     0xfffe0080  /* gpio registers */
#define UART_BASE     0xfffe0100  /* uart registers */
#define SPI_BASE      0xfffe0800  /* SPI master controller registers */
#define MPI_BASE      0xfffe1000  /* MPI control registers */
#define MEMC_BASE     0xfffe1200  /* Memory control registers */
#define DDR_BASE      0xfffe12a0  /* DDR IO Buf Control registers */
#define USB_EHCI_BASE 0xfffe1300  /* USB host registers */
#define USB_OHCI_BASE 0xfffe1400  /* USB host registers */
#define USBH_CFG_BASE 0xfffe1500

typedef struct MemoryControl {
    uint32 Control;
    uint32 Config;
    uint32 RefPDControl;
    uint32 BistStatus;
    uint32 ModExtModBuf;
    uint32 BankClsTimer;
    uint32 PrioInvTimer;
    uint32 DRAMTiming;
    uint32 IntStatus;
    uint32 IntMask;
    uint32 IntInfo;
    uint32 Reserved[6];
    uint32 Barrier;
    uint32 CoreID;
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
  uint32        GPIODir_high; /* bits 36:32 */
  uint32        GPIODir;      /* bits 31:00 */
  uint32        GPIOio_high;  /* bits 36:32 */
  uint32        GPIOio;       /* bits 31:00 */
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

  uint32        GPIOMode;
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
#define BRCM_PCI_CONFIG_TIMER_RETRY_MASK	0x0000FF00
#define BRCM_PCI_CONFIG_TIMER_TRDY_MASK		0x000000FF

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

