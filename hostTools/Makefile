ifndef KERNEL_DIR
KERNEL_DIR = ../kernel/linux
endif
ifndef BRCM_BOARD
BRCM_BOARD = bcm963xx
endif
ifndef TARGETS_DIR
TARGETS_DIR = ../targets
endif
ifndef SHARED_DIR
SHARED_DIR = ../shared
endif
ifndef INC_BRCMBOARDPARMS_PATH
INC_BRCMBOARDPARMS_PATH = $(SHARED_DIR)/opensource/boardparms
endif
LZMADIR = ./lzma/compress
INCLUDEDIR = -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I. -I$(LZMADIR)
LZMAOBJS = \
	7zapi.o \
        7zlzma.o \
        AriBitCoder.o \
        IInOutStreams.o \
        LenCoder.o \
        LiteralCoder.o \
        LZMA.o \
        LZMAEncoder.o \
        OutByte.o \
        WindowIn.o

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	else echo sh; fi ; fi)
TOPDIR	:= $(shell /bin/pwd)
SSHD_DIR = $(OPENSOURCE_DIR)/sshd
GENKEY_DIR = $(SSHD_DIR)/genkey
HOST_GENKEY = $(TOPDIR)/host_genkey
CC = gcc
STRIP = strip
RSA_KEY = $(TARGETS_DIR)/fs.src/etc/rsa_host_key
###DSS_KEY = $(TARGETS_DIR)/fs.src/etc/dss_host_key
export SSHD_DIR HOST_GENKEY CC STRIP

#WARNINGS= -Wall

#export DEFS		= -DDEBUG
export CFLAGS		= $(DEFS) $(WARNINGS) -O2 -DGNU $(INCLUDEDIR)

vpath %.cpp $(LZMADIR)

ifeq ($(strip $(BRCM_KERNEL_ROOTFS)),nfs)
all:
else
all: build_mkfs build_cmplzma build_hostgenkey build_imageutil
endif

build_imageutil: bcmImageBuilder addvtoken createimg

bcmImageBuilder: bcmImageBuilder.c
	gcc $(CFLAGS) -o bcmImageBuilder bcmImageBuilder.c
addvtoken: addvtoken.c
	gcc $(CFLAGS) -o addvtoken addvtoken.c 
createimg: createimg.c $(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD)/boardparms.c
	gcc $(CFLAGS) -DCONFIG_BCM9$(BRCM_CHIP) -o createimg createimg.c $(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD)/boardparms.c

ifeq ($(strip $(BRCM_KERNEL_ROOTFS)),squashfs)
build_mkfs: mksquashfs $(LZMAOBJS)
else
ifeq ($(strip $(BRCM_KERNEL_ROOTFS)),cramfs)
build_mkfs: mkcramfs $(LZMAOBJS)
endif
endif

mksquashfs:  squashfs/*.c squashfs/*.h $(LZMAOBJS)
	@if [ ! -e /usr/include/zlib.h ]; then \
          echo *****You need to install zlib-devel package to build mksquashfs! ; \
          echo *****You may find it in your Linux distribution CDs. ; \
	  exit ;\
        fi
	gcc $(CFLAGS) -Isquashfs -c squashfs/*.c
	g++  -o mksquashfs mksquashfs.o read_fs.o sort.o $(LZMAOBJS) -lz

mkcramfs:  cramfs/mkcramfs.cpp $(LZMAOBJS)
	@if [ ! -e /usr/include/zlib.h ]; then \
          echo *****You need to install zlib-devel package to build mkcramfs! ; \
          echo *****You may find it in your Linux distribution CDs. ; \
	  exit ;\
        fi
	g++ $(CFLAGS) -I$(KERNEL_DIR)/include -c cramfs/mkcramfs.cpp
	g++  -o mkcramfs mkcramfs.o $(LZMAOBJS) -lz -lm

build_cmplzma: cmplzma $(LZMAOBJS)

cmplzma: cmplzma.cpp $(LZMAOBJS)
	g++ $(CFLAGS) -c cmplzma.cpp
	g++  -o cmplzma cmplzma.o $(LZMAOBJS) -lm


ifeq ($(strip $(BUILD_CFM_SSHD)),y)
ifeq ($(strip $(BUILD_SSHD_MIPS_GENKEY)),y)
build_hostgenkey:
	@if [ -e $(RSA_KEY) ]; then \
		rm -fr $(RSA_KEY); \
	  exit ;\
        fi
###	@if [ -e $(DSS_KEY) ]; then \
###		rm -fr $(DSS_KEY); \
###	  exit ;\
###        fi
else
build_hostgenkey:	
	@if [ ! -e $(RSA_KEY) ]; then \
		cd $(GENKEY_DIR) && $(MAKE); \
		$(HOST_GENKEY) -t rsa -f $(RSA_KEY); \
	  exit ;\
        fi
###	@if [ ! -e $(DSS_KEY) ]; then \
###		$(HOST_GENKEY) -t dss -f $(DSS_KEY); \
###	  exit ;\
###     fi
endif
else
build_hostgenkey:
	@if [ -e $(RSA_KEY) ]; then \
		rm -fr $(RSA_KEY); \
	  exit ;\
        fi
###	@if [ -e $(DSS_KEY) ]; then \
###		rm -fr $(DSS_KEY); \
###	  exit ;\
###     fi
endif

$(LZMAOBJS): %.o: %.cpp
	g++ -c $(CFLAGS) $^ -o $@

clean:
	rm -f bcmImageBuilder addvtoken createimg mksquashfs mkcramfs cmplzma $(HOST_GENKEY)
	rm -fr *.o core
	rm -fr $(LZMADIR)/*.o $(GENKEY_DIR)/*.o
