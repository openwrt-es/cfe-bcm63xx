#!/bin/bash
currdir=`pwd`

rm cfe63*.bin
cd cfe/build/broadcom/bcm63xx_rom

make clean

cd $currdir/hostTools
make clean
rm nvramcrc nvramembed
