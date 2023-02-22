# BCM63xx CFE Bootloader

This is the build system for the CFE BCM63XX bootloader.
To build CFE bootloader, the following steps are required:

## Install dependencies:
```bash
$ sudo apt install gcc make libstdc++5 linux-libc-dev:i386 unzip libz-dev
```
Clone the CFE from 
```bash
$ git clone https://github.com/javad123javad/cfe-bcm63xx
```
Run the `build.sh` file based on your chip id and board name. At the moment the following chips are supported: 
* BCM6338(32)
* BCM6348
* BCM6358

If you don’t know the configuration of your modem's hardware, type `generic` as the board name
For example, to build the CFE for `BCM6338`, run the `build.sh` script as follows:
```bash
$ ./build.sh 96338L-2M-8M 6338
```

If everything goes well, the CFE bootloader will be available in the parent directory. There are two bin files, one is `cfe{CHIP_ID}.bin` and another is `cfe_{CHIP_ID}-nvr.bin`. The `nvr` file contains the board’s hardware-related information (e.g. MAC address, board name, etc…)

Credits
-------
Broadcom

@danitool

@Noltari

@javad123javad
