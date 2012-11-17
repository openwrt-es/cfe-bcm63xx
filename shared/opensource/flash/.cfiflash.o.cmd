cmd_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o := /opt/toolchains/uclibc-crosstools_gcc-3.4.2_uclibc-20050502/bin/mips-linux-uclibc-gcc -Wp,-MD,/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/.cfiflash.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -I /myviews/ltossey_rel306_sview/CommEngine/kernel/linux/include/asm/gcc -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -Iinclude/asm-mips/mach-bcm963xx -Iinclude/asm-mips/mach-generic  -Os -fomit-frame-pointer -Wdeclaration-after-statement -DCONFIG_BCM96348 -I/myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx -I/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx -DINC_CFI_FLASH_DRIVER=1 -DINC_SPI_FLASH_DRIVER=0   -DKBUILD_BASENAME=cfiflash -DKBUILD_MODNAME=cfiflash -c -o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.c

deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o := \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.c \
    $(wildcard include/config/bcm96338.h) \
    $(wildcard include/config/bcm96358.h) \
  include/linux/param.h \
    $(wildcard include/config/bcm/endpoint/module.h) \
  include/asm/param.h \
  include/asm-mips/mach-generic/param.h \
    $(wildcard include/config/mips/brcm.h) \
  include/linux/sched.h \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/linux/capability.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/asm/posix_types.h \
  include/asm/sgidefs.h \
  include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/mips64.h) \
    $(wildcard include/config/lbd.h) \
  include/linux/compiler.h \
  include/linux/compiler-gcc3.h \
  include/linux/compiler-gcc.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
  include/linux/preempt.h \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/linux/thread_info.h \
  include/linux/bitops.h \
  include/asm/bitops.h \
    $(wildcard include/config/mips32.h) \
  include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/cpu-features.h \
  include/asm/cpu.h \
  include/asm/cpu-info.h \
    $(wildcard include/config/sgi/ip27.h) \
  include/linux/cache.h \
  include/linux/kernel.h \
    $(wildcard include/config/debug/spinlock/sleep.h) \
  /opt/toolchains/uclibc-crosstools_gcc-3.4.2_uclibc-20050502/bin/../lib/gcc/mips-linux-uclibc/3.4.2/include/stdarg.h \
  include/asm/bug.h \
  include/asm/break.h \
  include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  include/asm-mips/mach-bcm963xx/cpu-feature-overrides.h \
  include/asm/war.h \
    $(wildcard include/config/sgi/ip22.h) \
    $(wildcard include/config/sni/rm200/pci.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/sb1/pass/1/workarounds.h) \
    $(wildcard include/config/sb1/pass/2/workarounds.h) \
    $(wildcard include/config/mips/malta.h) \
    $(wildcard include/config/mips/atlas.h) \
    $(wildcard include/config/mips/sead.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/momenco/jaguar/atx.h) \
    $(wildcard include/config/pmc/yosemite.h) \
  include/asm/system.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/has/wb.h) \
  include/asm/addrspace.h \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/mips64.h) \
    $(wildcard include/config/cpu/r8000.h) \
    $(wildcard include/config/cpu/r10000.h) \
  include/asm-mips/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/asm/ptrace.h \
  include/asm/isadep.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  include/asm/hazards.h \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/processor.h \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
  include/asm/cachectl.h \
  include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
  include/asm/prefetch.h \
  include/linux/stringify.h \
  include/linux/timex.h \
    $(wildcard include/config/time/interpolation.h) \
  include/asm/timex.h \
  include/asm-mips/mach-generic/timex.h \
  include/linux/time.h \
  include/linux/seqlock.h \
  include/asm/div64.h \
  include/linux/jiffies.h \
  include/linux/rbtree.h \
  include/linux/cpumask.h \
    $(wildcard include/config/hotplug/cpu.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
  include/asm/string.h \
  include/asm/semaphore.h \
  include/asm/atomic.h \
  include/linux/wait.h \
  include/linux/list.h \
  include/linux/prefetch.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/asm/page.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/limited/dma.h) \
  include/asm/mmu.h \
  include/linux/smp.h \
  include/linux/sem.h \
    $(wildcard include/config/sysvipc.h) \
  include/linux/ipc.h \
  include/asm/ipcbuf.h \
  include/asm/sembuf.h \
  include/linux/signal.h \
  include/asm/signal.h \
    $(wildcard include/config/binfmt/irix.h) \
  include/asm/sigcontext.h \
  include/asm/siginfo.h \
    $(wildcard include/config/compat.h) \
  include/asm-generic/siginfo.h \
  include/linux/securebits.h \
  include/linux/fs_struct.h \
  include/linux/completion.h \
  include/linux/pid.h \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/.h) \
  include/linux/gfp.h \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
  include/linux/numa.h \
  include/linux/topology.h \
  include/asm/topology.h \
  include/asm-mips/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/kmalloc_sizes.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/large/allocs.h) \
  include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/resource.h \
  include/asm/resource.h \
  include/linux/timer.h \
  include/linux/aio.h \
  include/linux/workqueue.h \
  include/linux/aio_abi.h \
  include/asm/current.h \
  include/asm/delay.h \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/bcm_map_part.h \
    $(wildcard include/config/bcm96348.h) \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx/6348_map_part.h \
    $(wildcard include/config/type.h) \
    $(wildcard include/config/timer.h) \
    $(wildcard include/config/timer/retry/mask.h) \
    $(wildcard include/config/timer/trdy/mask.h) \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/bcmtypes.h \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/bcmtypes.h \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/board.h \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx/flash_api.h \

/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o: $(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o)

$(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/cfiflash.o):
