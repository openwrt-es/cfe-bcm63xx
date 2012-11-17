#include "sbmips.h"

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
		j	ra
END(board_setleds)

LEAF(bcmcore_tp1_switch)
		j	ra
END(bcmcore_tp1_switch)
