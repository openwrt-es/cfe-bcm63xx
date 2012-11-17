#include "sbmips.h"
#include "bsp_config.h"

#include "6338_cpu.h"
#include "6338_common.h"
#include "board.h"
#include "boardparms.h"
#include "mipsmacros.h"

/*  *********************************************************************
    *  BOARD_EARLYINIT()
    *  
    *  Initialize board registers.  This is the earliest 
    *  time the BSP gets control.  This routine cannot assume that
    *  memory is operational, and therefore all code in this routine
    *  must run from registers only.  The $ra register must not
    *  be modified, as it contains the return address.
    *
    *  This routine will be called from uncached space, before
    *  the caches are initialized.  If you want to make
    *  subroutine calls from here, you must use the CALLKSEG1 macro.
    *
    *  Among other things, this is where the GPIO registers get 
    *  programmed to make on-board LEDs function, or other startup
    *  that has to be done before anything will work.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

LEAF(board_earlyinit)

        .set    noreorder
        mtc0    zero,C0_SR
        li      v0,M_SR_BEV
        mtc0    v0,C0_SR                # state unknown on reset
        mtc0    zero,C0_CAUSE           # clear software interrupts
        nop                             # paranoia

        /**--------------------------------------------------------------**/
        /** platform specific code                                       **/
        /**--------------------------------------------------------------**/
        /**----- Initialize EBI -----------------------------------------**/
        li      t1, MPI_BASE            
        li      t2, EBI_SIZE_4M
        or      t2, a0
        sw      t2, CS0BASE(t1)        # CS[0] Base
        li      t2, THREEWT|EBI_WORD_WIDE|EBI_ENABLE
        sw      t2, CS0CNTL(t1)        # CS[0] Control

        /**----- Enable I Cache -----------------------------------------**/
        mfc0    t1, C0_BCM_CONFIG
        nop
        or      t1, (CP0_CFG_ICSHEN | CP0_CFG_DCSHEN | CP0_CFG_OWBE)
        mtc0    t1, C0_BCM_CONFIG      # Enable I Cache
        nop

        /**----- Switch MIPS to sync mode -------------------------------**/
        .word   0x4009b004             # mfc t1, C0_BCM_CONFIG, 4
        and     t1, ~(0x7 << 22)
        .word   0x4089b004             # mtc t1, C0_BCM_CONFIG, 4
        sync


        /**----- Enable branch prediction -------------------------------**/
        .word   0x4009b005          # mfc t1, C0_BCM_CONFIG, 5
        and     t1, ~(0x1 << 16)
        .word   0x4089b005          # mtc t1, C0_BCM_CONFIG, 5

        /**----- Enable RAC ---------------------------------------------**/
        li      t2, MIPS_BASE
        li      t1, 0x200 << RAC_UPB_SHFT    # Enable prefetch for any address
        sw      t1, RAC_CR1(t2)

        lw      t1, RAC_CR0(t2)
        or      t1, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t1, RAC_CR0(t2)
        sync

        /**----- Initialize Serial --------------------------------------**/
        li       t3, ((FPERIPH / 115200) / 16)
        /*
        #  Baudword = (FPeriph)/Baud/32-1.  We have to perform rounding
        #  and subtraction.  Above we divided by 16 (instead of 32).  If
        #  bit0 is set, we round up.  However, we then subtract 1, so final
        #  result should be t3/2.  If bit0 is 0, then we truncate and subtract
        #  1, t3=t3/2-1.
        */
        andi    t0, t3, 0x1
        bne     t0,zero,1f              # do shift only (in delay slot)
                                        # and jump to apply
        srl     t3,1                    # do divide by 2
        addiu   t3, -1                  # subtract 1
1:		

        # t3 contains the UART BAUDWORD
        li      t0, UART_BASE
        sw      t3, UART0BAUD(t0)       # Store BaudRate
        li      t1, BITS8SYM|ONESTOP    
        sb      t1, UART0CONFIG(t0)     # 8 Bits/1 Stop
        li      t1, TX4|RX4             
        sb      t1, UART0FIFOCFG(t0)    # FifoThreshold
        li      t1, 0x5
        sb      t1, UART0RXTIMEOUT(t0)  # RxTimeout
        li      t1, TXEN|RXEN|BRGEN|TXPARITYEVEN|RXPARITYEVEN
        sb      t1, UART0CONTROL(t0)    # Enable, No Parity
        move    t1, zero
        sh      t1, UART0INTMASK(t0)
        .set    reorder

        j	ra
END(board_earlyinit)

/*  *********************************************************************
    *  BOARD_DRAMINFO
    *  
    *  Return the address of the DRAM information table
    *  
    *  Input parameters: 
    *  	   nothing
    *  	    
    *  Return value:
    *  	   v0 - DRAM info table, return 0 to use default table
    ********************************************************************* */
LEAF(board_draminfo)
        j	ra
END(board_draminfo)

/*  *********************************************************************
    *  BOARD_DRAMINIT
    *  
    *  This routine should activate memory.
    *  
    *  Input parameters: 
    *  	   a0 - points to configuration table returned by board_draminfo
    *           or 0 to use an automatic table
    *  	   
    *  Return value:
    *  	   v0 - total memory installed
    *  	   
    *  Registers used:
    *  	   can use all registers.
    ********************************************************************* */
LEAF(board_draminit)

        .set    noreorder

        li      t1,SDRAM_BASE          

        /* Enable prioritization */
        lw      t2,SDR_PRIOR(t1)
        or      t2,SDR_EN_PRIOR
        or      t2,SDR_ADSL_HPE
        sw      t2,SDR_PRIOR(t1)
        
        /* check memory config type 16MB and default to 8MB */
        li      t0,BOARD_SDRAM_TYPE_ADDRESS
        lw      t0,0(t0)
		
        /* setting for 16MB, 1 chip */
        li      t3,SDR_PEND|SDR_2_BANKS|SDR_9_COLS|SDR_12_ROWS
        li      v0,16
        li      t4,BP_MEMORY_16MB_1_CHIP
        beq     t0,t4,1f
        nop
 
        /* Default SDRAM settings for BCM6338 board (8MB, 1 chip) */
        li      t3,SDR_PEND|SDR_2_BANKS|SDR_8_COLS|SDR_12_ROWS
        li      v0,8
			
1:
        or      t3,SDR_MASTER_EN|SDR_MRS_CMD|SDR_PEND
        li      t2,SDR_SLOW_MEM|SDR_FULL_PG
        sw      t2,SDR_CFG_REG(t1)    # Full page burst, fast memory
        li      t2,SDR_MASTER_EN|SDR_PRE_CMD
        sw      t2,SDR_INIT_CTL(t1)   # Pre-charge powerup sequence
        li      t2,SDR_MASTER_EN|SDR_CBR_CMD
        sw      t2,SDR_INIT_CTL(t1)   # 8 or more auto-refresh cmds
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t3,SDR_INIT_CTL(t1)   # Mode Register set, 
        li      t2,SDR_REF_EN|0x40    # Refresh period 0x40, Enable 
        sw      t2,SDR_REF_CTL(t1)    

        j       ra
        nop
        .set    reorder
END(board_draminit)

/*  *********************************************************************
    *  BOARD_SETLEDS(x)
    *  
    *  Set LEDs for boot-time progress indication.  Not used if
    *  the board does not have progress LEDs.  This routine
    *  must not call any other routines, since it may be invoked
    *  either from KSEG0 or KSEG1 and it may be invoked 
    *  whether or not the icache is operational.
    *  
    *  Input parameters: 
    *  	   a0 - LED value (8 bits per character, 4 characters)
    *  	   
    *  Return value:
    *  	   nothing
    *  
    *  Registers used:
    *  	   t0,t1,t2,t3
    ********************************************************************* */
LEAF(board_setleds)
#if 0
        li      t0, UART_BASE
        li      t2, TXFIFOEMT

1:      lh      t1, UART0INTSTAT(t0)
        and     t1, t2
        bne     t1, t2, 1b

        srl     t3, a0, 24
        sb      t3, UART0DATA(t0)
        srl     t3, a0, 16
        sb      t3, UART0DATA(t0)
        srl     t3, a0, 8
        sb      t3, UART0DATA(t0)
        sb      a0, UART0DATA(t0)
        li      a0, '\r'
        sb      a0, UART0DATA(t0)
        li      a0, '\n'
        sb      a0, UART0DATA(t0)
#endif
        j	ra
END(board_setleds)

