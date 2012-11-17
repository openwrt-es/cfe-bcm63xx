/* $Id: sbprof.h,v 1.6 2003/09/02 20:43:33 mpl Exp $ */

/*
 * Copyright 2001, 2002, 2003
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

#ifndef _SIBYTE_SBPROF_H_
#define _SIBTYE_SBPROF_H_

/* sbprof_read_csr, sbprof_write_csr: Read and write 64-bit SOC registers.

   These functions read and write 64-bit SOC registers.

   If __mips64 is defined when compiling the library (i.e., compiling
   for an ABI with 64-bit general purpose registers), these functions
   are defined by the library in terms of the MIPS "ld" and "sd"
   instructions.  Otherwise, the application must provide them in a
   way that is safe for the application's environment.  */
unsigned long long sbprof_read_csr (unsigned long long * /* va */);
void sbprof_write_csr (unsigned long long * /* va */,
		      unsigned long long /* value */);


/* sbprof_init: Initialize the CPU and ZBbus profiling interface

   This routine allocate buffers for data collection and generally
   prepares for profiling.  The buffer sizes given as arguments specify
   how much space will be allocated for storing trace data for CPU
   and ZBbus samples.  Note that in some cases, not all of the space
   provided will be used.  Specifying a given buffer size as zero
   prevents collection of that type of profiling data.

   This function arranges to have a message printed at program exit
   that tells the user where the profiling buffers are located and
   how to upload them to a server (assuming the code is being run
   under CFE).

   If the profiling library is initialized successfully, this
   function returns 0.  On error, it returns -1.

   The sbprof_init2 variant allows the caller to supply some
   run-time startup flags. 

   This function should not be called from an interrupt/exception
   context.  */
int sbprof_init (long /* cpu_bufsize */, long /* zb_bufsize */);
int sbprof_init2 (long /* cpu_bufsize */, long /* zb_bufsize */, 
		  unsigned int flags);

#define SBPROF_FLG_QUIET	0x00000001	/* no noise at exit */

/* This define can be used to select the default buffer size for the
   cpu or zbbus sample buffers.  For CPU samples, the default is
   24MB.  For ZBbus (trace buffer) samples the default is 12MB.  */
#define	SBPROF_BUFSIZE_DEFAULT	-1


/* sbprof_start: Discard any buffered CPU profiling data and
   begin collecting new CPU profiling data.

   This routine arranges to have "hardware interrupt 5" interrupts
   delivered to its own handler (as well as other handlers already
   established), and it enables those interrupts if they are not already
   enabled.

   While profiling is enabled, code should not attempt to use the
   CPU performance counters directly.

   This function should not be called from an interrupt/exception
   context.  */
void sbprof_start (void);


/* sbprof_stop: Stop collecting CPU profiling data.

   This routine stops collecting profiling data, and undoes interrupt
   handler changes made in sbprof_start.  It will disable
   "hardware interrupt 5" interrupts, if they were disabled when
   sbprof_start() was called.

   This function should not be called from an interrupt/exception
   context.  */
void sbprof_stop (void);


/* sbprof_zbprof_start: Start collecting ZBBus profiles.

   This routine arranges to have "hardware interrupt 4" interrupts
   delivered to its own handler (as well as other handlers already
   established), and it enables those interrupts if they are not already
   enabled.  It directs the SCD performance counter interrupts to that
   CPU interrupt line, and uses them to gather ZBBus profile data.

   While profiling is enabled, code should not attempt to use the
   ZBBus trace buffer or SCD performance counters directly.

   This function should not be called from an interrupt/exception
   context.  */
void sbprof_zbprof_start (void);

/* sbprof_zbprof_stop: Stop collecting ZBBus profiling data.

   This routine stops collecting ZBBus profiling data, and undoes
   interrupt handler changes made in sbprof_start.  (Note that it
   leaves the SCD performance counter interrupts pointing to
   hardware interrupt 4.)  It will disable "hardware interrupt 4"
   interrupts, if they were disabled when sbprof_zbprof_start()
   was called.

   This function should not be called from an interrupt/exception
   context.  */
void sbprof_zbprof_stop (void);

/* sbprof_get_zb_buf, sbprof_get_cpu_buf: Get pointers to profile data.

   This routine returns the pointer to the collected data.  If
   the application has the means to save this data to the network,
   it can use the returned pointer and size to get the data
   to be saved.
*/
void sbprof_get_cpu_buf(void **bufpp, size_t *sizep);
void sbprof_get_zb_buf(void **bufpp, size_t *sizep);


/* sbprof_cpu_speed: Returns the cpu clock speed (in hertz).  */
unsigned long long sbprof_cpu_speed (void);

/* sbprof_zbbus_count: Returns the current value of the ZBBus Count
   Register.  (Note that the ZBBus Count Register increments once every
   two CPU cycles.)  */
unsigned long long sbprof_zbbus_count (void);

#endif /* _SIBYTE_SBPROF_H_ */
