cmd_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o := /opt/toolchains/uclibc-crosstools_gcc-3.4.2_uclibc-20050502/bin/mips-linux-uclibc-gcc -Wp,-MD,/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/.boardparms.o.d -nostdinc -iwithprefix include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -I /myviews/ltossey_rel306_sview/CommEngine/kernel/linux/include/asm/gcc -G 0 -mno-abicalls -fno-pic -pipe  -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap -Iinclude/asm-mips/mach-bcm963xx -Iinclude/asm-mips/mach-generic  -Os -fomit-frame-pointer -Wdeclaration-after-statement -DCONFIG_BCM96348 -I/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx   -DKBUILD_BASENAME=boardparms -DKBUILD_MODNAME=boardparms -c -o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.c

deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o := \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.c \
    $(wildcard include/config/bcm96338.h) \
    $(wildcard include/config/mdio.h) \
    $(wildcard include/config/mdio/pseudo/phy.h) \
    $(wildcard include/config/bcm96348.h) \
    $(wildcard include/config/spi/ssb/0.h) \
    $(wildcard include/config/spi/ssb/1.h) \
    $(wildcard include/config/bcm96358.h) \
  /myviews/ltossey_rel306_sview/CommEngine/shared/opensource/include/bcm963xx/boardparms.h \
    $(wildcard include/config/gpio.h) \
    $(wildcard include/config/spi/ssb/2.h) \
    $(wildcard include/config/spi/ssb/3.h) \

/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o: $(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o)

$(deps_/myviews/ltossey_rel306_sview/CommEngine/shared/opensource/boardparms/bcm963xx/boardparms.o):
