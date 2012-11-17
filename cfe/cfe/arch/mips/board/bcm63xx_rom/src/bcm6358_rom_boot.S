#include "sbmips.h"
#include "bsp_config.h"

#include "6358_cpu.h"
#include "6358_common.h"
#include "board.h"
#include "boardparms.h"
#include "mipsmacros.h"

#define SETLEDS1(a,b,c,d)                     \
       li       a0,(((a)<<24)|((b)<<16)|((c)<<8)|(d)) ;    \
       jal      board_setleds

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

        .word   0x4009b003              # mfc0 t1, C0_BCM_CONFIG, 3
        li      t2, CP0_CMT_TPID
        and     t1, t2
        bnez    t1, 2f                  # if we are running on thread 1, skip init
        nop
        
        /**--------------------------------------------------------------**/
        /** platform specific code                                       **/
        /**--------------------------------------------------------------**/
        /**----- Enable I Cache -----------------------------------------**/
        mfc0    t1, C0_BCM_CONFIG
        or      t1, (CP0_BCM_CFG_ICSHEN | CP0_BCM_CFG_DCSHEN)
        mtc0    t1, C0_BCM_CONFIG      # Enable I Cache

        // In the begining MIPS core registers are mapped to 0xbfax_xxxx
        li      t1, 0x1FA0000C      # Set up CBR to 1FAx_xxxx
        .word   0x4089b006          # mtc t1, C0_BCM_CONFIG, 6
        
        li      t1, MIPS_BASE_BOOT
        lw      t2, MIPS_LMB_CR(t1)
        or      t2, 0xC0000000      # enable ffxx_xxxx space   
        sw      t2, MIPS_LMB_CR(t1)
        li      t2, 0xFFF80001      # SBR FFF8_xxxx and enable
        sw      t2, MIPS_SBR(t1)
        
        // Now map MIPS core registers to 0xFF4x_xxxx space
        li      t1, 0xFF40000C      # CBR FF4x_xxxx (and reserved bits 0xc).
        .word   0x4089b006          # mtc t1, C0_BCM_CONFIG, 6

        /**----- Initialize EBI -----------------------------------------**/
        li      t1, MPI_BASE            
        li      t2, EBI_SIZE_32M
        or      t2, a0
        sw      t2, CS0BASE(t1)        # CS[0] Base
        li      t2, THREEWT|EBI_WORD_WIDE|EBI_ENABLE
        sw      t2, CS0CNTL(t1)        # CS[0] Control

        /**----- Initialize Serial --------------------------------------**/
        li      t3, ((FPERIPH / 115200) / 16)
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
2:
        j	    ra
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
    *  	   None
    *  	   
    *  Return value:
    *  	   None
    *  	   
    *  Registers used:
    *  	   can use all registers.
    ********************************************************************* */
LEAF(board_draminit)
        li      sp, 0xfff84000                  # for now use the buffer manager memory for stack
        sub     sp, 4
        sw      ra, 0(sp)

        .set    noreorder

        li      t0, MEMC_BASE
        li      t1, DDR_BASE

        /* check memory config type */
        li      t3, BOARD_SDRAM_TYPE_ADDRESS
        FIXUP(t3)
        lw      t3, 0(t3)

        /* setting for 32MB, 1 chip */
        li      t2, (MEMC_13BIT_ROW | MEMC_9BIT_COL)
        li      v0, 32
        li      t4, BP_MEMORY_32MB_1_CHIP
        beq     t3, t4, 1f
        nop
 
        /* Default DRAM settings for BCM6358 board (16MB, 1 chip) */
        li      t2, (MEMC_12BIT_ROW | MEMC_9BIT_COL)
        li      v0, 16

1:
        or      t2, (MEMC_SELPRIORITY | MEMC_HIPRRTYQEN | MEMC_MIPS1HIPREN)
        or      t2, (MEMC_DQSGATEEN | MEMC_16BIT_BUS | MEMC_MEMTYPE_DDR)
        sw      t2, MEMC_CONFIG(t0)             # Enable DDR Mem & SEQ EN, 16MB

        li      t2, 0x7                         # Reduce drive strength for command pins (per John Lorek)
        sw      t2, DDR_CMD_PAD_CNTL(t1)

        li      t2, 0x000754d9
        sw      t2, MEMC_DRAM_TIM(t0)           # DDR Timing Set Latency  2.5 Latency
        li      t2, 0x00000003
        sw      t2, MEMC_CONTROL(t0)            # Turn on CKE 
        li      t2, 0x0000000B
        sw      t2, MEMC_CONTROL(t0)            # PreCharge 
        li      t2, 0x00004002
        sw      t2, MEMC_M_EM_BUF(t0)           # Value for Extended Mode Register
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS command
        li      t2, 0x00000163
        sw      t2, MEMC_M_EM_BUF(t0)           # Reset DLL, Burst Length = 8, Burst Type Sequential  2.5 Latency
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS command
        nop                                     # Delay 200 DDR clock cycles (~1.5 uS)
        nop
        nop
        li      t2, 0x0000000B
        sw      t2, MEMC_CONTROL(t0)            # Precharge 
        li      t2, 0x0000840f
        sw      t2, MEMC_REF_PD_CONTROL(t0)     # Enable auto refresh
        li      t2, 0x00000007
        sw      t2, MEMC_CONTROL(t0)            # Set Auto Refresh Mode  
        li      t2, 0x00000007
        sw      t2, MEMC_CONTROL(t0)            # Set Auto Refresh Mode  
        li      t2, 0x00000063
        sw      t2, MEMC_M_EM_BUF(t0)           # Reset DLL, Burst Length = 8, Burst Type Sequential 2.5 Latency 
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS


        # It's very important to turn off the counter bypass bit BEFORE writing
        # new values to the phase control registers.
        li      t2, (7 << 28) | (7 << 24) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PLL cnt updt | Other Clocks
        sw      t2, DDR_MIPS_PHASE_CNTL(t1)

        # Calculate a value for a 90 degree phase shift.
        lw      t2, DDR_MIPSDDR_PLL_CONFIG(t1)
        srl     t2, 8                               # Shift and mask off DDR_MDIV
        and     t2, 0xff
        sll     t2, 3                               # PI_steps = (90deg * 32 * MBUS(t2) + 2)/360 ~= MBUS * 8
        or      t2, (1 << 13)                       # set the count direction

        lw      t3, DDR_DDR3_4_PHASE_CNTL0(t1)      # Get current DDR3/4 value.
        ori     t3, 0x7fff                          # Clear low 15 bits (DDR3 value).
        xori    t3, 0x7fff
        or      t3, t2                              # Set new DDR3 value, preserving existing DDR4 value.
        sw      t3, DDR_DDR3_4_PHASE_CNTL0(t1)

        .set    reorder
        
        jal     sdramDqsPhaseSet
        
        /**----- switch to sync -----------------------------------------**/
        li      t0, 0xff410000
        li      t1, DDR_BASE
        li      t2, 2048
        li      t3, 1

1:
        lw      t4, 0x40(t0)                # Read a sample value.
        srl     t4, 16                      # The sample is in the upper 16 bits.
 
        andi    t4, t4, 0x41                # Look at the 2 outermost bits; if the LSB is 0 and the MSB is 1, 
        beq     t4, 0x40, 2f                # then there is an edge somewhere in the sample.
 
        and     t5, t3, 0x7ff               # Adjust the sample window.
        or      t5, (7 << 28) | (7 << 24) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PLL cnt updt | Other Clocks
        sw      t5, DDR_MIPS_PHASE_CNTL(t1)
 
        lw      a1, 0x40(t0)                # Delay before reading another sample.
        add     t3, 1
        bne     t2, t3, 1b
        b       3f

2:
        # Success
        lw      t2, DDR_MIPS_PHASE_CNTL(t1) # Turn on auto-PI mode.
        and     t2, 0xffff
        or      t2, (7 << 28) | (1 << 24) | (1<< 21) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PI | Enable PLL cnt updt | Other Clocks
        sw      t2, DDR_MIPS_PHASE_CNTL(t1)

        li      t2, 0x0010                  # Set PI mask to 0000110, and check new value every 16 MIPS cycles.
        sw      t2, 0x40(t0)                # set PI update to 16 ddr cycles
        li      t2, 0x80000090              # Enable MIPS auto-PI | Enable update period | Set 16 clock update
        sw      t2, 0x40(t0)
        li      t2, 0x80000c90              # Enable MIPS auto-PI | Enable comparator | Enable update period | Set 16 clock update
        sw      t2, 0x40(t0)
        
        lw      t2, 0x40(t0)                # Do a few reads to wait till the edge is stable...
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
 
        .word   0x4009b005                  # mfc t1, C0_BCM_CONFIG, 5
        and     t1, ~(0x1 << 28)
        .word   0x4089b005                  # mtc t1, C0_BCM_CONFIG, 5

3:
        /**----- Enable RAC and LMB -------------------------------------**/
        li      t1, MIPS_BASE
        lw      t2, MIPS_LMB_CR(t1)
        or      t2, LMB_EN | DISABLE_UCS_FIX  # Enable LMB
        sw      t2, MIPS_LMB_CR(t1)

        li      t2, 0x1000 << RAC_UPB_SHFT    # Enable prefetch for any address
        sw      t2, MIPS_RAC_ARR(t1)

        lw      t2, MIPS_RAC_CR0(t1)
        or      t2, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t2, MIPS_RAC_CR0(t1)

        lw      t2, MIPS_RAC_CR1(t1)
        or      t2, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t2, MIPS_RAC_CR1(t1)

        /**----- Enable branch prediction and non-blocking data cache ---**/
        mfc0    t1, C0_BCM_CONFIG
        and     t1, ~CP0_BCM_CFG_BTHD
        or      t1, CP0_BCM_CFG_NBK
        mtc0    t1, C0_BCM_CONFIG

        lw      ra, 0(sp)
        add     sp, 4
        j       ra

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

/*  *********************************************************************
    *  BCMCORE_TP1_SWITCH()
    *  
    *  Check if the thread switch is required. If we are already 
    *  running on thread 1 this function will do nothing and just return
    *  If we are running on thread 0 this function will take thread 1
    *  out of reset and put thread 0 to sleep waiting for singnal from
    *  thread 1.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */
jump_code:
        li      t1, 0xbe000000
        j       t1

LEAF(bcmcore_tp1_switch)

        .word   0x4009b003              # mfc0 t1, C0_BCM_CONFIG, 3
        li      t2, CP0_CMT_TPID
        and     t1, t2
        bnez    t1, 1f                  # if we are already running on thread 1, skip this code

        li      t0,BOARD_THREAD_NUM_ADDRESS
        FIXUP(t0)
        lw      t0,0(t0)
        beqz    t0, 1f                  # don't do anything if Thread 0 is requested

        beqz    s6, 3f                  # s6 contains boot offset. If s6 == 0 MIPS running from 0xBFC00000 address
        
# Set boot address for TP1
# there is a bug in the chip preventing boot address to be set at 0xbe000000
# we can set boot address at 0xfff00000 (ADSL PHY memory) and then set an 
# instruction there to jump to 0xbe000000

#define ADSL_LMEM_BASE      0xFFF00000
#define ADSL_CORE_RESET     0xf98

        li      t1, MIPS_BASE
        lw      t2, MIPS_TP1_ALT_BV(t1)
        or      t2, ENABLE_ALT_BV
        li      t4, ADSL_LMEM_BASE
        or      t2, t4
        sw      t2, MIPS_TP1_ALT_BV(t1)

        # take ADSL core out of reset
        li      t1, ADSL_BASE
        sw      zero, ADSL_CORE_RESET(t1)

        # copy jump code to ADSL LMEM
        LOADREL(t3, jump_code)
        lw      t2, 0(t3)
        sw      t2, 0(t4)
        lw      t2, 4(t3)
        sw      t2, 4(t4)
        lw      t2, 8(t3)
        sw      t2, 8(t4)

3:        
        # enable interrupts and enable SW IRQ 0
        li      t0, M_SR_IE | M_SR_IBIT1
        mtc0    t0, C0_SR

        # Set up to use alternate exception vector 0x80000200
        li      t0, M_CAUSE_IV
        mtc0    t0, C0_CAUSE

        .word   0x4009b001                  # mfc0 t1, C0_BCM_CONFIG, 1
        # set all ints except IRQ1 to TP1 and cross over SW IRQ 0
        or      t1, (CP0_CMT_XIR_4 | CP0_CMT_XIR_3 | CP0_CMT_XIR_2 | CP0_CMT_XIR_0 | CP0_CMT_SIR_0 | CP0_CMT_NMIR_TP1)
        .word   0x4089b001                  # mtc0 t1, C0_BCM_CONFIG, 1

        .word   0x4009b002                  # mfc0 t1, C0_BCM_CONFIG, 2
        # Take TP1 out of reset, set debug on TP1, give priority to TP0, and 
        # set TLB exception serialization to ignore SCNT value in CP0 reg22 sel 4
        li      t2, ( CP0_CMT_TPS3 | CP0_CMT_TPS2 | CP0_CMT_TPS1 )
        nor     t2, t2, t2
        and     t1, t1, t2
        or      t1, (CP0_CMT_DSU_TP1 | CP0_CMT_PRIO_TP0 | CP0_CMT_RSTSE | CP0_CMT_TPS0)
        .word   0x4089b002                  # mtc0 t1, C0_BCM_CONFIG, 2

        # TP0 will be used for voice. Enable Data RAC
        li      t1, MIPS_BASE
        lw      t2, MIPS_RAC_CR0(t1)
        or      t2, (RAC_D | RAC_PF_D)
        sw      t2, MIPS_RAC_CR0(t1)

        li      t8, 0

2:
        sync
        wait

        beqz    t8, 2b
        jalr    t8                          # jump to execute TP0 code

        li      t8, 0
        b       2b                          # if ever come back, wait for another SW IRQ

1:
        j       ra
END(bcmcore_tp1_switch)

