/* $Id: exlib.h,v 1.23 2003/05/14 21:56:46 cgd Exp $ */

/*
 * Copyright 2003
 * Broadcom Corporation. All rights reserved.
 *
 * This software is furnished under license and may be used and copied only
 * in accordance with the following terms and conditions.  Subject to these
 * conditions, you may download, copy, install, use, modify and distribute
 * modified or unmodified copies of this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce and
 *    retain this copyright notice and list of conditions as they appear in
 *    the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Broadcom Corporation.  The "Broadcom Corporation" name may not be
 *    used to endorse or promote products derived from this software
 *    without the prior written permission of Broadcom Corporation.
 *
 * 3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 *    NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL BROADCOM BE LIABLE
 *    FOR ANY DAMAGES WHATSOEVER, AND IN PARTICULAR, BROADCOM SHALL NOT BE
 *    LIABLE FOR DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *    OR OTHERWISE), EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* exlib.h: Exception handling library "user" header file.

   This header defines structures, constants, and functions which may
   be used by callers of the -lexception exception handling library.  */

#ifndef _SIBYTE_EXLIB_H_
#define _SIBYTE_EXLIB_H_

/* struct exframe: Structure to hold state saved on exception.

   When the exception handling library handles an exception it saves
   the machine state into this structure.  It passes a pointer to the
   structure to exception handlers.

   Note that the FP state saved depends on the flags used to link the
   library, i.e., if a soft-float library is used no FP registers will
   ever be saved, and 16- vs. 32-register mode (Status:FR) is chosen
   at link time.  Don't change Status:FR at run time and expect the
   result to work.  */

#ifndef __LANGUAGE_ASSEMBLY
struct exframe
  {
    unsigned long long ef_regs[32];
#define ef_pc ef_regs[0]

    unsigned long long ef_hi;
    unsigned long long ef_lo;

    unsigned int ef_status;
    unsigned int ef_cause;

#ifndef __mips_soft_float
    unsigned int ef_fpsavemask;
    unsigned int ef_fcsr;
#if (__mips_fpr == 64)
    unsigned long long ef_fpregs[32];
    unsigned long long ef_mdmxacc[3];		/* High, middle, low.  */
#else
    unsigned long long ef_fpregs[16];
#endif
#endif /* __mips_soft_float */
  };
#endif /* __LANGUAGE_ASSEMBLY */

/* These constants are used from assembly language, when creating and
   filling in the exframe structure.  */
#define EXFRAME_REG_OFF(x)      (8 * (x))
#define EXFRAME_REG_SIZE        (32 * 8)
#define EXFRAME_PC_OFF          (EXFRAME_REG_OFF (0))

#define EXFRAME_HI_OFF          (EXFRAME_REG_OFF (0) + EXFRAME_REG_SIZE)
#define EXFRAME_HI_SIZE         8

#define EXFRAME_LO_OFF          (EXFRAME_HI_OFF + EXFRAME_HI_SIZE)
#define EXFRAME_LO_SIZE         8

#define EXFRAME_STATUS_OFF      (EXFRAME_LO_OFF + EXFRAME_LO_SIZE)
#define EXFRAME_STATUS_SIZE     4

#define EXFRAME_CAUSE_OFF       (EXFRAME_STATUS_OFF + EXFRAME_STATUS_SIZE)
#define EXFRAME_CAUSE_SIZE      4

#define EXFRAME_FP_OFF          (EXFRAME_CAUSE_OFF + EXFRAME_CAUSE_SIZE)
#ifdef __mips_soft_float
# define EXFRAME_FP_SIZE        0
#else
# if (__mips_fpr == 64)
#  define EXFRAME_FP_SIZE       ((2 * 4) + (32 * 8) + (3 * 8))
# else
#  define EXFRAME_FP_SIZE       ((2 * 4) + (16 * 8))
# endif
#endif

#define EXFRAME_SIZE            (EXFRAME_FP_OFF + EXFRAME_FP_SIZE)


/* FP state to attempt to save (set when establishing handler), or to
   attempt to restore (set in exframe structure).  Note that the
   exframe structure records in ex_fpsavemask the state actually saved
   in the frame.

   If an exception handler uses FP registers and returns normally,
   then EXFPSAVEMASK_FCSR, EXFPSAVEMASK_CALLER, and
   EXFPSAVEMASK_MDMXACC (if it uses MDMX) should be used.  If an
   exception handler uses FP registers and returns via a direct call
   to exlib_return(), EXFPSAVEMASK_ALL should be used.  */

#define EXFPSAVEMASK_NONE     0
#define EXFPSAVEMASK_FCSR     0x1		/* Save FCSR.  */
#define EXFPSAVEMASK_CALLER   0x2		/* Save caller-saved regs.  */
#define EXFPSAVEMASK_CALLEE   0x4		/* Save callee-saved regs.  */
#define EXFPSAVEMASK_MDMXACC  0x8		/* Save MDMX accumulator.  */
#define EXFPSAVEMASK_ALL      (EXFPSAVEMASK_FCSR | EXFPSAVEMASK_CALLER \
                               | EXFPSAVEMASK_CALLEE | EXFPSAVEMASK_MDMXACC)


/* Exception codes reported by the library are Cause:ExcCode values
   (which are in the range [0..31]).  */

#define EXCODE_NCODES		32


#ifndef __LANGUAGE_ASSEMBLY

/* exhandler_func: An exception handler function.  */
typedef void (*exhandler_func) (unsigned int /* code */,
                                struct exframe * /* ef */);

/* exlib_init: Initialize the exception handling library.

   Call this at the start of a program which uses -lexception, before
   making any other exception library calls.  This function may be
   called multiple times, and will only take action if the exception
   handling library has not been initialized. */
void exlib_init (void);

/* exlib_shutdown: Shut down the exception handling library.

   Optionally call this at the end of a program which uses
   -lexception.  This function may be called multiple times, and will
   only take action if the exception handling library is initialized.  */
void exlib_shutdown (void);

/* exlib_set_common_handlers: Setup common exception handlers.

   This function sets up oft-used exception handlers: panic on most
   exceptions, enable MDMX on an MDMX exception.  (By default, all
   exception handlers simply abort.)  */
void exlib_set_common_handlers (void);

/* exlib_panic: Print an error message and exit.

   This function prints the error message (a la printf), dumps the
   given frame (if non-NULL), and causes the program to exit with an
   error code.  */
void exlib_panic (struct exframe * /* ef */ , const char * /* fmt */, ...);

/* exlib_return: Return to saved context.

   This function restores the registers saved in exception frame (and
   thereby returns to the context that took the exception or interrupt
   that created the frame.  */
void exlib_return (struct exframe * /* ef */) __attribute__ ((__noreturn__));

/* exlib_set_handler and exlib_get_handler: Set and get the handler
   for the given exception code.  */
int exlib_set_handler (unsigned int /* excode */, exhandler_func /* f */);
exhandler_func exlib_get_handler (unsigned int /* excode */);

/* exlib_set_fpsavemask and exlib_get_fpsavemask: Set and get the mask
   of FP registers to save for the given exception code.

   Note that the more registers you save, the slower the exception
   dispatch for a given exception will be.  */
int exlib_set_fpsavemask (unsigned int /* excode */,
                          unsigned int /* fpsavemask */);
unsigned int exlib_get_fpsavemask (unsigned int /* excode */);

/* exframe_dump: Dump the state saved in the given exception frame
   (print it to stdout).  */
void exframe_dump (struct exframe * /* ef */);

/* The following are commonly-used exception handlers provided by this
   library:

   exhandler_abort: Causes the program to exit with an error result.
   (This is the default handler for all exceptions after library
   initialization.)

   exhandler_mdmxenable: Enables access to MDMX resources and returns.

   exhandler_panic: Prints a message indicating a fatal error, dumps
   the contents of the exception frame, and causes the program to exit
   with an error result.  */
void exhandler_abort (unsigned int /* code */, struct exframe * /* ef */);
void exhandler_mdmxenable (unsigned int /* code */, struct exframe * /* ef */);
void exhandler_panic (unsigned int /* code */, struct exframe * /* ef */);


/* Below are definitions related to a simple interrupt handling
   abstraction provided by the exception handling library.  */

/* intr_func: An interrupt-handling function.

   This function must return non-zero if it believes it handled the
   cause of the interrupt.  If an interrupt occurs and nothing handles
   it, a panic results.  */
typedef int (*intr_func) (void *, struct exframe *);

/* struct ihand: Structure used to hold interrupt handler data.

   The caller of intr_handler_add() should allocate one of these
   structures, and should initialize it using the INIT_IHAND() macro.
   The structure must remain valid until the handler is removed with
   intr_handler_remove().

   Note that the set of FP registers saved on interrupt is the union
   of those used by all interrupt handlers, so if *ANY* handlers use
   FP, all will pay the price!  */
struct ihand {
  struct ihand *ih_next;
  intr_func ih_func;			/* Function to call.  */
  void *ih_arg;				/* Arg to pass to function.  */
  unsigned int ih_fpsavemask;		/* FP registers to save.  */
};

#define INIT_IHAND(ihp, f, a, sm)					\
  do									\
   {									\
     (ihp)->ih_next = 0;						\
     (ihp)->ih_func = (f);						\
     (ihp)->ih_arg = (a);						\
     (ihp)->ih_fpsavemask = (sm);					\
   }									\
  while (0)

/* intr_init: Initialize interrupt handling facilities.

   Invoke this after exlib_init() to initialize the interrupt dispatch
   routines.  This function may be called multiple times, and will
   only take action if the interrupt handling facilities have not been
   initialized.  */
void intr_init (void);

/* intr_shutdown: Shut down interrupt handling facilities.

   Optionally call this at the end of a program which uses the
   interrupt handling facilities, before the call to exlib_shutdown().
   This function may be called multiple times, and will only take
   action if the exception handling library is initialized.  */
void intr_shutdown (void);

/* intr_set_ie and intr_get_ie: Set or clear the master interrupt
   enable.

   These functions set and clear the master interrupt enable
   (Status:IE).  If 'ef' is given as NULL, the change takes effect
   immediately.  Otherwise the given frame's state is changed so that
   Status:IE will be set or cleared when the frame's state is
   restored.  These functions return the previous state of the
   interrupt enable (1 means enabled, 0 means disabled).

   Typically, if you are calling these functions from an exception
   or interrupt context, you should pass in a valid the exframe
   structure pointer.  (If you enable interrupts immediately, later
   restoring an exframe may disable them again.)  */
int intr_set_ie (struct exframe * /* ef */);
int intr_clear_ie (struct exframe * /* ef */);

/* intr_enable_int and intr_disable_int: Enable or disable an interrupt.

   The functions enable or disable a particular interrupt via the
   interrupt mask bits (Status:IM0..IM7).  'i' must be in the range
   [0..7].  These functions return the previous state of the interrupt
   enable (1 means enabled, 0 means disabled).

   Like intr_set_ie() and intr_clear_ie(), if 'ef' is given as NULL the
   change takes effect immediately (and may be clobbered by a future
   return from exception).  Otherwise the change will take effect when
   then given frame's state is restored.  */
int intr_enable_int (struct exframe * /* ef */, unsigned int  /* i */);
int intr_disable_int (struct exframe * /* ef */, unsigned int  /* i */);

/* intr_set_softint and intr_clear_sofint: Set or clear a software
   interrupt request bit.

   These functions set or clear one of the software interrupt request
   bits (Cause:IP0..IP1).  'i' must be in the range of [0..1].  They
   return the previous state of the bit (1 means set, 0 means clear).

   These operations takes effect immediately (but obviously a software
   interrupt won't be delivered until its interrupt enable is set and
   the master interrupt enable is also set).

   Note that the interrupt dispatch code DOES NOT clear software
   interrupts automatically.  You must use intr_clear_softint() from
   your handler to do that.  */
int intr_set_softint (unsigned int /* i */);
int intr_clear_softint (unsigned int /* i */);

/* intr_handler_add and intr_handler_remove: Add or remove interrupt
   handlers.

   These functions add and remove handlers for the interrupt named by
   'intr'.  'intr' must be in the range [0..7].

   A given 'struct ihand' may be used to handle no more than one
   interrupt at a time, and must be removed from the same interrupt to
   which it was added.  */
void intr_handler_add (unsigned int /* intr */, struct ihand * /* ic */);
void intr_handler_remove (unsigned int /* intr */, struct ihand * /*ic */);

#endif /* __LANGUAGE_ASSEMBLY */

#endif /* _SIBYTE_EXLIB_H_ */
