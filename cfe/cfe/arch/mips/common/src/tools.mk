#
# Basic compiler options and preprocessor flags
#

CFLAGS += -g -c  -ffreestanding 
CFLAGS += -Os -Wall -Werror -Wstrict-prototypes -Wmissing-prototypes 

#
# Tools locations
#

GCC     ?= $(TOOLS)sb1-elf-gcc
GLD     ?= $(TOOLS)sb1-elf-ld
AR      ?= $(TOOLS)sb1-elf-ar
OBJDUMP ?= $(TOOLS)sb1-elf-objdump
OBJCOPY ?= $(TOOLS)sb1-elf-objcopy
RANLIB  ?= $(TOOLS)sb1-elf-ranlib

#
# Check for 64-bit mode
#

ifeq ($(strip ${CFG_MLONG64}),1)
  CFLAGS += -mlong64 -D__long64
endif

#
# Figure out which linker script to use
#

ifeq ($(strip ${CFG_RAMAPP}),1)
   CFLAGS += -DCFG_RAMAPP=1
   LDFLAGS = -g --script ${ARCH_SRC}/cfe_ramapp.lds
   CFLAGS += -DCFG_RUNFROMKSEG0=1
else
  CFLAGS += -membedded-pic
  ifeq ($(strip ${CFG_RELOC}),0)
    ifeq ($(strip ${CFG_BOOTRAM}),1)
      CFLAGS += -DCFG_BOOTRAM=1
      ROMRAM = ram
    else
      CFLAGS += -DCFG_BOOTRAM=0
      ROMRAM = rom
    endif
    ifeq ($(strip ${CFG_UNCACHED}),1)
      CFLAGS += -DCFG_RUNFROMKSEG0=0
      LDFLAGS = -g --script ${ARCH_SRC}/cfe_${ROMRAM}_uncached.lds
    else
      CFLAGS += -DCFG_RUNFROMKSEG0=1
      LDFLAGS = -g --script ${ARCH_SRC}/cfe_${ROMRAM}_cached.lds
    endif
  else
    CFLAGS += -membedded-pic -mlong-calls -DCFG_EMBEDDED_PIC=1
    ifeq ($(strip ${CFG_UNCACHED}),1)
      CFLAGS += -DCFG_RUNFROMKSEG0=0
      LDFLAGS = -g --script ${ARCH_SRC}/cfe_rom_reloc_uncached.lds --embedded-relocs
    else
      CFLAGS += -DCFG_RUNFROMKSEG0=1
      ifeq ($(strip ${CFG_TEXTAT1MB}),1)
        LDFLAGS = -g --script ${ARCH_SRC}/cfe_rom_reloc_cached_biendian.lds --embedded-relocs
      else
        LDFLAGS = -g --script ${ARCH_SRC}/cfe_rom_reloc_cached.lds --embedded-relocs
      endif
    endif
  endif
endif
#
# Determine target endianness
#

ifeq ($(strip ${CFG_LITTLE}),1)
  ENDIAN = -EL
  CFLAGS += -EL
  LDFLAGS += -EL
else
  ENDIAN = -EB
  CFLAGS += -EB
  LDFLAGS += -EB
endif

