#!/bin/bash
currdir=`pwd`

if [ -z "$1" ] || [ -z "$2" ];
then
    echo 'Usage: build.sh <BOARD_NAME> <CHIP_NAME>'
    echo 'Example: ./build_38.sh 96338L-2M-8M 6338'
    exit 1
fi

BRCM_CHIP=$2
BCM_CC_FLAG=_BCM9${BRCM_CHIP}_
echo $BCM_CC_FLAG
BOARD_NAME=$1

rm cfe63*.bin
cd cfe/build/broadcom/bcm63xx_rom

make clean
make BRCM_CHIP=${BRCM_CHIP}

cp cfe${BRCM_CHIP}.bin $currdir
cd $currdir/hostTools
gcc -m32 -D ${BCM_CC_FLAG} -I ../shared/opensource/include/bcm963xx/ \
-o nvramembed nvramembed.c ../shared/opensource/boardparms/bcm963xx/boardparms.c

#####################################
# -b board ID
# -n number of maximum mac addresses
# -m ethernet MAC
#####################################
cd $currdir
hostTools/nvramembed -b ${BOARD_NAME} -n 4 -m 00:11:22:33:44:55 -i cfe${BRCM_CHIP}.bin -o cfe${BRCM_CHIP}-nvr.bin

