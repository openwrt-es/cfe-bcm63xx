#include "sbmips.h"
#include "bsp_config.h"

#include "6348_cpu.h"
#include "6348_common.h"
#include "board.h"
#include "boardparms.h"
#include "mipsmacros.h"

    .globl  pll_table
pll_table:
    _LONG_ 4,6, 0x0000001B, 0x0B6D0B6D
    _LONG_ 5,8, 0x00000017, 0x00B500AD
    _LONG_ 4,7, 0x00000016, 0x2B561AB5
    _LONG_ 5,9, 0x00000018, 0x01550155
    _LONG_ 4,8, 0x00000017, 0x00AA0055
    _LONG_ 3,6, 0x0000001B, 0x0AAA0555 
    _LONG_ 4,9, 0x00000018, 0x02AA02AA
    _LONG_ 3,7, 0x00000016, 0x002A002A
    _LONG_ 5,12,0x0000001B, 0x054A052A
    _LONG_ 3,8, 0x00000017, 0x0052004A
    _LONG_ 4,11,0x0000001A, 0x02520252
    _LONG_ 4,12,0x0000001B, 0x04920492
    _LONG_ 0,0,0,0

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
    *  	   a0 - Flash base address (address of MIPS reset)
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

        /**----- Enable I Cache -----------------------------------------**/
        mfc0    t1, C0_BCM_CONFIG
        nop
        or      t1, (CP0_CFG_ICSHEN | CP0_CFG_DCSHEN | CP0_CFG_OWBE)
        mtc0    t1, C0_BCM_CONFIG      # Enable I Cache
        nop

        /**----- Switch MIPS to sync mode -------------------------------**/
        li      v0, PERF_BASE
        lw      v0, PLL_STRAP_VALUE(v0) # v0 = PLL strap value

        /* Extracting m1,m2 values */
        andi    t0, v0, 0x07           # t0 = m2b
        srl     v0, v0, 3
        andi    t1, v0, 0x07           # t1 = m1b
        srl     v0, v0, 3
        andi    t2, v0, 0x07           # t2 = m1m
        srl     v0, v0, 3
        andi    t3, v0, 0x07           # t3 = m2r
        srl     v0, v0, 3
        andi    t4, v0, 0x07           # t4 = m1r

        /* calculating ratios */
        addiu   t3, t3, 4
        addiu   t4, t4, 1
        mult    t3, t4
        mflo    t3                     # t3 = (m2r+4) * (m1r+1)
        move    t5, t3                 # t5 = t3

        addiu   t0, t0, 4
        addiu   t1, t1, 1
        mult    t0, t1
        mflo    t0                     # t0 = (m2b+4) * (m1b+1)
        divu    t3, t0
        mflo    t3                     # t3 = t3 / t0 [rb]

        addiu   t2, t2, 1
        divu    t5, t2
        mflo    t5                     # t5 = t5 / t2 [rm]
        
        /* if((pll_table[i] == rb) && (pll_table[i+1] == rm)) */
        move    a0, ra
        LOADREL(v0, pll_table)
        move    ra, a0
1:
        lw      t2, 0(v0)
        beqz    t2, 3f                 # end of table, entry not found
        nop
        bne     t2, t3, 2f             # jump if pll_table[i] != rb
        nop
        lw      t2, 4(v0)
        bne     t2, t5, 2f             # jump if pll_table[i+1] != rm
        nop

        /* table entry found */
        li      v1, MIPS_BASE
        li      t0, (1 << 31)
        sw      t0, 16(v1)
        sync

        lw      t1, 8(v0)        
        .word   0x4089b001             # mtc0 t1, C0_BCM_CONFIG, 1
        lw      t1, 12(v0)
        .word   0x4089b003             # mtc0 t1, C0_BCM_CONFIG, 3

        /* enable new ratio */
        .word   0x4009b002             # mfc t1, C0_BCM_CONFIG, 2
        or      t1, 0x9                # set cfg_active and mips_pll_enable
        .word   0x4089b002             # mtc t1, C0_BCM_CONFIG, 2

        nop
        nop
        nop
        nop
        nop

        /* switch to sync */
        .word   0x4009b004             # mfc t1, C0_BCM_CONFIG, 4
        and     t1, ~(0x7 << 22)
        .word   0x4089b004             # mtc t1, C0_BCM_CONFIG, 4

        sync

        b       3f
        nop

2:
        /* get next table entry */
        addiu   v0, v0, 16
        b       1b
        nop

3:

        /**----- Enable branch prediction -------------------------------**/
        .word   0x4009b005          # mfc t1, C0_BCM_CONFIG, 5
        and     t1, ~(0x1 << 16)
        .word   0x4089b005          # mtc t1, C0_BCM_CONFIG, 5

        li      t0, PERF_BASE
        lw      t1, 0(t0)
        li      t2, 0x634800A1
        beq     t1, t2, 1f          # No RAC on A1
        /**----- Enable RAC ---------------------------------------------**/
        li      t2, MIPS_BASE
        li      t1, 0x200 << RAC_UPB_SHFT    # Enable prefetch for any address
        sw      t1, RAC_CR1(t2)

        lw      t1, RAC_CR0(t2)
        or      t1, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t1, RAC_CR0(t2)
1:
        sync
        
        /**----- Initialize MPI Clock ----------------------------------**/
        li      t0, PERF_BASE           # The address of the PLL register
        lw      t1, SYSPLLCFG(t0)
        and     t1, ~M_MPI_MASK
        or      t1, M_MPI_33MHZ << M_MPI_SHFT  # Set MPI clock to 33MHz
        sw      t1, SYSPLLCFG(t0)

        /**----- Initialize EBI -----------------------------------------**/
        li      t1, MPI_BASE
        li      t2, PHYS_FLASH_BASE|EBI_SIZE_4M
        sw      t2, CS0BASE(t1)        # CS[0] Base
        /** inv_xde **/
        li      t2, THREEWT|EBI_WORD_WIDE|EBI_ENABLE
        /* li      t2, FIVEWT|EBI_WORD_WIDE|EBI_ENABLE */
        sw      t2, CS0CNTL(t1)        # CS[0] Control

        /** Initialisation of CS1 timings, base address and range **/
        /** inv_xde: support of 8 MB flash **/
        li      t1,MPI_BASE
        li      t2,PHYS_FLASH_CS1_BASE|EBI_SIZE_16M
        sw      t2,CS1BASE(t1)        # CS[1] Base
        li      t2,THREEWT|EBI_WORD_WIDE|EBI_ENABLE
        sw      t2,CS1CNTL(t1)        # CS[1] Control
        /** inv_xde: DSP **/
        li      t1,MPI_BASE
        li      t2,PHYS_DSP_CS3_BASE|EBI_SIZE_4M
        sw      t2,CS3BASE(t1)        # CS[3] Base
        li      t2,FIVEWT|EBI_WORD_WIDE|EBI_ENABLE
        sw      t2,CS3CNTL(t1)        # CS[3] Control

        /**----- Initialize Serial --------------------------------------**/
        li              t3, ((FPERIPH / 115200) / 16)
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
        
        /* check memory config type 64MB, 32MB, 16MB and default to 8MB */
        li      t0,BOARD_SDRAM_TYPE_ADDRESS
        FIXUP(t0)
        lw      t0,0(t0)
		
        /* setting for 64MB, 2 chip */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_9_COLS|SDR_13_ROWS
        li      v0,64
        li      t4,BP_MEMORY_64MB_2_CHIP
        beq     t0,t4,1f
        nop

        /* setting for 32MB, 2 chip */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_9_COLS|SDR_12_ROWS
        li      v0,32
        li      t4,BP_MEMORY_32MB_2_CHIP
        beq     t0,t4,1f
        nop
 
        /* setting for 32MB, 16bit, 1 chip */
        li      t3,SDR_2_BANKS|SDR_16_BIT|SDR_9_COLS|SDR_13_ROWS
        li      v0,32
        li      t4,BP_MEMORY_32MB_1_CHIP
        beq     t0,t4,1f
        nop

        /* setting for 16MB, 2 chip */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_8_COLS|SDR_12_ROWS
        li      v0,16
        li      t4,BP_MEMORY_16MB_2_CHIP
        beq     t0,t4,1f
        nop
      
        /* setting for 16MB, 16bit, 1 chip */
        li      t3,SDR_2_BANKS|SDR_16_BIT|SDR_9_COLS|SDR_12_ROWS
        li      v0,16
        li      t4,BP_MEMORY_16MB_1_CHIP
        beq     t0,t4,1f
        nop

        /* Default SDRAM setting for 8MB, 16bit, 1 chip) */
        li      t3,SDR_2_BANKS|SDR_16_BIT|SDR_8_COLS|SDR_12_ROWS
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

