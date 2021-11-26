# ChibiOS
## Introduction
![Photo](img/chibios.png)

- Abstracting away timing information. This allows the structure of the  application code to be simplier and smaller.
- Maintainability/Extensibility. Fewer dependencies between modules.
- Task modularity.
- Event-driven means improved effiency.
- Easier power management when idle task is detected.
- Flexible interrupt handling

## How works?

### Requirements

1. Developer ARM Toolchain. 
    + Manual installation > developer.arm.com
    + Package manager > sudo apt-get install gcc-arm-none-eabi.
2. Download specific version of ChibiOS for RPi B
    - [Github repository of ChibiOS-RPi B](github.com/steve-bate/Chibios-Rpi)
3. Prepare Minimum bootable SD-Card for the Raspberry Pi B
    - bootcode.ini - [Download here](data/bootcode.bin)
    - start.elf - [Download here](data/start.elf)
    - It has been said that _loader.bin_ is not necessary, so we have deleted it.

In order to know if the Developer ARM Toolchain is correctly working:

```
arm-none-eabi-gcc --version
```

Expected output:

```
arm-none-eabi-gcc (15:9-2019-q4-0ubuntu2) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### Demos

The different demos can be downloaded from [here](https://cv.udl.cat/access/content/group/103056-2122/Programs_Firmwares/ChibiOS/demos_ChibiOS.zip). You will have to be in `ChibiOS-RPI/demos/<name_demo>`. The directory should have this structure
```
- build/
    - ch.bin
    - ...
- chconf.h
- halconf.h
- main.c
- Makefile
- mcuconf.h
- readme.txt
```
In order to build the binary you must open the terminal and type `make`.

Then, in `build/` directory, it must have been created a `ch.bin` file. Put the file on the **SD card**  and rename it into `kernel.img`.