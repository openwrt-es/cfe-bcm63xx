
CFG_MLONG64  ?= 0
CFG_LITTLE   ?= 0        # Mindbender
CFG_RELOC    ?= 0
CFG_UNCACHED ?= 0      # Mindbender
CFG_NEWRELOC ?= 0
CFG_VAPI     ?= 0 
CFG_BOOTRAM  ?= 0
CFG_RAMAPP   ?= 1
CFG_BOARDNAME = "BCM9$(BRCM_CHIP)"
CFG_PCI       = 0
CFG_ZLIB	  = 0		# enable zlib

COMPRESS ?= cmplzma

TOP   = ../../../cfe
ARCH  = mips
CPU   = bcmcore
BOARD = bcm63xx_ram

HOSTTOOLS = ../../../../hostTools
SHARED_DIR = ../../../../shared
BOARDPARMS_DIR = $(SHARED_DIR)/opensource/boardparms/bcm963xx
FLASH_DIR = $(SHARED_DIR)/opensource/flash
INC_SHARED_PUB_PATH = $(SHARED_DIR)/opensource/include/bcm963xx
INC_SHARED_PRIV_PATH = $(SHARED_DIR)/broadcom/include/bcm963xx
ROM_BUILD = ../bcm63xx_rom
ASM_FILE =$(ROM_BUILD)/flashimg.S

ifeq ($(strip $(CFG_SBP)),1)
	CFLAGS += -DUSE_SINGLE_BOARD_PARAM 
endif	

# TOOLS=/usr/local/sbtools/x86-linux-rh6.0/sb1-elf-2.5.11/bin/
#TOOLS=/opt/specifix/broadcom_2004d_341/i686-pc-linux-gnu/bin/
TOOLS=../../../../broadcom_2004d_341/i686-pc-linux-gnu/bin/
include $(TOP)/arch/mips/board/$(BOARD)/src/ram_cfe.mk

VPATH += ${ARCH_TOP}/board/$(BOARD)/src/html ${BOARDPARMS_DIR} ${FLASH_DIR}
INCDIRS += -I${INC_SHARED_PUB_PATH} -I${INC_SHARED_PRIV_PATH}

$(HOSTTOOLS)/$(COMPRESS):
	$(MAKE) -C $(HOSTTOOLS) build_cmplzma

ALL: sanity_check $(HOSTTOOLS)/$(COMPRESS)
	find $(SHARED_DIR) -name "*.o" -exec rm -f "{}" ";"
	find $(BOARDPARMS_DIR) -name "*.o" -exec rm -f "{}" ";"
	${MAKE} cfe$(BRCM_CHIP)
	$(HOSTTOOLS)/$(COMPRESS) -t -c -2 cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin $(ASM_FILE)

sanity_check:
	@if [ "$(BRCM_CHIP)" = "" ]; then \
          echo You must specify chip ID using 'gmake BRCM_CHIP=[6338|6348|6358]'; exit 1; \
	fi

$(ASM_FILE): cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin

cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin: $(CRT0OBJS) $(BSPOBJS) $(LIBCFE)
	$(GLD) -o cfe$(BRCM_CHIP) -Map cfe$(BRCM_CHIP).map $(LDFLAGS) $(CRT0OBJS) $(BSPOBJS) -L. -lcfe $(LDLIBS)
	$(OBJDUMP) -d cfe$(BRCM_CHIP) > cfe$(BRCM_CHIP).dis
	$(OBJCOPY) --output-target=binary cfe$(BRCM_CHIP) cfe$(BRCM_CHIP).bin

clean :
	rm -f *.o *~ cfe*
	rm -f build_date.c
	rm -f libcfe.a
	rm -f $(ASM_FILE)
	
distclean : clean
