	
CFG_MLONG64  ?= 0
CFG_LITTLE   ?= 0        # Mindbender
CFG_RELOC    ?= 0
CFG_UNCACHED ?= 0      # Mindbender
CFG_NEWRELOC ?= 0
CFG_VAPI     ?= 0
CFG_BOOTRAM  ?= 0
CFG_RAMAPP   ?= 0       # EPI
CFG_BOARDNAME = "BCM9$(BRCM_CHIP)"
CFG_PCI      ?= 0
CFG_ZLIB     ?= 0           # enable zlib
CFG_SBP		 ?= 0		# use Single Board Parameter if defined to 1

TOP   = ../../../cfe
ARCH  = mips
CPU   = bcmcore
BOARD = bcm63xx_rom

HOSTTOOLS = ../../../../hostTools
TARGETS_DIR = ../../../../targets
SHARED_DIR = ../../../../shared
INC_SHARED_PUB_PATH = $(SHARED_DIR)/opensource/include/bcm963xx
INC_SHARED_PRIV_PATH = $(SHARED_DIR)/broadcom/include/bcm963xx

BOOT_DIR = ${ARCH_TOP}/board/$(BOARD)/src
ASM_FILE = flashimg.S
RAM_BUILD = ../bcm63xx_ram

# TOOLS=/usr/local/sbtools/x86-linux-rh6.0/sb1-elf-2.5.11/bin/
TOOLS=/opt/specifix/broadcom_2004d_341/i686-pc-linux-gnu/bin/
#TOOLS=../../../../broadcom_2004d_341/i686-pc-linux-gnu/bin/
#TOOLS=/opt/toolchains/uclibc-crosstools/mips-linux/bin/
include $(TOP)/arch/mips/board/$(BOARD)/src/rom_cfe.mk

INCDIRS += -I${ARCH_TOP}/board/bcm63xx_ram/include -I${INC_SHARED_PUB_PATH} -I${INC_SHARED_PRIV_PATH}

BSPOBJS += flashimg.o

CRTIOBJS = /opt/toolchains/uclibc-crosstools/mips-linux/lib/crti.o

ALL : sanity_check cfe$(BRCM_CHIP)
	gcc  -o ${HOSTTOOLS}/addvtoken -I ${INC_SHARED_PUB_PATH} -I/usr/include  ${HOSTTOOLS}/addvtoken.c
	@if [ $(shell find . -name cfe$(BRCM_CHIP).bin -printf '%s\n') -le 65536 ] ; then \
		${HOSTTOOLS}/addvtoken cfe$(BRCM_CHIP).bin bcm9$(BRCM_CHIP)_cfe.w; \
	else \
		find . -name cfe$(BRCM_CHIP).bin -printf '\nERROR: cfe.bin size, %s, is greater than 65536 maximum size.\n'; \
		rm cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin; \
	fi
	@echo done

sanity_check:
	@if [ "$(BRCM_CHIP)" = "" ]; then \
          echo You must specify chip ID using 'gmake BRCM_CHIP=[6338|6348|6358]'; exit 1; \
	fi

mksbp: ALL
	@echo done

sbp:
	${MAKE} clean
	${MAKE} CFG_SBP?=1 mksbp
	
cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin : $(CRT0OBJS) $(BSPOBJS) $(LIBCFE) $(ASM_FILE)
	$(GLD) -o cfe$(BRCM_CHIP) -Map cfe$(BRCM_CHIP).map $(LDFLAGS) $(CRT0OBJS) $(BSPOBJS) -L. -lcfe $(LDLIBS)
	$(OBJDUMP) -d cfe$(BRCM_CHIP) > cfe$(BRCM_CHIP).dis
	$(OBJCOPY) --output-target=binary cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin
	
$(ASM_FILE) : $(TOP)/arch/mips/board/bcm63xx_ram/src 
	rm -f $(ASM_FILE)
	$(MAKE) -C $(RAM_BUILD)

clean :
	rm -f *.o *~ cfe* *.w
	rm -f build_date.c
	rm -f libcfe.a
	$(MAKE) -C $(RAM_BUILD) clean
	
distclean : clean

release :
	$(MAKE) clean
	$(MAKE) BRCM_CHIP=6338
	cp -f cfe*.bin $(TARGETS_DIR)/cfe
	$(MAKE) clean
	$(MAKE) BRCM_CHIP=6348
	cp -f cfe*.bin $(TARGETS_DIR)/cfe
	$(MAKE) clean
	$(MAKE) BRCM_CHIP=6358
	cp -f cfe*.bin $(TARGETS_DIR)/cfe
	$(MAKE) clean
