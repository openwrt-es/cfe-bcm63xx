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
//**************************************************************************
// File Name  : bcmmii.c
//
// Description: Broadcom PHY/Ethernet Switch Configuration
//               
//**************************************************************************

#include "lib_types.h"
#include "lib_printf.h"
#include "cfe_timer.h"
#include "mii.h"

#include "bcm_map.h"
#include "boardparms.h"
#include "bcmmii.h"

typedef enum {
    MII_100MBIT     = 0x0001,
    MII_FULLDUPLEX  = 0x0002,
    MII_AUTONEG     = 0x0004,
}   MII_CONFIG;

#define EMAC_MDC            0x1f

/* local prototypes */
static MII_CONFIG mii_getconfig(bcmenet_softc *softc);
static MII_CONFIG mii_autoconfigure(bcmenet_softc *softc);
static void mii_setup(bcmenet_softc *softc);
static void mii_soft_reset(volatile EmacRegisters *emac, uint32 uPhyAddr);
static int spi_read( int spi_ss, unsigned char *msg_buf, int nbytes );
static int spi_write(int spi_ss, unsigned char *msg_buf, int nbytes );
static int ethsw_spi_ss_id(bcmenet_softc *softc);
static void ethsw_spi_select(bcmenet_softc *softc, int page);
static void ethsw_spi_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
static void ethsw_spi_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
static void ethsw_mdio_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
static void ethsw_mdio_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
static void ethsw_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
static void ethsw_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len);
#if defined (_BCM96338_)
static void pga_fix_enable(bcmenet_softc *softc);
#endif
#ifdef PHY_LOOPBACK
static void mii_loopback(bcmenet_softc *softc);
#endif

/* read a value from the MII */
uint32 mii_read(volatile EmacRegisters *emac, uint32 uPhyAddr, uint32 uRegAddr) 
{
    emac->mdioData = MDIO_RD | (uPhyAddr << MDIO_PMD_SHIFT) | (uRegAddr << MDIO_REG_SHIFT);
    cfe_usleep(1000);
    while ( ! (emac->intStatus & EMAC_MDIO_INT) );
    emac->intStatus |= EMAC_MDIO_INT;
    return emac->mdioData & 0xffff;
}

/* write a value to the MII */
void mii_write(volatile EmacRegisters *emac, uint32 uPhyAddr, uint32 uRegAddr, uint32 data)
{
    uint32 val;
    val = MDIO_WR | (uPhyAddr << MDIO_PMD_SHIFT) | (uRegAddr << MDIO_REG_SHIFT) | data;
    emac->mdioData = val;
    cfe_usleep(1000);
    while ( ! (emac->intStatus & EMAC_MDIO_INT) );
    emac->intStatus |= EMAC_MDIO_INT;
}

#ifdef PHY_LOOPBACK
/* set the MII loopback mode */
static void mii_loopback(bcmenet_softc *softc)
{
    uint32 val;

    TRACE(("mii_loopback\n"));

    val = mii_read(softc->emac, softc->EnetInfo.ucPhyAddress, MII_BMCR);
    /* Disable autonegotiation */
    val &= ~BMCR_ANENABLE;
    /* Enable Loopback */
    val |= BMCR_LOOPBACK;
    mii_write(softc->emac, softc->EnetInfo.ucPhyAddress, MII_BMCR, val);
}
#endif

/* return the current MII configuration */
static MII_CONFIG mii_getconfig(bcmenet_softc *softc)
{
    uint32 val;
    MII_CONFIG eConfig = 0;

    TRACE(("mii_getconfig\n"));

    /* Read the Link Partner Ability */
    val = mii_read(softc->emac, softc->EnetInfo.ucPhyAddress, MII_ANLPAR);
    if (val & ANLPAR_TXFD) {          /* 100 MB Full-Duplex */
        eConfig = (MII_100MBIT | MII_FULLDUPLEX);
    } else if (val & ANLPAR_TXHD) {   /* 100 MB Half-Duplex */
        eConfig = MII_100MBIT;
    } else if (val & ANLPAR_10FD) {   /* 10 MB Full-Duplex */
        eConfig = MII_FULLDUPLEX;
    } 

    return eConfig;
}

/* Auto-Configure this MII interface */
static MII_CONFIG mii_autoconfigure(bcmenet_softc *softc)
{
    int i;
    uint32 val;
    MII_CONFIG eConfig;

    TRACE(("mii_autoconfigure\n"));
    /* enable and restart autonegotiation */
    val = mii_read(softc->emac, softc->EnetInfo.ucPhyAddress, MII_BMCR);
    val |= (BMCR_RESTARTAN | BMCR_ANENABLE);
    mii_write(softc->emac, softc->EnetInfo.ucPhyAddress, MII_BMCR, val);

    /* wait for it to finish */
    for (i = 0; i < 1000; i++) {
        cfe_usleep(1000);
        
        if (i == 250)
        {
      	AlertLed_Off();//roy
      	}
        
        if (i == 650)
        {
      	AlertLed_On();//roy
      	}
               
        
        val = mii_read(softc->emac, softc->EnetInfo.ucPhyAddress, MII_BMSR);
        if (val & BMSR_ANCOMPLETE) {
            break;
        }
    }
    eConfig = mii_getconfig(softc);
    if (val & BMSR_ANCOMPLETE) {
        eConfig |= MII_AUTONEG;
    } 
    return eConfig;
}

static void mii_setup(bcmenet_softc *softc)
{
    MII_CONFIG eMiiConfig;
    eMiiConfig = mii_autoconfigure(softc);

    if (! (eMiiConfig & MII_AUTONEG)) {
        xprintf("Auto-negotiation timed-out\n");
    }
    if ((eMiiConfig & (MII_100MBIT | MII_FULLDUPLEX)) == (MII_100MBIT | MII_FULLDUPLEX)) {
        xprintf("100 MB Full-Duplex (auto-neg)\n");
    } else if (eMiiConfig & MII_100MBIT) {
        xprintf("100 MB Half-Duplex (auto-neg)\n");
    } else if (eMiiConfig & MII_FULLDUPLEX) {
        xprintf("10 MB Full-Duplex (auto-neg)\n");
    } else {
        xprintf("10 MB Half-Duplex (assumed)\n");
    }
#ifdef PHY_LOOPBACK
    /* Enable PHY loopback */
    mii_loopback(softc);
#endif
    /* Check for FULL/HALF duplex */
    if (eMiiConfig & MII_FULLDUPLEX) {
        softc->emac->txControl = EMAC_FD;
    }
}

/* reset the MII */
static void mii_soft_reset(volatile EmacRegisters *emac, uint32 uPhyAddr) 
{
    uint32 val;

    mii_write(emac, uPhyAddr, MII_BMCR, BMCR_RESET);
    cfe_usleep(10); /* wait ~10usec */
    do {
        val = mii_read(emac, uPhyAddr, MII_BMCR);
    } while (val & BMCR_RESET);

}

#if defined (_BCM96338_)
static void pga_fix_enable(bcmenet_softc *softc)
{
    uint32 val;
    uint32 phyaddr;

    phyaddr = softc->EnetInfo.ucPhyAddress;
    val = (MII_BRCM_TEST_HARDRESET |
           MII_BRCM_TEST_10BT_SERIAL_NODRIB |
           MII_BRCM_TEST_100TX_POWERDOWN |
           MII_BRCM_TEST_10BT_POWERDOWN );
    mii_write(softc->emac, phyaddr, MII_BRCM_TEST, val); //  reset phy
    val = mii_read(softc->emac, phyaddr, MII_BRCM_TEST);
    val |= MII_BRCM_TEST_SHADOW_ENABLE;
    mii_write(softc->emac, phyaddr, MII_BRCM_TEST, val); //  shadow mode
    val = mii_read(softc->emac, phyaddr, MII_TX_CONTROL);
    val |= MII_TX_CONTROL_PGA_FIX_ENABLE;
    mii_write(softc->emac, phyaddr, MII_TX_CONTROL, val); // pga fix enable
    val = mii_read(softc->emac, phyaddr, MII_BRCM_TEST);
    val &= ~MII_BRCM_TEST_SHADOW_ENABLE;
    mii_write(softc->emac, phyaddr, MII_BRCM_TEST, val); //  shadow mode
}

/* phy_tuning_mse */
void phy_tuning_mse(bcmenet_softc *softc) 
{
    volatile EmacRegisters *emac = softc->emac;
    uint32 phyaddr;
    uint32 val;
    int i = 0;
    int retry = 0;

    phyaddr = softc->EnetInfo.ucPhyAddress;
    val = mii_read(emac, phyaddr, MII_BRCM_TEST);
    val |= MII_BRCM_TEST_SHADOW_ENABLE;
    mii_write(emac, phyaddr, MII_BRCM_TEST, val); // enabled shadow mode

    do {
        i++;
        val = mii_read(emac, phyaddr, MII_AUX_STATUS3); // read the MSE value
        if ((val & MII_AUX_STATUS3_MSE_MASK) >= 0x4000) {
            val = mii_read(emac, phyaddr, MII_TX_CONTROL);
            val &= ~MII_TX_CONTROL_PGA_FIX_ENABLE;
            mii_write(emac, phyaddr, MII_TX_CONTROL, val); // pga fix disable
            cfe_usleep(100); /* wait ~100usec */
            val |= MII_TX_CONTROL_PGA_FIX_ENABLE;
            mii_write(emac, phyaddr, MII_TX_CONTROL, val); // pga fix enable
            i = 0;
            retry++;
        }
        if ((i > 12) || (retry > 2)) // read twelve times to ensure good 
            break;
    } while (1);

    val = mii_read(emac, phyaddr, MII_BRCM_TEST);
    val &= ~MII_BRCM_TEST_SHADOW_ENABLE;
    mii_write(emac, phyaddr, MII_BRCM_TEST, val); // disabled shadow mode
}
#endif

/* BCM5325 register access through SPI */
static int spi_read( int spi_ss, unsigned char *msg_buf, int nbytes )
{
    int i;

    SPI->spiMsgCtl = (nbytes << SPI_BYTE_CNT_SHIFT |
                            HALF_DUPLEX_R << SPI_MSG_TYPE_SHIFT);
    
    for (i = 0; i < BCM5325_SPI_PREPENDCNT; i++)
	    SPI->spiMsgData[i] = msg_buf[i];

    SPI->spiCmd = (SPI_CMD_START_IMMEDIATE << SPI_CMD_COMMAND_SHIFT | 
                    spi_ss << SPI_CMD_DEVICE_ID_SHIFT | 
                    BCM5325_SPI_PREPENDCNT << SPI_CMD_PREPEND_BYTE_CNT_SHIFT );

    while (!(SPI->spiIntStatus & SPI_INTR_CMD_DONE));

    SPI->spiIntStatus = SPI_INTR_CLEAR_ALL;

    for(i = 0; i < nbytes; i++) {
	    msg_buf[i] = SPI->spiRxDataFifo[i];
    }

    return SPI_STATUS_OK;
}

static int spi_write(int spi_ss, unsigned char *msg_buf, int nbytes )
{
    int i;
    
    if (nbytes < BCM5325_SPI_PREPENDCNT) {
        return SPI_STATUS_INVALID_LEN;
    }

    SPI->spiMsgCtl = (nbytes << SPI_BYTE_CNT_SHIFT |
                    HALF_DUPLEX_W << SPI_MSG_TYPE_SHIFT);

    for (i = 0; i < nbytes; i++)
	    SPI->spiMsgData[i] = msg_buf[i];

    SPI->spiCmd = (SPI_CMD_START_IMMEDIATE << SPI_CMD_COMMAND_SHIFT | 
                    spi_ss << SPI_CMD_DEVICE_ID_SHIFT | 
                    0 << SPI_CMD_PREPEND_BYTE_CNT_SHIFT );

    while (!(SPI->spiIntStatus & SPI_INTR_CMD_DONE));

    SPI->spiIntStatus = SPI_INTR_CLEAR_ALL;

    return SPI_STATUS_OK;
}

static int ethsw_spi_ss_id(bcmenet_softc *softc)
{
    int slave_select;

    switch(softc->EnetInfo.usConfigType) {
        case BP_ENET_CONFIG_SPI_SSB_0:
            slave_select = SPI_DEV_ID_0;
            break;
        case BP_ENET_CONFIG_SPI_SSB_1:
            slave_select = SPI_DEV_ID_1;
            break;
        case BP_ENET_CONFIG_SPI_SSB_2:
            slave_select = SPI_DEV_ID_2;
            break;
        case BP_ENET_CONFIG_SPI_SSB_3:
            slave_select = SPI_DEV_ID_3;
            break;
        default:
            slave_select = SPI_DEV_ID_1;
            break;
    }
    return slave_select;
}

static void ethsw_spi_select(bcmenet_softc *softc, int page)
{
    unsigned char buf[3];
    int spi_ss;
    ethsw_info_t *ethsw = &softc->ethSwitch;

    if (ethsw->cid == softc->EnetInfo.ucPhyAddress && ethsw->page == page)
        return;

    spi_ss = ethsw_spi_ss_id(softc);
    ethsw->cid = softc->EnetInfo.ucPhyAddress;
    ethsw->page = page;
    /* Select new chip */
    buf[0] = BCM5325_SPI_CMD_NORMAL | BCM5325_SPI_CMD_WRITE |
        ((ethsw->cid & BCM5325_SPI_CHIPID_MASK) << BCM5325_SPI_CHIPID_SHIFT);

    /* Select new page */
    buf[1] = PAGE_SELECT;
    buf[2] = (char)ethsw->page;
    spi_write(spi_ss, buf, sizeof(buf));
}

static void ethsw_spi_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    unsigned char buf[64];
    int rc;
    int i;
    int max_check_spi_sts;
    int spi_ss;

    ethsw_spi_select(softc, page);

    spi_ss = ethsw_spi_ss_id(softc);
    /* write command byte and register address */
    buf[0] = BCM5325_SPI_CMD_NORMAL | BCM5325_SPI_CMD_READ |
        ((softc->ethSwitch.cid & BCM5325_SPI_CHIPID_MASK) << BCM5325_SPI_CHIPID_SHIFT);
    buf[1] = (unsigned char)reg;
    rc = spi_read(spi_ss, buf, 1);

    if (rc == SPI_STATUS_OK) {
        max_check_spi_sts = 0;
        do {
            /* write command byte and read spi_sts address */
            buf[0] = BCM5325_SPI_CMD_NORMAL | BCM5325_SPI_CMD_READ |
                ((softc->ethSwitch.cid & BCM5325_SPI_CHIPID_MASK) << BCM5325_SPI_CHIPID_SHIFT);
            buf[1] = (unsigned char)BCM5325_SPI_STS;
            rc = spi_read(spi_ss, buf, 1);
            if (rc == SPI_STATUS_OK) {
                /* check the bit 0 RACK bit is set */
                if (buf[0] & BCM5325_SPI_CMD_RACK) {
                    break;
                }
                cfe_usleep(1000);
            } else {
                break;
            }
        } while (max_check_spi_sts++ < 10);

        if (rc == SPI_STATUS_OK) {
            for (i = len-1; i >= 0; i--) {
                buf[0] = BCM5325_SPI_CMD_NORMAL | BCM5325_SPI_CMD_READ |
                    ((softc->ethSwitch.cid & BCM5325_SPI_CHIPID_MASK) << BCM5325_SPI_CHIPID_SHIFT);
                buf[1] = (unsigned char)0xf0;
                rc = spi_read(spi_ss, buf, 1);
                if (rc == SPI_STATUS_OK) {
                    /* Write the data out in LE format to the switch */
                    *(data + len - i - 1) = buf[0];
                }
            }
        }
   }
}

static void ethsw_spi_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    unsigned char buf[64];
    int i;
    int spi_ss;

    ethsw_spi_select(softc, page);

    spi_ss = ethsw_spi_ss_id(softc);
    buf[0] = BCM5325_SPI_CMD_NORMAL | BCM5325_SPI_CMD_WRITE |
        ((softc->ethSwitch.cid & BCM5325_SPI_CHIPID_MASK) << BCM5325_SPI_CHIPID_SHIFT);

    buf[1] = (char)reg;
    for (i = 0; i < len; i++) {
        /* Write the data out in LE format to the switch */
        buf[BCM5325_SPI_PREPENDCNT+i] = *(data + (len - i - 1));
    }
    spi_write( spi_ss, buf, len+BCM5325_SPI_PREPENDCNT);
}

/* BCM5325E register access through MDC/MDIO */
static void ethsw_mdio_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    volatile EmacRegisters *emac; 
    uint32 cmd, res, ret;
    int max_retry = 0;

    emac = softc->emac;
    cmd = (page << REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT) | REG_PPM_REG16_MDIO_ENABLE;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG16, cmd);

    cmd = (reg << REG_PPM_REG17_REG_NUMBER_SHIFT) | REG_PPM_REG17_OP_READ;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17, cmd);

    do {
        res = mii_read(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17);
        cfe_usleep(10);
    } while (((res & (REG_PPM_REG17_OP_WRITE|REG_PPM_REG17_OP_READ)) != REG_PPM_REG17_OP_DONE) &&
              (max_retry++ < 5));

    ret = 0;
    ret |= mii_read(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG24) << 0;
    ret |= mii_read(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG25) << 16;
    switch (len) {
        case 1:
            *data = (uint8)ret;
            break;
        case 2:
            *(uint16 *)data = (uint16)ret;
            break;
        case 4:
            *(uint32 *)data = ret;
            break;
    }
}

static void ethsw_mdio_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    volatile EmacRegisters *emac; 
    uint32 cmd, res;
    uint32 val = 0;
    int max_retry = 0;

    emac = softc->emac;
    switch (len) {
        case 1:
            val = *data;
            break;
        case 2:
            val = *(uint16 *)data;
            break;
        case 4:
            val = *(uint32 *)data;
            break;
    }
    cmd = (page << REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT) | REG_PPM_REG16_MDIO_ENABLE;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG16, cmd);

    cmd = val>>0 & 0xffff;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG24, cmd);
    cmd = val>>16 & 0xffff;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG25, cmd);
    cmd = 0;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG26, cmd);
    cmd = 0;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG27, cmd);

    cmd = (reg << REG_PPM_REG17_REG_NUMBER_SHIFT) | REG_PPM_REG17_OP_WRITE;
    mii_write(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17, cmd);

    do {
        res = mii_read(emac, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17);
        cfe_usleep(10);
    } while (((res & (REG_PPM_REG17_OP_WRITE|REG_PPM_REG17_OP_READ)) != REG_PPM_REG17_OP_DONE) &&
                (max_retry++ < 5));
}

static void ethsw_rreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    switch (softc->EnetInfo.usConfigType) {
        case BP_ENET_CONFIG_MDIO_PSEUDO_PHY:
            ethsw_mdio_rreg(softc, page, reg, data, len);
            break;

        case BP_ENET_CONFIG_SPI_SSB_0:
        case BP_ENET_CONFIG_SPI_SSB_1:
        case BP_ENET_CONFIG_SPI_SSB_2:
        case BP_ENET_CONFIG_SPI_SSB_3:
            ethsw_spi_rreg(softc, page, reg, data, len);
            break;
    }
}

static void ethsw_wreg(bcmenet_softc *softc, int page, int reg, uint8 *data, int len)
{
    switch (softc->EnetInfo.usConfigType) {
        case BP_ENET_CONFIG_MDIO_PSEUDO_PHY:
            ethsw_mdio_wreg(softc, page, reg, data, len);
            break;

        case BP_ENET_CONFIG_SPI_SSB_0:
        case BP_ENET_CONFIG_SPI_SSB_1:
        case BP_ENET_CONFIG_SPI_SSB_2:
        case BP_ENET_CONFIG_SPI_SSB_3:
            ethsw_spi_wreg(softc, page, reg, data, len);
            break;
    }
}

int mii_init(bcmenet_softc *softc)
{
    volatile EmacRegisters *emac;
    uint32 data32;
    uint16 data16;
    uint16 blkEnables;
    uint8 data8;
    int i;

    emac = softc->emac;
    switch(softc->EnetInfo.ucPhyType) {

        case BP_ENET_INTERNAL_PHY:
            /* init mii clock, do soft reset of phy, default is 10Base-T */
            emac->mdioFreq = EMAC_MII_PRE_EN | EMAC_MDC;
            /* reset phy */
            mii_soft_reset(emac, softc->EnetInfo.ucPhyAddress);

#if defined(_BCM96338_)
            pga_fix_enable(softc);
#endif
            mii_setup(softc);
#if defined(_BCM96338_)
            phy_tuning_mse(softc);
#endif
            break;

        case BP_ENET_EXTERNAL_PHY:
            emac->config |= EMAC_EXT_PHY;
            emac->mdioFreq = EMAC_MII_PRE_EN | EMAC_MDC;
            /* reset phy */
            if (softc->EnetInfo.usGpioPhyReset != BP_NOT_DEFINED) {
                data16 = GPIO_NUM_TO_MASK(softc->EnetInfo.usGpioPhyReset);
                GPIO->GPIODir |= data16;
                if (softc->EnetInfo.usGpioPhyReset & BP_ACTIVE_LOW) {
                    GPIO->GPIOio &= ~data16;
                    cfe_usleep(400); /* hold > 150us */
                    GPIO->GPIOio |= data16;
                }
                else {
                    GPIO->GPIOio |= data16;
                    cfe_usleep(400); /* hold > 150us */
                    GPIO->GPIOio &= ~data16;
                }
                cfe_usleep(1100000); /* wait > 1 second */
            } else {
                mii_soft_reset(emac, softc->EnetInfo.ucPhyAddress);
            }

            data32 = mii_read(softc->emac, softc->EnetInfo.ucPhyAddress, MII_ANAR);
            data32 |= ADVERTISE_FDFC; /* advertise flow control capbility */
            mii_write(softc->emac, softc->EnetInfo.ucPhyAddress, MII_ANAR, data32);

            mii_setup(softc);
            break;

        case BP_ENET_EXTERNAL_SWITCH:
            emac->config |= EMAC_EXT_PHY;
            emac->mdioFreq = EMAC_MII_PRE_EN | EMAC_MDC;
            emac->txControl = EMAC_FD;
            switch (softc->EnetInfo.usConfigType) {
                case BP_ENET_CONFIG_MDIO_PSEUDO_PHY:
                    mii_soft_reset(emac, PSEUDO_PHY_ADDR);
                    break;

                case BP_ENET_CONFIG_SPI_SSB_0:
                case BP_ENET_CONFIG_SPI_SSB_1:
                case BP_ENET_CONFIG_SPI_SSB_2:
                case BP_ENET_CONFIG_SPI_SSB_3:
                    blkEnables = PERF->blkEnables;
                    if ((blkEnables & SPI_CLK_EN) == 0) {
                        blkEnables |= SPI_CLK_EN;
                        PERF->blkEnables = blkEnables;
                    }
                    softc->ethSwitch.cid = 0xff;
                    softc->ethSwitch.page = 0xff;
                    break;

                case BP_ENET_CONFIG_MDIO:
                    /* reset phy */
                    if (softc->EnetInfo.numSwitchPorts) {
                        for (i = 0; i < softc->EnetInfo.numSwitchPorts; i++) {
                            mii_soft_reset(emac, softc->EnetInfo.ucPhyAddress | i);
                        }
                    }
                    return 0;

                default:
                    xprintf("Unknown PHY configuration type\n");
                    break;
            }
            if (softc->EnetInfo.numSwitchPorts) {
                data8 = 0;
                switch (softc->EnetInfo.numSwitchPorts) {
                    case 5:
                        data8 |= REG_POWER_DOWN_MODE_PORT5_PHY_DISABLE;
                    case 4:
                        data8 |= REG_POWER_DOWN_MODE_PORT4_PHY_DISABLE;
                    case 3:
                        data8 |= REG_POWER_DOWN_MODE_PORT3_PHY_DISABLE;
                    case 2:
                        data8 |= REG_POWER_DOWN_MODE_PORT2_PHY_DISABLE;
                    case 1:
                        data8 |= REG_POWER_DOWN_MODE_PORT1_PHY_DISABLE;
                        break;
                    default:
                        break;
                }
                /* disable Switch port PHY */
                ethsw_wreg(softc, PAGE_CONTROL, REG_POWER_DOWN_MODE, (uint8 *)&data8, sizeof(data8));
                /* enable Switch port PHY */
                data8 &= ~data8;
                ethsw_wreg(softc, PAGE_CONTROL, REG_POWER_DOWN_MODE, (uint8 *)&data8, sizeof(data8));
            }
            /* setup Switch MII1 port state override */
            ethsw_rreg(softc, PAGE_CONTROL, REG_CONTROL_MII1_PORT_STATE_OVERRIDE, &data8, sizeof(data8));

            if (softc->EnetInfo.usReverseMii == BP_ENET_REVERSE_MII)
                data8 |= REG_CONTROL_MPSO_REVERSE_MII;
            data8 |= (REG_CONTROL_MPSO_MII_SW_OVERRIDE|REG_CONTROL_MPSO_LINKPASS);
            data8 |= (REG_CONTROL_MPSO_LP_FLOW_CONTROL|REG_CONTROL_MPSO_SPEED100|REG_CONTROL_MPSO_FDX);

            ethsw_wreg(softc, PAGE_CONTROL, REG_CONTROL_MII1_PORT_STATE_OVERRIDE, &data8, sizeof(data8));

            /* checking Switch functional */
            data8 = 0;
            ethsw_rreg(softc, PAGE_CONTROL, REG_CONTROL_MII1_PORT_STATE_OVERRIDE, &data8, sizeof(data8));
            if ((data8 & (REG_CONTROL_MPSO_MII_SW_OVERRIDE|REG_CONTROL_MPSO_LINKPASS)) !=
                (REG_CONTROL_MPSO_MII_SW_OVERRIDE|REG_CONTROL_MPSO_LINKPASS)) {
                    xprintf("error on Ethernet Switch setup\n");
                    return -1;
            }
            if (softc->EnetInfo.usReverseMii == BP_ENET_REVERSE_MII) {
                if ((data8 & REG_CONTROL_MPSO_REVERSE_MII) != REG_CONTROL_MPSO_REVERSE_MII) {
                    xprintf("error on Ethernet Switch reverse MII setup\n");
                    return -1;
                }
            }

            break;

        default:
            xprintf("Unknown PHY type\n");
            break;
    }
    return 0;
}
