BCM63xx CFE Bootloader
======================
This is the buildsystem for the CFE BCM63XX bootloader.

You need to have installed gcc, make, libstdc++5, unzip, libz-dev and libc headers.

Building
--------
Go to the main directory for building:

cd cfe/build/broadcom/bcm63xx_rom

make clean

make BRCM_CHIP=6338

make BRCM_CHIP=6348

make BRCM_CHIP=6358

Building Script
---------------
First edit build.sh to match your board's ID and MAC addresses

./build.sh

Credits
-------
Broadcom

@danitool

@Noltari
