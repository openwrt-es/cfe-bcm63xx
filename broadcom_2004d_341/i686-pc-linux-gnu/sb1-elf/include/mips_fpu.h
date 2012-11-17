/* $Id: mips_fpu.h,v 1.2 2004/05/16 22:12:09 cgd Exp $ */

/*
 * Copyright 2004
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

/* mips_fpu.h: Routines to manipulate the MIPS FPU (CP1) control
   registers.

   Several features are provided by this header file and this library:

   * Inline functions to read and write the MIPS FPU control registers:

	unsigned int fpu_get_fir (void);
	unsigned int fpu_get_fccr (void);
	void         fpu_set_fccr (unsigned int);
	unsigned int fpu_get_fexr (void);
	void         fpu_set_fexr (unsigned int);
	unsigned int fpu_get_fenr (void);
	void         fpu_set_fenr (unsigned int);
	unsigned int fpu_get_fcsr (void);
	void         fpu_set_fcsr (unsigned int);

   * Definitions for fields of the MIPS FPU control registers.  See
     the M_* (bit mask), G_* (get field value), and V_*
     (create register value from field value) defines below.

   * Definitions for some constructor functions which can be used to
     set certain FPU control register fields automatically at program
     startup.  Two functions are provided auto_fpu_set_fs() and
     auto_fpu_clear_enables(), which set FCSR:FS and clear the FCSR
     exception enables, respectively.

     To use these functions, when linking your program use the compiler
     or linker '-u' command line option to force the function to be
     pulled in and link with -lmips_fpu.  For instance:

	sb1-elf-gcc foo.c -u auto_fpu_set_fs -lmips_fpu -Tcfe.ld

     will create a program from 'foo.c', linking with the CFE-based
     runtime support library and automatically setting FCSR:FS
     at startup.  */

#ifndef _SIBYTE_MIPS_FPU_H_
#define _SIBYTE_MIPS_FPU_H_

/* Bitfield macros.  */

/* Make a mask for 1 bit at position 'n'.  */
#define _FPU_MAKEMASK1(n)       (1 << (n))

/* Make a mask for 'v' bits at position 'n'.  */
#define _FPU_MAKEMASK(v, n)     (((1 << (v)) - 1) << (n))

/* Make a value at 'v' at bit position 'n'.  */
#define _FPU_MAKEVALUE(v, n)    ((v) << (n))

/* Retrieve a value from 'v' at bit position 'n' with 'm' mask bits.  */
#define _FPU_GETVALUE(v, n, m)  (((v) & (m)) >> (n))


/* Macros to build inline FCSR access functions.  */
#define _fpu_get_cr(name, num)                                        \
  static inline unsigned int                                          \
  fpu_get_ ## name (void)                                             \
  {                                                                   \
    unsigned int val;                                                 \
    __asm__ __volatile__ ("cfc1 %0, $%1"                              \
                          : "=r"(val) : "i"(num));                    \
    return val;                                                       \
  }

#define _fpu_set_cr(name, num)                                        \
  static inline void                                                  \
  fpu_set_ ## name (unsigned int val)                                 \
  {                                                                   \
    __asm__ __volatile__ ("ctc1 %0, $%1"                              \
                          : : "r"(val), "i"(num));                    \
  }


/* Floating Point Implementation Register (FIR) access functions.  */
_fpu_get_cr (fir, 0)

/* Floating Point Condition Codes Register (FCCR) access functions.  */
_fpu_get_cr (fccr, 25)
_fpu_set_cr (fccr, 25)

/* Floating Point Exceptions Register (FEXR) access functions.  */
_fpu_get_cr (fexr, 26)
_fpu_set_cr (fexr, 26)

/* Floating Point Enables Register (FENR) access functions.  */
_fpu_get_cr (fenr, 28)
_fpu_set_cr (fenr, 28)

/* Floating Point Control and Status Register (FCSR) access functions.  */
_fpu_get_cr (fcsr, 31)
_fpu_set_cr (fcsr, 31)


/* Values for rounding mode (RM) field in FCSR and FENR.  */
#define K_RM_RN         0			/* Round to Nearest.  */
#define K_RM_RZ         1			/* Round Toward Zero.  */
#define K_RM_RP         2			/* Round Toward +Infinity.  */
#define K_RM_RM         3			/* Round Toward -Infinity.  */

/* Masks for bits in Flags, Enables, and Cause in FCSR, FENR, and FEXR.  */
#define M_FEX_I         _FPU_MAKEMASK1 (0)	/* Inexact.  */
#define M_FEX_U         _FPU_MAKEMASK1 (1)	/* Underflow.  */
#define M_FEX_O         _FPU_MAKEMASK1 (2)	/* Overflow.  */
#define M_FEX_Z         _FPU_MAKEMASK1 (3)	/* Divide by Zero.  */
#define M_FEX_V         _FPU_MAKEMASK1 (4)	/* Invalid Operation.  */
#define M_FEX_E         _FPU_MAKEMASK1 (5)	/* Unimplemented Operation
						   (Cause field only).  */


/* FIR fields.  */
#define S_FIR_REVISION          0
#define W_FIR_REVISION          8
#define M_FIR_REVISION          _FPU_MAKEMASK (W_FIR_REVISION, S_FIR_REVISION)
#define G_FIR_REVISION(x)       _FPU_GETVALUE (x, S_FIR_REVISION, \
                                               M_FIR_REVISION)

#define S_FIR_PROCESSORID       8
#define W_FIR_PROCESSORID       8
#define M_FIR_PROCESSORID       _FPU_MAKEMASK (W_FIR_PROCESSORID, \
                                               S_FIR_PROCESSORID)
#define G_FIR_PROCESSORID(x)    _FPU_GETVALUE (x, S_FIR_PROCESSORID, \
                                               M_FIR_PROCESSORID)

#define S_FIR_S                 16
#define W_FIR_S                 1
#define M_FIR_S                 _FPU_MAKEMASK (W_FIR_S, S_FIR_S)
#define G_FIR_S(x)              _FPU_GETVALUE (x, S_FIR_S, M_FIR_S)

#define S_FIR_D                 17
#define W_FIR_D                 1
#define M_FIR_D                 _FPU_MAKEMASK (W_FIR_D, S_FIR_D)
#define G_FIR_D(x)              _FPU_GETVALUE (x, S_FIR_D, M_FIR_D)

#define S_FIR_PS                18
#define W_FIR_PS                1
#define M_FIR_PS                _FPU_MAKEMASK (W_FIR_PS, S_FIR_PS)
#define G_FIR_PS(x)             _FPU_GETVALUE (x, S_FIR_PS, M_FIR_PS)

#define S_FIR_3D                19
#define W_FIR_3D                1
#define M_FIR_3D                _FPU_MAKEMASK (W_FIR_3D, S_FIR_3D)
#define G_FIR_3D(x)             _FPU_GETVALUE (x, S_FIR_3D, M_FIR_3D)

#define S_FIR_IMPL              24
#define W_FIR_IMPL              4
#define M_FIR_IMPL              _FPU_MAKEMASK (W_FIR_IMPL, S_FIR_IMPL)
#define G_FIR_IMPL(x)           _FPU_GETVALUE (x, S_FIR_IMPL, M_FIR_IMPL)


/* FCSR fields.  */
#define S_FCSR_RM               0
#define W_FCSR_RM               2
#define M_FCSR_RM               _FPU_MAKEMASK (W_FCSR_RM, S_FCSR_RM)
#define V_FCSR_RM(x)            _FPU_MAKEVALUE (x, S_FCSR_RM)
#define G_FCSR_RM(x)            _FPU_GETVALUE (x, S_FCSR_RM, M_FCSR_RM)

#define S_FCSR_FLAGS            2
#define W_FCSR_FLAGS            5
#define M_FCSR_FLAGS            _FPU_MAKEMASK (W_FCSR_FLAGS, S_FCSR_FLAGS)
#define V_FCSR_FLAGS(x)         _FPU_MAKEVALUE (x, S_FCSR_FLAGS)
#define G_FCSR_FLAGS(x)         _FPU_GETVALUE (x, S_FCSR_FLAGS, M_FCSR_FLAGS)

#define S_FCSR_ENABLES          7
#define W_FCSR_ENABLES          5
#define M_FCSR_ENABLES          _FPU_MAKEMASK (W_FCSR_ENABLES, S_FCSR_ENABLES)
#define V_FCSR_ENABLES(x)       _FPU_MAKEVALUE (x, S_FCSR_ENABLES)
#define G_FCSR_ENABLES(x)       _FPU_GETVALUE (x, S_FCSR_ENABLES, \
                                               M_FCSR_ENABLES)

#define S_FCSR_CAUSE            12
#define W_FCSR_CAUSE            6
#define M_FCSR_CAUSE            _FPU_MAKEMASK (W_FCSR_CAUSE, S_FCSR_CAUSE)
#define V_FCSR_CAUSE(x)         _FPU_MAKEVALUE (x, S_FCSR_CAUSE)
#define G_FCSR_CAUSE(x)         _FPU_GETVALUE (x, S_FCSR_CAUSE, M_FCSR_CAUSE)

#define S_FCSR_FCC0             23
#define W_FCSR_FCC0             1
#define M_FCSR_FCC0             _FPU_MAKEMASK (W_FCSR_FCC0, S_FCSR_FCC0)
#define V_FCSR_FCC0(x)          _FPU_MAKEVALUE (x, S_FCSR_FCC0)
#define G_FCSR_FCC0(x)          _FPU_GETVALUE (x, S_FCSR_FCC0, M_FCSR_FCC0)

#define S_FCSR_FS               24
#define W_FCSR_FS               1
#define M_FCSR_FS               _FPU_MAKEMASK (W_FCSR_FS, S_FCSR_FS)
#define V_FCSR_FS(x)            _FPU_MAKEVALUE (x, S_FCSR_FS)
#define G_FCSR_FS(x)            _FPU_GETVALUE (x, S_FCSR_FS, M_FCSR_FS)

#define S_FCSR_FCC17            25
#define W_FCSR_FCC17            7
#define M_FCSR_FCC17            _FPU_MAKEMASK (W_FCSR_FCC17, S_FCSR_FCC17)
#define V_FCSR_FCC17(x)         _FPU_MAKEVALUE (x, S_FCSR_FCC17)
#define G_FCSR_FCC17(x)         _FPU_GETVALUE (x, S_FCSR_FCC17, M_FCSR_FCC17)

#define V_FCSR_FCC(x)           (V_FCSR_FCC17 (((x) & 0xfe) >> 1) \
				 | V_FCSR_FCC0 ((x) & 1))
#define G_FCSR_FCC(x)           ((G_FCSR_FCC17 (x) << 1) | G_FCSR_FCC0 (x))


/* FCCR fields.  */
#define S_FCCR_FCC              0
#define W_FCCR_FCC              8
#define M_FCCR_FCC              _FPU_MAKEMASK (W_FCCR_FCC, S_FCCR_FCC)
#define V_FCCR_FCC(x)           _FPU_MAKEVALUE (x, S_FCCR_FCC)
#define G_FCCR_FCC(x)           _FPU_GETVALUE (x, S_FCCR_FCC, M_FCCR_FCC)


/* FEXR fields.  */
#define S_FEXR_FLAGS            2
#define W_FEXR_FLAGS            5
#define M_FEXR_FLAGS            _FPU_MAKEMASK (W_FEXR_FLAGS, S_FEXR_FLAGS)
#define V_FEXR_FLAGS(x)         _FPU_MAKEVALUE (x, S_FEXR_FLAGS)
#define G_FEXR_FLAGS(x)         _FPU_GETVALUE (x, S_FEXR_FLAGS, M_FEXR_FLAGS)

#define S_FEXR_CAUSE            12
#define W_FEXR_CAUSE            6
#define M_FEXR_CAUSE            _FPU_MAKEMASK (W_FEXR_CAUSE, S_FEXR_CAUSE)
#define V_FEXR_CAUSE(x)         _FPU_MAKEVALUE (x, S_FEXR_CAUSE)
#define G_FEXR_CAUSE(x)         _FPU_GETVALUE (x, S_FEXR_CAUSE, M_FEXR_CAUSE)


/* FENR fields.  */
#define S_FENR_RM               0
#define W_FENR_RM               2
#define M_FENR_RM               _FPU_MAKEMASK (W_FENR_RM, S_FENR_RM)
#define V_FENR_RM(x)            _FPU_MAKEVALUE (x, S_FENR_RM)
#define G_FENR_RM(x)            _FPU_GETVALUE (x, S_FENR_RM, M_FENR_RM)

#define S_FENR_FS               2
#define W_FENR_FS               1
#define M_FENR_FS               _FPU_MAKEMASK (W_FENR_FS, S_FENR_FS)
#define V_FENR_FS(x)            _FPU_MAKEVALUE (x, S_FENR_FS)
#define G_FENR_FS(x)            _FPU_GETVALUE (x, S_FENR_FS, M_FENR_FS)

#define S_FENR_ENABLES          7
#define W_FENR_ENABLES          5
#define M_FENR_ENABLES          _FPU_MAKEMASK (W_FENR_ENABLES, S_FENR_ENABLES)
#define V_FENR_ENABLES(x)       _FPU_MAKEVALUE (x, S_FENR_ENABLES)
#define G_FENR_ENABLES(x)       _FPU_GETVALUE (x, S_FENR_ENABLES, \
                                               M_FENR_ENABLES)

void auto_fpu_set_fs (void) __attribute__ ((__constructor__));
void auto_fpu_clear_enables (void) __attribute__ ((__constructor__));

#endif /* _SIBYTE_MIPS_FPU_H */
