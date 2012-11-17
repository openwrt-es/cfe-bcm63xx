/*
<:copyright-broadcom 
 
 Copyright (c) 2004 Broadcom Corporation 
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
#ifndef _BCMMII_H_
#define _BCMMII_H_

#include "dev_bcm6348_eth.h"

/*---------------------------------------------------------------------*/
/* Broadcom PHY MII register address                                   */
/* use when PhyType is BP_ENET_INTERNAL_PHY                            */
/*---------------------------------------------------------------------*/
#define MII_AUX_CTRL_STATUS                 0x18
#define MII_TX_CONTROL                      0x19
#define MII_AUX_STATUS3                     0x1C
#define MII_BRCM_TEST                       0x1f

/* MII Auxiliary control/status register */
#define MII_AUX_CTRL_STATUS_SP100_INDICATOR 0x0002  /* Speed indication */

/* MII TX Control register. */
#define MII_TX_CONTROL_PGA_FIX_ENABLE       0x0100

/* MII Auxilliary Status 3 register. */
#define MII_AUX_STATUS3_MSE_MASK            0xFF00

/* Broadcom Test register. */
#define MII_BRCM_TEST_HARDRESET             0x0200
#define MII_BRCM_TEST_SHADOW_ENABLE         0x0080
#define MII_BRCM_TEST_10BT_SERIAL_NODRIB    0x0008
#define MII_BRCM_TEST_100TX_POWERDOWN       0x0002
#define MII_BRCM_TEST_10BT_POWERDOWN        0x0001

/* Advertisement control register. */
#define ADVERTISE_FDFC                      0x0400  /* MII advertisement register Flow Control */


/*---------------------------------------------------------------------*/
/* 5325 Switch Page Register                                           */
/* use when PhyType is BP_ENET_EXTERNAL_SWITCH                         */
/*---------------------------------------------------------------------*/
#define PAGE_CONTROL                        0x00
#define PAGE_SELECT                         0xff

/* Control page registers */
#define REG_CONTROL_MII1_PORT_STATE_OVERRIDE 0x0e
#define REG_POWER_DOWN_MODE                 0x0f

/* MII1 Port State Override Register Page 0x00 Address 0x0e */
#define REG_CONTROL_MPSO_MII_SW_OVERRIDE    0x80
#define REG_CONTROL_MPSO_REVERSE_MII        0x10
#define REG_CONTROL_MPSO_LP_FLOW_CONTROL    0x08
#define REG_CONTROL_MPSO_SPEED100           0x04
#define REG_CONTROL_MPSO_FDX                0x02
#define REG_CONTROL_MPSO_LINKPASS           0x01

/* Power down mode register Page 0x00 Address 0x0f */
#define REG_POWER_DOWN_MODE_PORT1_PHY_DISABLE     0x01
#define REG_POWER_DOWN_MODE_PORT2_PHY_DISABLE     0x02
#define REG_POWER_DOWN_MODE_PORT3_PHY_DISABLE     0x04
#define REG_POWER_DOWN_MODE_PORT4_PHY_DISABLE     0x08
#define REG_POWER_DOWN_MODE_PORT5_PHY_DISABLE     0x10


/*---------------------------------------------------------------------*/
/* 5325 Switch SPI Interface                                           */
/* use when configuration type is BP_ENET_CONFIG_SPI_SSB_x             */
/*---------------------------------------------------------------------*/
#define BCM5325_SPI_CMD_LEN                 1
#define BCM5325_SPI_ADDR_LEN                1
#define BCM5325_SPI_PREPENDCNT              (BCM5325_SPI_CMD_LEN+BCM5325_SPI_ADDR_LEN)

/* 5325 SPI Status Register */
#define BCM5325_SPI_STS                     0xfe

/* 5325 SPI Status Register definition */
#define BCM5325_SPI_CMD_RACK                0x20

/* 5325 Command Byte definition */
#define BCM5325_SPI_CMD_READ                0x00    /* bit 0 - Read/Write */
#define BCM5325_SPI_CMD_WRITE               0x01    /* bit 0 - Read/Write */
#define BCM5325_SPI_CHIPID_MASK             0x7     /* bit 3:1 - Chip ID */
#define BCM5325_SPI_CHIPID_SHIFT            1
#define BCM5325_SPI_CMD_NORMAL              0x60    /* bit 7:4 - Mode */
#define BCM5325_SPI_CMD_FAST                0x10    /* bit 4 - Mode */

/*---------------------------------------------------------------------*/
/* 5325 Switch Pseudo PHY MII Register                                 */
/* use when configuration type is BP_ENET_CONFIG_MDIO_PSEUDO_PHY       */
/*---------------------------------------------------------------------*/
#define PSEUDO_PHY_ADDR             0x1e    /* Pseduo PHY address */

/* Pseudo PHY MII registers */
#define REG_PSEUDO_PHY_MII_REG16    0x10    /* register 16 - Switch Register Set Access Control Register */
#define REG_PSEUDO_PHY_MII_REG17    0x11    /* register 17 - Switch Register Set Read/Write Control Register */
#define REG_PSEUDO_PHY_MII_REG24    0x18    /* register 24 - Switch Accesss Register bit 15:0 */
#define REG_PSEUDO_PHY_MII_REG25    0x19    /* register 25 - Switch Accesss Register bit 31:16 */
#define REG_PSEUDO_PHY_MII_REG26    0x20    /* register 26 - Switch Accesss Register bit 47:32 */
#define REG_PSEUDO_PHY_MII_REG27    0x21    /* register 27 - Switch Accesss Register bit 63:48 */

/*Pseudo PHY MII register 16 Switch Register Set Access Control Register */
#define REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT  8       /* bit 8..15 - switch page number */
#define REG_PPM_REG16_MDIO_ENABLE               0x01    /* bit 0 - set MDC/MDIO access enable */

/*Pseudo PHY MII register 17 Switch Register Set Read/Write Control Register */
#define REG_PPM_REG17_REG_NUMBER_SHIFT          8       /* bit 8..15 - switch register number */
#define REG_PPM_REG17_OP_DONE       0x00    /* bit 0..1 - no operation */
#define REG_PPM_REG17_OP_WRITE      0x01    /* bit 0..1 - write operation */
#define REG_PPM_REG17_OP_READ       0x02    /* bit 0..1 - read operation */

#define SPI_STATUS_OK                       0
#define SPI_STATUS_INVALID_LEN              -1

extern int mii_init(bcmenet_softc *softc);
extern uint32 mii_read(volatile EmacRegisters *emac, uint32 uPhyAddr, uint32 uRegAddr);
extern void mii_write(volatile EmacRegisters *emac, uint32 uPhyAddr, uint32 uRegAddr, uint32 data);
#if defined (_BCM96338_)
extern void phy_tuning_mse(bcmenet_softc *softc);
#endif

#endif /* _BCMMII_H_ */
