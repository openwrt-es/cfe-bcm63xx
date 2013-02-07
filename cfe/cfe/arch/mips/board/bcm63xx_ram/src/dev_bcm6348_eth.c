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

/** Includes. **/
#include "lib_types.h"
#include "lib_malloc.h"
#include "lib_string.h"
#include "lib_printf.h"

#include "cfe_iocb.h"
#include "cfe_ioctl.h"
#include "cfe_device.h"
#include "cfe_devfuncs.h"
#include "sbmips.h"
#include "cfe_timer.h"
#include "dev_bcm6348_eth.h"
#include "dev_bcm63xx_flash.h"
#include "mii.h"
#include "bcmmii.h"


/* Define the desired EMAC that the CFE boot loader uses. */
//#define ENET_USE_EMAC1
#define ENET_USE_EMAC2

#if defined(_BCM96338_) || defined(ENET_USE_EMAC1)
/* CFE Ethernet use EMAC 1 */
#define EMAC                EMAC1
#define EMAC_TX_CHAN        EMAC1_TX_CHAN
#define EMAC_RX_CHAN        EMAC1_RX_CHAN
#define EMAC_INFO_INDEX     0
#else
/* CFE Ethernet use EMAC 2 */
#define EMAC                EMAC2
#define EMAC_TX_CHAN        EMAC2_TX_CHAN
#define EMAC_RX_CHAN        EMAC2_RX_CHAN
#define EMAC_INFO_INDEX     1
#endif

#define CACHE_ALIGN     16
extern void _cfe_flushcache(int, uint8_t *, uint8_t *);
#define INVAL_RANGE(s,l) _cfe_flushcache(CFE_CACHE_INVAL_RANGE,((uint8_t *) (s)),((uint8_t *) (s))+(l))
#define FLUSH_RANGE(s,l) _cfe_flushcache(CFE_CACHE_FLUSH_RANGE,((uint8_t *) (s)),((uint8_t *) (s))+(l))

/** Externs. **/
extern unsigned long sysGetEnetModeFlag(void);


/** Prototypes. **/
static void bcm6348_ether_probe( cfe_driver_t * drv,    unsigned long probe_a,
                                 unsigned long probe_b, void * probe_ptr );
static int bcm6348_ether_open(cfe_devctx_t *ctx);
static int bcm6348_ether_read(cfe_devctx_t *ctx,iocb_buffer_t *buffer);
static int bcm6348_ether_inpstat(cfe_devctx_t *ctx,iocb_inpstat_t *inpstat);
static int bcm6348_ether_write(cfe_devctx_t *ctx,iocb_buffer_t *buffer);
static int bcm6348_ether_ioctl(cfe_devctx_t *ctx,iocb_buffer_t *buffer);
static int bcm6348_ether_close(cfe_devctx_t *ctx);
static void bcm6348_write_mac_address(bcmenet_softc *softc, unsigned char *macAddr);
static void delay_t(int ticks);
static int internal_open(bcmenet_softc * softc);


/** Variables. **/
const static cfe_devdisp_t bcm6348_ether_dispatch = {
    bcm6348_ether_open,
    bcm6348_ether_read,
    bcm6348_ether_inpstat,
    bcm6348_ether_write,
    bcm6348_ether_ioctl,
    bcm6348_ether_close,
    NULL,
    NULL
};

const cfe_driver_t bcm6348_enet = {
#if defined (_BCM96338_)
    "BCM6338 Ethernet",
#endif
#if defined (_BCM96348_)
    "BCM6348 Ethernet",
#endif
#if defined (_BCM96358_)
    "BCM6358 Ethernet",
#endif
    "eth",
    CFE_DEV_NETWORK,
    &bcm6348_ether_dispatch,
    bcm6348_ether_probe
};

/** Functions. **/
static void bcm6348_ether_probe( cfe_driver_t * drv,    unsigned long probe_a,
                                unsigned long probe_b, void * probe_ptr )
{
    bcmenet_softc * softc;
    char descr[100];
    softc = (bcmenet_softc *) KMALLOC( sizeof(bcmenet_softc), CACHE_ALIGN );
    if( softc == NULL ) {
        xprintf( "BCM6348 : Failed to allocate softc memory.\n" );
    } else {
        memset( softc, 0, sizeof(bcmenet_softc) );

        if (internal_open( softc ) == -1) 
            xprintf("Failed initializing enet hardware\n");
        else
        {
            cfe_attach( drv, softc, NULL, descr );
        }
    }
}

static int bcm6348_ether_open(cfe_devctx_t *ctx)
{
    /* FIXME -- See if this can be returned to its normal place. */
    return 0;
}

/*
 * init_emac: Initializes the Ethernet control registers
 */
static int init_emac(bcmenet_softc *softc)
{
    ETHERNET_MAC_INFO EnetInfos[BP_MAX_ENET_MACS];
#if defined(_BCM96348_)
    uint32 gpiomode;
#endif

    /* disable ethernet MAC while updating its registers */
    EMAC->config = EMAC_DISABLE ;           
    while(EMAC->config & EMAC_DISABLE);     

    /* issue soft reset, wait for it to complete */
    EMAC->config = EMAC_SOFT_RESET;
    while (EMAC->config & EMAC_SOFT_RESET);

    BpGetEthernetMacInfo( EnetInfos, BP_MAX_ENET_MACS );
    memcpy(&softc->EnetInfo,&EnetInfos[EMAC_INFO_INDEX],sizeof(softc->EnetInfo));

    softc->emac = EMAC;
#if defined(_BCM96348_)
    if (softc->emac == EMAC2) {
        if (softc->EnetInfo.ucPhyType != BP_ENET_INTERNAL_PHY) {
            gpiomode = GPIO->GPIOMode;
            gpiomode |= (GROUP3_EXT_MII|GROUP0_EXT_MII); /* External MII */
            if ((softc->EnetInfo.usConfigType == BP_ENET_CONFIG_SPI_SSB_1) ||
                (softc->EnetInfo.usConfigType == BP_ENET_CONFIG_SPI_SSB_2) ||
                (softc->EnetInfo.usConfigType == BP_ENET_CONFIG_SPI_SSB_3)) {
                gpiomode &= ~GROUP1_SPI_MASTER;
                gpiomode |= GROUP1_SPI_MASTER;
                }
            GPIO->GPIOMode = gpiomode;
        }
    }
#endif
    if (mii_init(softc))
        return -1;

    /* Initialize emac registers */
    EMAC->rxControl = EMAC_FC_EN; //  //EMAC_PROM;   NO Promiscuous > 5.0

#ifdef MAC_LOOPBACK
    EMAC->rxControl |= EMAC_LOOPBACK;
#endif
    EMAC->rxMaxLength = ENET_MAX_MTU_SIZE;
    EMAC->txMaxLength = ENET_MAX_MTU_SIZE;

    /* tx threshold = abs(63-(0.63*DMA_MAX_BURST_LENGTH)) */
    EMAC->txThreshold = EMAC_TX_WATERMARK;
    EMAC->mibControl  = EMAC_NO_CLEAR;  /* clear MIBs on read */
    EMAC->intMask = 0;                  /* mask all EMAC interrupts*/

    EMAC->config |= EMAC_ENABLE;  

    softc->dmaCtrl->controller_cfg |= DMA_MASTER_EN;
    return 0;
}
/*
 * init_dma: Initialize DMA control register
 */
static void init_dma(bcmenet_softc *softc)
{
    uint32 *StateRam;
    int i;

    /*
     * clear State RAM
     */
    StateRam = (UINT32 *)&softc->dmaCtrl->stram.s[0];
    for (i = 0; i < sizeof(DmaStateRam) / sizeof(UINT32) * NUM_CHANS_PER_EMAC * NUM_EMAC; i++)
        StateRam[i] = 0;

    /*
     * initialize IUDMA controller register
     */
    softc->dmaCtrl->controller_cfg = DMA_FLOWC_CH1_EN;
    softc->dmaCtrl->flowctl_ch1_thresh_lo = DMA_FC_THRESH_LO;
    softc->dmaCtrl->flowctl_ch1_thresh_hi = DMA_FC_THRESH_HI;
    softc->dmaCtrl->flowctl_ch1_alloc = 0;

	// transmit
    softc->txDma->cfg = 0;       /* initialize first (will enable later) */
    softc->txDma->maxBurst = DMA_MAX_BURST_LENGTH;
    softc->txDma->intMask = 0;   /* mask all ints */
    /* clr any pending interrupts on channel */
    softc->txDma->intStat = DMA_DONE|DMA_NO_DESC|DMA_BUFF_DONE;
    softc->txDma->intMask = DMA_DONE;
    softc->dmaCtrl->stram.s[EMAC_TX_CHAN].baseDescPtr = (uint32)K1_TO_PHYS((uint32_t)(softc->txFirstBdPtr));

    // receive
    softc->rxDma->cfg = 0;  // initialize first (will enable later)
    softc->rxDma->maxBurst = DMA_MAX_BURST_LENGTH;
    softc->rxDma->intMask = 0;   /* mask all ints */
    /* clr any pending interrupts on channel */
    softc->rxDma->intStat = DMA_DONE|DMA_NO_DESC|DMA_BUFF_DONE;
    softc->rxDma->intMask = DMA_DONE;
    softc->dmaCtrl->stram.s[EMAC_RX_CHAN].baseDescPtr = (uint32)K1_TO_PHYS((uint32_t)(softc->rxFirstBdPtr));
}


static int internal_open(bcmenet_softc * softc)
{
    int i;
    void *p;

    /* make sure emac clock is on */
    PERF->blkEnables |= EMAC_CLK_EN;

    /* init rx/tx dma channels */
    softc->dmaCtrl = (DmaRegs *)(EMAC_DMA_BASE);
    softc->rxDma = &softc->dmaCtrl->chcfg[EMAC_RX_CHAN];
    softc->txDma = &softc->dmaCtrl->chcfg[EMAC_TX_CHAN];

    p = KMALLOC( NR_TX_BDS * sizeof(DmaDesc), CACHE_ALIGN );
    if( p == NULL ) {
        xprintf( "BCM6348 : Failed to allocate txBds memory.\n" );
        return -1;
    }
    INVAL_RANGE(p, NR_TX_BDS * sizeof(DmaDesc));
    softc->txBds = (DmaDesc *)K0_TO_K1((uint32) p);

    p = KMALLOC( NR_RX_BDS * sizeof(DmaDesc), CACHE_ALIGN );
    if( p== NULL ) {
        xprintf( "BCM6348 : Failed to allocate rxBds memory.\n" );
        KFREE( (void *)(softc->txBds) );
        softc->txBds = NULL;
        return -1;
    }
    INVAL_RANGE(p, NR_RX_BDS * sizeof(DmaDesc));
    softc->rxBds = (DmaDesc *)K0_TO_K1((uint32) p);

    softc->rxBuffers = (uint32_t)KMALLOC( NR_RX_BDS * ENET_BUF_SIZE, CACHE_ALIGN );
    if( softc->rxBuffers == NULL ) {
        xprintf( "BCM6348 : Failed to allocate RxBuffer memory.\n" );
        KFREE( (void *)(softc->txBds) );
        softc->txBds = NULL;
        KFREE( (void *)(softc->rxBds) );
        softc->rxBds = NULL;
        return -1;
    }
    INVAL_RANGE(softc->rxBuffers, NR_RX_BDS * ENET_BUF_SIZE);

    softc->txBuffers = (uint32_t)KMALLOC( NR_TX_BDS * ENET_BUF_SIZE, CACHE_ALIGN );
    if( softc->txBuffers == NULL ) {
        xprintf( "BCM6348 : Failed to allocate txBuffer memory.\n" );
        KFREE( (void *)(softc->rxBuffers) );
        softc->rxBuffers = NULL;
        KFREE( (void *)(softc->txBds) );
        softc->txBds     = NULL;
        KFREE( (void *)(softc->rxBds) );
        softc->rxBds     = NULL;
        return -1;
    }
    INVAL_RANGE(softc->txBuffers, NR_TX_BDS * ENET_BUF_SIZE);

    /* Init the Receive Buffer Descriptor Ring. */
    softc->rxFirstBdPtr = softc->rxBdReadPtr = softc->rxBds;     
    softc->rxLastBdPtr = softc->rxBds + NR_RX_BDS - 1;

    for(i = 0; i < NR_RX_BDS; i++) {
        (softc->rxBds + i)->status  = DMA_OWN;
        (softc->rxBds + i)->length  = ENET_BUF_SIZE;
        (softc->rxBds + i)->address = softc->rxBuffers + i * ENET_BUF_SIZE;
        (softc->rxBds + i)->address = K1_TO_PHYS( (softc->rxBds + i)->address );
        softc->dmaCtrl->flowctl_ch1_alloc = 1;
    }
    softc->rxLastBdPtr->status |= DMA_WRAP;

    /* Init Transmit Buffer Descriptor Ring. */
    softc->txFirstBdPtr = softc->txNextBdPtr =  softc->txBds;
    softc->txLastBdPtr = softc->txBds + NR_TX_BDS - 1;

    for(i = 0; i < NR_TX_BDS; i++) {
        (softc->txBds + i)->status  = 0;
        (softc->txBds + i)->length  = 0;
        (softc->txBds + i)->address = softc->txBuffers + i * ENET_BUF_SIZE;
        (softc->txBds + i)->address = K1_TO_PHYS( (softc->txBds + i)->address );
    }
    softc->txLastBdPtr->status = DMA_WRAP;

    /* init dma registers */
    init_dma(softc);

    /* init enet control registers */
    if (init_emac(softc))
        return -1;

    getMacAddress(softc->hwaddr);

    bcm6348_write_mac_address( softc, softc->hwaddr);

    softc->rxDma->cfg |= DMA_ENABLE;

    return 0;
}

static int bcm6348_ether_read( cfe_devctx_t * ctx, iocb_buffer_t * buffer )
{
    unsigned char * dstptr;
    unsigned char * srcptr;
    volatile DmaDesc * CurrentBdPtr;
    bcmenet_softc * softc = (bcmenet_softc *) ctx->dev_softc;
    uint16 dmaFlag;

    if( ctx == NULL ) {
        xprintf( "No context\n" );
        return -1;
    }

    if( buffer == NULL ) {
        xprintf( "No dst buffer\n" );
        return -1;
    }

    if( buffer->buf_length > ENET_MAX_MTU_SIZE ) {
        xprintf( "dst buffer too small.\n" );
        xprintf( "actual size is %d\n", buffer->buf_length );
        return -1;
    }

    if( softc == NULL ) {
        xprintf( "softc has not been initialized.\n" );
        return -1;
    }

    dmaFlag = (uint16) softc->rxBdReadPtr->status;
    if (!(dmaFlag & DMA_EOP))
    {
        xprintf("dmaFlag (return -1)[%04x]\n", dmaFlag);
        return -1;
    }

    dstptr       = buffer->buf_ptr;
    CurrentBdPtr = softc->rxBdReadPtr;

    srcptr = (unsigned char *)( PHYS_TO_K1(CurrentBdPtr->address) );

    buffer->buf_retlen = CurrentBdPtr->length;
    memcpy( dstptr, srcptr, buffer->buf_retlen );

    CurrentBdPtr->length = ENET_BUF_SIZE;
    CurrentBdPtr->status &= DMA_WRAP;
    CurrentBdPtr->status |= DMA_OWN;

    IncRxBdPtr( CurrentBdPtr, softc );
    softc->rxBdReadPtr = CurrentBdPtr;
    softc->dmaCtrl->flowctl_ch1_alloc = 1;

    // enable rx dma
    softc->rxDma->cfg = DMA_ENABLE;
    return 0;
}


static int bcm6348_ether_inpstat( cfe_devctx_t * ctx, iocb_inpstat_t * inpstat )
{
    bcmenet_softc    * softc;
    volatile DmaDesc * CurrentBdPtr;

    /* ============================= ASSERTIONS ============================= */

    if( ctx == NULL ) {
        xprintf( "No context\n" );
        return -1;
    }

    if( inpstat == NULL ) {
        xprintf( "No inpstat buffer\n" );
        return -1;
    }

    softc = (bcmenet_softc *)ctx->dev_softc;
    if( softc == NULL ) {
        xprintf( "softc has not been initialized.\n" );
        return -1;
    }

    /* ====================================================================== */


    CurrentBdPtr = softc->rxBdReadPtr;

    // inp_status == 1 -> data available
    inpstat->inp_status = (CurrentBdPtr->status & DMA_OWN) ? 0 : 1;

    return 0;
}


static int bcm6348_ether_write(cfe_devctx_t *ctx,iocb_buffer_t *buffer)
{
    uint32_t status;
    unsigned char * dstptr;
    bcmenet_softc * softc;
    volatile DmaDesc * CurrentBdPtr;
    volatile uint32 txEvents = 0;

    /* ============================= ASSERTIONS ============================= */

    if( ctx == NULL ) {
        xprintf( "No context\n" );
        return -1;
    }

    if( buffer == NULL ) {
        xprintf( "No dst buffer\n" );
        return -1;
    }

    if( buffer->buf_length > ENET_MAX_MTU_SIZE ) {
        xprintf( "src buffer too large.\n" );
        xprintf( "size is %d\n", buffer->buf_length );
        return -1;
    }

    softc = (bcmenet_softc *) ctx->dev_softc;
    if( softc == NULL ) {
        xprintf( "softc has not been initialized.\n" );
        return -1;
    }

    /* ====================================================================== */

    CurrentBdPtr = softc->txNextBdPtr;

    /* Find out if the next BD is available. */
    if( CurrentBdPtr->status & DMA_OWN ) {
        xprintf( "No tx BD available ?!\n" );
        return -1;
    }

    dstptr = (unsigned char *)PHYS_TO_K1( CurrentBdPtr->address );
    memcpy( dstptr, buffer->buf_ptr, buffer->buf_length );

    /* Set status of DMA BD to be transmitted. */
    status = DMA_SOP | DMA_EOP | DMA_APPEND_CRC | DMA_OWN;
    if( CurrentBdPtr == softc->txLastBdPtr ) {
        status |= DMA_WRAP;
    }
    CurrentBdPtr->length = buffer->buf_length;
    CurrentBdPtr->status = status;

    // Enable DMA for this channel
    softc->txDma->cfg |= DMA_ENABLE;

    // poll the dma status until done
    do
    {
        txEvents = CurrentBdPtr->status; 
    } while (txEvents & DMA_OWN);


    //Advance BD pointer to next in the chain.
    InctxBdPtr( CurrentBdPtr, softc );
    softc->txNextBdPtr = CurrentBdPtr;

    return 0;
}

/*
 * perfectmatch_write: write an address to the EMAC perfect match registers
 */
static void perfectmatch_write(int reg, unsigned char *pAddr, bool bValid)
{
    volatile uint32 *pmDataLo;
    volatile uint32 *pmDataHi;

    switch (reg) {
    case 0:
        pmDataLo = &EMAC->pm0DataLo;
        pmDataHi = &EMAC->pm0DataHi;
        break;
    case 1:
        pmDataLo = &EMAC->pm1DataLo;
        pmDataHi = &EMAC->pm1DataHi;
        break;
    case 2:
        pmDataLo = &EMAC->pm2DataLo;
        pmDataHi = &EMAC->pm0DataHi;
        break;
    case 3:
        pmDataLo = &EMAC->pm3DataLo;
        pmDataHi = &EMAC->pm3DataHi;
        break;
    default:
        return;
    }
    /* Fill DataHi/Lo */
    if (bValid == 1)
        *pmDataLo = MAKE4((pAddr + 2));
    *pmDataHi = MAKE2(pAddr) | (bValid << 16);
}


/*
 * bcm6348_write_mac_address: store MAC address into EMAC perfect match registers                   
 */
static void bcm6348_write_mac_address(bcmenet_softc *softc, unsigned char *macAddr)
{
    volatile uint32 data32bit;

    data32bit = EMAC->config;
    if (data32bit & EMAC_ENABLE) {
        /* disable ethernet MAC while updating its registers */
        EMAC->config &= ~EMAC_ENABLE ;           
        EMAC->config |= EMAC_DISABLE ;           
        while(EMAC->config & EMAC_DISABLE);     
    }

    perfectmatch_write(0, macAddr, 1);

    if (data32bit & EMAC_ENABLE) {
        EMAC->config = data32bit;
    }
}

static int bcm6348_ether_ioctl(cfe_devctx_t *ctx,iocb_buffer_t *buffer)
{
    int retval = 0;

    return retval;
}

static int bcm6348_ether_close(cfe_devctx_t *ctx)
{
    int i;
    bcmenet_softc * softc = (bcmenet_softc *) ctx->dev_softc;
    unsigned long sts;

    sts = softc->rxDma->intStat;
    softc->rxDma->intStat = sts;
    softc->rxDma->intMask = 0;
    softc->rxDma->cfg = 0;
    // wait the current packet to complete before turning off EMAC, otherwise memory corruption can occur.
    for(i=0; softc->rxDma->cfg & DMA_ENABLE; i++) {
        // put the line below inside - it seems the signal can be lost and DMA never stops
        softc->rxDma->cfg = 0;
        if (i >= 20) { break; }
        delay_t(CFE_HZ/10000);
    }

    sts = softc->txDma->intStat;
    softc->txDma->intStat = sts;
    softc->txDma->intMask = 0;
    softc->txDma->cfg = 0;
    for(i=0; softc->txDma->cfg & DMA_ENABLE; i++) {
        // put the line below inside - it seems the signal can be lost and DMA never stops
        softc->txDma->cfg = 0;
        if (i >= 20) { break; }
        delay_t(CFE_HZ/10000);
    }

    /* return buffer allocation register internal count to 0 */
    softc->dmaCtrl->flowctl_ch1_alloc = (DMA_BUF_ALLOC_FORCE | 0);

    softc->dmaCtrl->controller_cfg &= ~DMA_MASTER_EN;

    // wait the shut down process to complete. EMAC registers should't be modified before that.
    EMAC->config |= EMAC_DISABLE;
    for(i=0; EMAC->config & EMAC_DISABLE; i++) {
        EMAC->config |= EMAC_DISABLE;
        if (i >= 20) { break; }
        delay_t(CFE_HZ/10000);
    }

    KFREE( (void *)(softc->txBuffers) );
    KFREE( (void *)(softc->rxBuffers) );
    KFREE( (void *)(softc->txBds) );
    KFREE( (void *)(softc->rxBds) );
    return 0;
}

static void delay_t(int ticks)
{
    int32_t t;
    int tenTicks = ticks * 15;      //~~~

    t = _getticks() + tenTicks;
    while (_getticks() < t) ; /* NULL LOOP */
}

