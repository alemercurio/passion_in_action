# Embedded Software course

Welcome to our Embedded Systems course!

In this repo you will find some tutorials and code examples. All the software here is developed using the [MIOSIX](https://miosix.org/) toolchain and operating system: for in-depth references see https://miosix.org/doxygen/doxygen_k2.01/index.html.

## Repository Roadmap

This is how this repo is organized:

* docs: 			datasheets and tutorials
* miosix-kernel: 		the kernel
    * arch/.../.../ : 		bsp
* src: sources
    * shared: 			i2c driver, spi driver..
    * examples:
	*<each example>
        	* main.cpp
        	* Makefile     build the examples
        	* bin/         contains a binary that can be flashed on the board
        	* build/
        	* obj/
* install:                     installation stuff
    * Linux/
        * MiosixToolchainInstaller.sh
        * install.sh -> qstLink, udev, gtkterm, add user to dialout
    * Windows/
        * STDrivers
        * MiosixToolchainInstaller
        

## First Setup

* Linux

* [Windows](install/Windows/README.md)

* Mac: just get a virtual machine

## Compiling

* make

## Flashing on an STM board

* STM32 drivers
* QSTLink
* udev rules

* getkTerm/PuTTY (GUI), minicom/screen (cli)
* udev rules
