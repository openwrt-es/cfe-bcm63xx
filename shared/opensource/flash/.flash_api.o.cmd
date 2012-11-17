cmd_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o := /opt/toolchains/uclibc-crosstools_gcc-3.4.2_uclibc-20050502/bin/mips-linux-uclibc-gcc -Wp,-MD,/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/.flash_api.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -I /myviews/ltossey_rel306_sview/CommEngine/kernel/linux/include/asm/gcc -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -Iinclude/asm-mips/mach-bcm963xx -Iinclude/asm-mips/mach-generic  -Os -fomit-frame-pointer -Wdeclaration-after-statement -DCONFIG_BCM96348 -I/myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx -I/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx -DINC_CFI_FLASH_DRIVER=1 -DINC_SPI_FLASH_DRIVER=0   -DKBUILD_BASENAME=flash_api -DKBUILD_MODNAME=flash_api -c -o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.c

deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o := \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.c \
  include/linux/kernel.h \
    $(wildcard include/config/debug/spinlock/sleep.h) \
  /opt/toolchains/uclibc-crosstools_gcc-3.4.2_uclibc-20050502/bin/../lib/gcc/mips-linux-uclibc/3.4.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
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
  include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/bug.h \
  include/asm/break.h \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/bcmtypes.h \
  /myviews/ltossey_rel306_sview/CommEngine/bcmdrivers/opensource/include/bcm963xx/board.h \
    $(wildcard include/config/bcm96348.h) \
    $(wildcard include/config/bcm96358.h) \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx/flash_api.h \

/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o: $(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o)

$(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/flash/flash_api.o):
