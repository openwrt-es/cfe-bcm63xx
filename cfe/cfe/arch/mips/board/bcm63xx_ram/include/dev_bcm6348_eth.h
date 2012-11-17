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
#ifndef __BCM6348_ETH_H
#define __BCM6348_ETH_H

#include "board.h"
#include "bcm_map.h"
#include "bcm_common.h"
#include "boardparms.h"
extern void getMacAddress(unsigned char *hwaddr);

// from linux if_ether.h
#define ETH_ALEN                6       /* Octets in one ethernet addr	 */
#define ETH_HLEN                14      /* Total octets in header.	 */
#define ETH_ZLEN                60      /* Min. octets in frame sans FCS */
#define ETH_DATA_LEN            1500    /* Max. octets in payload	 */
#define ETH_CRC_LEN	            4       /* CRC length */
// end if_ether.h

/*---------------------------------------------------------------------*/
/* specify number of BDs and buffers to use                            */
/*---------------------------------------------------------------------*/
#define NR_TX_BDS               20
#define NR_RX_BDS               20
#define ENET_MAX_MTU_SIZE       1518    /* Body(1500) + EH_SIZE(14) + FCS(4) */
#define DMA_MAX_BURST_LENGTH    16      /* in 32 bit words */
#define ENET_BUF_SIZE           ((ENET_MAX_MTU_SIZE + (DMA_MAX_BURST_LENGTH*4)) & ~((DMA_MAX_BURST_LENGTH*4)-1))
#define DMA_FC_THRESH_LO        5
#define DMA_FC_THRESH_HI        10
#define EMAC_TX_WATERMARK       32

#define MAKE4(x)   ((x[3] & 0xFF) | ((x[2] & 0xFF) << 8) | ((x[1] & 0xFF) << 16) | ((x[0] & 0xFF) << 24))
#define MAKE2(x)   ((x[1] & 0xFF) | ((x[0] & 0xFF) << 8))


#define ERROR(x)        xsprintf x
#ifndef ASSERT
#define ASSERT(x)       if (x); else ERROR(("assert: "__FILE__" line %d\n", __LINE__)); 
#endif

//#define DUMP_TRACE

#if defined(DUMP_TRACE)
#define TRACE (x)         xprintf x
#else
#define TRACE(x)
#endif

typedef struct PM_Addr {
    uint16              valid;          /* 1 indicates the corresponding address is valid */
    unsigned char       dAddr[ETH_ALEN];/* perfect match register's destination address */
    unsigned int        ref;            /* reference count */
} PM_Addr;					 
#define MAX_PMADDR      4               /* # of perfect match address */

#define NUM_CHANS_PER_EMAC          2
#define NUM_EMAC                    2
#define EMAC1_RX_CHAN               0
#define EMAC1_TX_CHAN               1
#define EMAC2_RX_CHAN               2
#define EMAC2_TX_CHAN               3

typedef struct gpio_reg_addrs_t {
    volatile uint16 *gpio_direction_reg;/* GPIO direction register */
    volatile uint16 *gpio_value_reg;    /* GPIO value register */
} gpio_reg_addrs_t;

typedef struct ethsw_info_t {
    gpio_reg_addrs_t sbh;
    uint32 ssl, clk, mosi, miso;        /* GPIO mapping */
    int cid, page;                      /* Current chip ID and page */
} ethsw_info_t;

typedef struct bcmenet_softc {

    volatile DmaRegs *dmaCtrl;
    
    /* transmit variables */    
    volatile DmaChannelCfg *txDma;      /* location of transmit DMA register set */
    volatile DmaDesc *txBds;            /* Memory location of tx Dma BD ring */
    volatile DmaDesc *txFirstBdPtr;     /* ptr to first allocated Tx BD */
    volatile DmaDesc *txNextBdPtr;      /* ptr to next Tx BD to transmit with */
    volatile DmaDesc *txLastBdPtr;      /* ptr to last allocated Tx BD */

    /* receive variables */
    volatile DmaChannelCfg *rxDma;      /* location of receive DMA register set */
    volatile DmaDesc *rxBds;            /* Memory location of rx bd ring */
    volatile DmaDesc *rxFirstBdPtr;     /* ptr to first allocated rx bd */
    volatile DmaDesc *rxBdReadPtr;      /* ptr to next rx bd to be processed */
    volatile DmaDesc *rxLastBdPtr;      /* ptr to last allocated rx bd */

    uint32_t rxBuffers;
    uint32_t txBuffers;

    uint16          chipId;             /* chip's id */
    uint16          chipRev;            /* step */
    uint8_t         hwaddr[ETH_ALEN];
    volatile EmacRegisters *emac;       /* EMAC register base address */
    ethsw_info_t    ethSwitch;          /* external switch */
    ETHERNET_MAC_INFO EnetInfo;
} bcmenet_softc;



#define IncRxBdPtr(x, s) if (x == ((bcmenet_softc *)s)->rxLastBdPtr) \
                             x = ((bcmenet_softc *)s)->rxBds; \
                      else x++
#define InctxBdPtr(x, s) if (x == ((bcmenet_softc *)s)->txLastBdPtr) \
                             x = ((bcmenet_softc *)s)->txBds; \
                      else x++

// extern and function prototype

extern int32_t _getticks(void);

#ifdef DUMP_DATA
static void hexdump( unsigned char * src, int srclen, int rowlen, int rows );
#endif

#endif // __BCM6348_ETH_H
