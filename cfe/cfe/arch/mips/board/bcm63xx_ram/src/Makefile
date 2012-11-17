ifeq ($(strip ${CFG_BOARDNAME}),"BCM96338")
BSPOBJS +=               \
    dev_bcm6348_eth.o
endif
ifeq ($(strip ${CFG_BOARDNAME}),"BCM96348")
BSPOBJS +=               \
    dev_bcm6348_eth.o
endif
ifeq ($(strip ${CFG_BOARDNAME}),"BCM96358")
BSPOBJS +=               \
    dev_bcm6348_eth.o
endif

BSPOBJS +=               \
	bcmmii.o			\
	dev_bcm63xx_uart.o  \
	dev_bcm63xx_flash.o \
	flash_api.o         \
	bcm63xx_ram_boot.o  \
	bcm63xx_devs.o      \
	bcm63xx_board.o     \
	bcm63xx_cmd.o       \
	bcm63xx_util.o		\
	bcm63xx_ldr_raw.o	\
	bcm63xx_ldr_elf.o	\
	bcm63xx_main.o		\
	bcm63xx_env_subr.o	\
	bcm63xx_net_icmp.o  \
	bcm63xx_httpd.o     \
	boardparms.o

ifeq ($(strip ${INC_CFI_FLASH_DRIVER}),1)
BSPOBJS +=               \
	cfiflash.o
endif

ifeq ($(strip ${INC_SPI_FLASH_DRIVER}),1)
BSPOBJS +=               \
	spiflash.o
endif

ifeq ($(strip ${CFG_WEB_SERVER}),1)
BSPOBJS +=               \
	ul.o				\
	ulinfo.o
endif

