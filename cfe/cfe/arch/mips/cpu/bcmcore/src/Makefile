
ALLOBJS += bcmcore_cpuinit.o bcmcore_l1cache.o
ifeq ($(strip ${CFG_RAMAPP}),1)
ALLOBJS += bcmcore_arena.o
endif
CFLAGS += -mips32 -D_MIPSREGS32_ -mabi=32
# CFLAGS += -mips2 -D_MIPSREGS32_

