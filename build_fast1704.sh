#!/bin/bash
currdir=`pwd`

rm cfe63*.bin
cd cfe/build/broadcom/bcm63xx_rom

make clean
make BRCM_CHIP=6338

cp cfe6338.bin $currdir
cd $currdir/hostTools
gcc -m32 -D _BCM96338_ -I ../shared/opensource/include/bcm963xx/ \
-o nvramembed nvramembed.c ../shared/opensource/boardparms/bcm963xx/boardparms.c

#####################################
# -b board ID
# -n number of maximum mac addresses
# -m ethernet MAC
#####################################
cd $currdir
hostTools/nvramembed -b "F@ST1704" -n 4 -m 7c:03:4c:08:07:d5 -i cfe6338.bin -o cfe6338-nvr.bin

