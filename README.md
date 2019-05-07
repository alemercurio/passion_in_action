# Embedded Software course

Welcome to our Embedded Systems course!

In this repo you will find some tutorials and code examples. All the software here is developed using the [MIOSIX](https://miosix.org/) toolchain and operating system: for in-depth references see https://miosix.org/doxygen/doxygen_k2.01/index.html.

## Setup

* [Linux](install/Linux/README.md)
* [Windows](install/Windows/README.md)
* Mac: there's a guide to compile the kernel from binaries [here](https://miosix.org/wiki/index.php?title=OSX_Quick_Start). If it doesn't work, just get a virtual machine :)

## Repository Roadmap

This what you will find in this repo:

```
 _ docs/ 					    datasheets and tutorials
|_ miosix-kernel/ 				the kernel of the OS
|   |_ arch/cortexM4_stm32f4/stm32f407vg_stm32f4discovery  bsp&stage1boot
|_ src/						
|    |_ shared/ 				i2c driver, spi driver..
|    |_ < in each folder >
|         |_ main.cpp     		MAIN of the example
|      	  |_ Makefile     		builds the binary
|      	  |_ bin/         		contains a binary that can be flashed on the board
|      	  |_ build/
|         |_ obj/
|_ install/                     		
     |_ Linux/
     |    |_ install.sh 			script to install dependencies 
     |    |_ README                             Install instructions
     |_ Windows/
          |_ drivers/				ST drivers
	      |_ README                 Install instructions
```

## Compiling and Flashing

Each folder inside `src/examples/<your example>` contains a Makefile to build the example. You can compile by opening a command prompt (or git bash) in that folder and typing `make`.

If the binary was compiled properly, you will find it in the `bin/` folder. To flash it on a board, you can use `STLink` on Windows or `QSTLink` on Linux and select the new generated binary to upload it. 

Remember to reset after you flashed!

## Reading from Serial Port

On STM32F407 discovery boards the USB is also usable as Serial Port. To connect to it, you can use `PuTTY` or `GTKTerm`.

## On Windows

First identify your device by opening the command prompt and typing `mode`. You should see some device named `COMxx`.

Open `PuTTY`, choose `Serial`, set baudrate to 19200 and port to what you found previously (`COMxx`).

Altenatively, you can use Arduino's IDE Serial Monitor.

## On Linux

Open `GtkTerm` and go to `Configuration > port`. Choose `/dev/ttyUSB0` as port, set baudrate to 19200 leave everything else deafault.
