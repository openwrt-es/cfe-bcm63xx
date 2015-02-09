#!/bin/bash
currdir=`pwd`

rm cfe63*.bin
cd cfe/build/broadcom/bcm63xx_rom

make clean
make BRCM_CHIP=6348

cp cfe6348.bin $currdir
cd $currdir/hostTools
gcc -m32 -D _BCM96348_ -I ../shared/opensource/include/bcm963xx/ \
-o nvramembed nvramembed.c ../shared/opensource/boardparms/bcm963xx/boardparms.c

#####################################
# -b board ID
# -n number of maximum mac addresses
# -m ethernet MAC
#####################################
cd $currdir
hostTools/nvramembed -b "96348GW-11" -n 4 -m 00:11:22:33:44:55 -i cfe6348.bin -o cfe6348-nvr.bin

