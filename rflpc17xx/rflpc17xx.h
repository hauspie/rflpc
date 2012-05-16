/* This file is part of rflpc. Copyright 2010-2011 Michael Hauspie
 *
 * rflpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rflpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rflpc.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
 Created:
 Time-stamp: <2012-03-21 15:26:19 (hauspie)>
 */
#ifndef __RFLPC_RFLPC17XX_H__
#define __RFLPC_RFLPC17XX_H__
/** @file
 * Main header file. Can be used to include every needed headers at once.
 */

/** @mainpage rfLPC : a small C library for LPC17xx SoC by NXP
 * 
 * This library is hosted at github: http://github.com/hauspie/rflpc. It is released under GPL licence.
 * @section Authors
 * - Original author: Michael Hauspie <tt>\<michael.hauspie@lifl.fr\></tt>
 * - Additional contributors:
 * 	- Thomas Soete (although not directly, some code parts are directly inspired by his)
 * 	- François Serman (did the refactored UART driver)
 * 
 * More documentation:
 * - @ref start
 * - @ref guidelines
 */

/** @page start Getting started in 10 minutes
 *
 * @tableofcontents
 *
 * @section faq Small FAQ
 * @subsection description What is this library?
 * This library allows development on a LPC17xx board from NXP.
 * The release includes configuration for the MBED platform which uses a LPC1768.
 *
 * @subsection needs What is needed to use rfLPC?
 * In order to compile, you will need a gcc version tuned to build bare metal binaries.
 * I use the one which can be build using the shell script available here: https://github.com/esden/summon-arm-toolchain
 *
 * This release also includes two header files from ARM and NXP that defines C structures to access Cortex M3 and LPC17xx registers.
 *
 * @subsection inside What is provided?
 * This library provides
 * - A linker script
 * - Board initialization code (mainly, data and bss segment initialization, moving the interrupt vector in RAM and configuring the PLL0 to set the CPU clock to something usable (96Mhz for the MBED)
 * - Few libc like functions (memcpy, printf)
 * - Drivers for several peripherals
 *      - Onboard LEDs
 *      - Ethernet 100Mbps
 *      - GPIO
 *      - Repetitive Interrupt Timer
 *      - System Tick Timer
 *      - General purpose Timers
 *      - UART
 * @subsection missing What is missing?
 *
 * A lot of stuff ! Mainly the remainder of drivers for the other devices such as
 * - CAN
 * - DAC/ADC
 * - PWM
 * - ...
 * @subsection use What is using this library ?
 * This library has mainly been developped for two purposes
 * - Enjoying myself
 * - Provide a nice prototyping platform for my research team (you can have a look at my research work here http://www.lifl.fr/~hauspie)
 *
 * For this last purpose, the MBED was a nice and affordable platform
 * although the whole on the cloud compiler stuff was not much what
 * pleases us as the libraries provided by mbed were HUGE and we wanted to release ALL our code open source, even the low level code.
 * The library provided by NXP (CMSIS) was a bit too high level for what we wanted and thus,
 * writing everything from scratch was our best option.
 *
 * The first of our project which has been ported to the MBED is Smews: Smart & Mobile Embedded Web Server (http://www.lifl.fr/2XS/smews)
 * 
 * @section how Environment configuration and building the library
 * 
 * To use the library, you just have to compile it by issuing a @p 'make' in the main folder. It will build the library as well as all the samples.
 * But before that, you have to install an arm compiler and modify the Makefile.vars to set the path and executable names of your compiler.
 * The library should be shipped with the configuration for arm-none-eabi-* tools suite. Modifying @p PREFIX and @p GCC_VERSION should be enough.
 * 
 * @code
 * ## Modify these settings
 * PREFIX=arm-none-eabi
 * GCC_VERSION=
 *
 * ## System commands definitions
 * CC=$(PREFIX)-gcc$(GCC_VERSION)
 * LD=$(CC)
 * AR=$(PREFIX)-ar
 * AS=$(PREFIX)-as
 * OBJCOPY=$(PREFIX)-objcopy
 * OBJDUMP=$(PREFIX)-objdump
 * NM=$(PREFIX)-nm
 * SIZE=$(PREFIX)-size
 * @endcode
 * 
 * If everything builds, then you are ready to use it.
 * 
 * Otherwise, there are few things that you have to check:
 * - Do you have an arm compiler in your path? 
 * 	- is the @p Makefile.vars file modified according to this compiler?
 * - Have you modified the config/config file which has been generated when compiling the library for the first time? 
 * 	- if so, then the library may compile but some samples will not depending on what functionalities you have enabled
 * 
 *  
 * @section first-program Your first program
 * 
 * The easiest way to start your first program is to copy the samples/skel folder and start from here.
 * 
 * In this folder, you will find two files
 * - Makefile
 * - main.c
 * 
 * The Makefile rules how your program is compiled. Here is how it looks
 * @code
 * OUTPUT_NAME=modify_this
 * SRC=$(wildcard *.c)
 * OBJS=$(SRC:.c=.o)
 * 
 * # Modify this variable at your own risk
 * RFLPC_DIR=../..
 * include $(RFLPC_DIR)/Makefile.in
 * @endcode
 * 
 * @li the @p OUTPUT_NAME variable define the name of the final binary. Here it will generate @p modify_this.elf and @p modify_this.bin files
 * @li the @p SRC variable should contain the name of all your @p .c files. These files will be compiled and linked to the final binary
 * @li the @p RFLPC_DIR is the relative path from your folder to the folder that contains the @p rflpc-config file
 * @li the @p include line includes the makefile that does all the magic for you. It contains generic rules for compiling your source files as well as the link rules to generate the @p .elf and @p .bin files
 * 
 * Thus, you should just have to modify @p OUTPUT_NAME and @p SRC variables to create the needed makefile to compile your project.
 * 
 * To compile, just type @p make. You should see something like that
 * 
 * @code
 * $ makefile
 * arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -fno-builtin -ffreestanding -Wall -Winline -O1 -I/home/hauspie/work/git/rflpc -DRFLPC_CONFIG_PLATFORM_MBED -include /home/hauspie/work/git/rflpc/rflpc17xx/config/config.h   -c -o main.o main.c
 * arm-none-eabi-gcc -o modify_this.elf main.o -nostdlib -L/home/hauspie/work/git/rflpc/rflpc17xx -Wl,-T,rflpc17xx.ld,-Map=rflpc.map -lrflpc17xx 
 * arm-none-eabi-objcopy -O binary -j .text -j .data modify_this.elf modify_this.bin 
 * @endcode
 * 
 * If so, then you will have two files, an elf file and a bin file. The elf file is your program in ELF format. You can inspect it, dissassemble it... with commands such as your arm objdump.
 * The bin file is the raw code memory file which is an extract of the @p .text and @p .data section of your elf file. For the MBED platform, it is this file that you have to copy on the USB mass storage drive.
 * 
 * To program your code on the MBED, you can issue a <tt>make program</tt>. This command will try to guess the mountpoint of your MBED (using the output of the @p mount command) and copy the bin file to it. After that, you just have to reset the MBED to actually flash the code.
 * 
 * @section config Fine tuning the library
 * 
 * The library can be configured so that some features are not included. This can save loads of code memory when you just need a few drivers.
 * @subsection config-file Automatic generation of the configuration file
 *
 * The configuration file is located in the rflpc17xx/config folder. When you clone the git repository, this file is NOT included.
 * However, if you just use @p make in the library folder, a default full configuration file is generated
 *
 * This file is a C header file defining the needed MACRO to activate some functionalities
 * To generate the file, you can use the makefile in the config folder. There are two main rules for generating a config file 
 * - <tt>make empty_config</tt>
 * - <tt>make full_config</tt>
 * 
 * The first one generates an empty config file. Then, when the library is compiled it is compiled with the minimum features which are:
 * - board initialization code
 * 	- clock configuration
 * 	- bss and data segment initialization
 * 	- default interrupt setup
 * - interrupt management
 * - Pins configuration
 * - GPIOs
 * - LEDs
 * 
 * Pins configuration, GPIOs and LEDs  are only macros or inlines in the library. Thus, the produced code will only include it if you use it.
 * The library in the minimal configuration is about 800 bytes of code.
 * 
 * The second option (full_config) automatically extracts all the @p RFLPC_CONFIG_ENABLE_xxx macros from the library source code and add it to the config file.
 * Thus, all the functionalities of the library are included. At the moment, this produce a library that is about 8kB of code.
 * 
 * @subsection fine-tune Fine tuning the configuration file
 * 
 * The simplest way to fine tune the library is to start by a make full_config and then remove the line you do not want from the rflpc17xx/config/config-options.h file
 * You can either remove the lines completely or comment them
 * 
 * 
 * @subsection warnings Common configuration mistakes
 * 
 * When fine tuning the configuration be sure to:
 * - Recompile the library AND your program after making a change to the config file. (use make mrproper to clean the library and rebuild it)
 * - Pay attention to dependencies. For example, if you enable printf but not UARTs, the default function used by printf to output its characters will do nothing
 * - Most of the samples will not work if you do not enable at least uarts and printf
 * 
 */

/** @page guidelines Developer guidelines
 * 
 * If you want to contribute to the library, please follow these guidelines to ease the integration
 * 
 * @section contribute How to contribute
 * 
 * - Use your github account to fork the @p hauspie/rflpc repository and push your contribution to your fork. Then, send me a pull-request for integration to the @p develop branch
 * - Always do your development based on the @p develop branch, except if you want to submit me a hotfix of a released version. In the later case, based your work on the master branch, I will take care of integrating the hotfix in the @p develop branch.
 * - Try to do some extensive tests on your code, and, if you can, a bit of code review (which I'll do with you anyway before integrating your patch)
 * 
 * @section conventions Coding conventions
 * 
 * - All symbol names are in lower case, prefixed by @p rflpc_ and a term describing the category of the symbol/function. For example:
 * 	- @p rflpc_eth_xxx for an ethernet driver
 * 	- @p rflpc_spi_xxx for SPI drivers
 * 	- ...
 * - Type names uses the same prefix guidelines and must finish by a @p _t
 * - If possible, prefer static inline functions to macros
 * - All new feature must be able to be enabled/disabled using the config system.
 * 	- you should put your code in a <tt>\#ifdef RFLPC_CONFIG_ENABLE_xxx ... \#endif</tt> block. The chosen name must be explicit
 * - Notable exception to the prefix rule is for function that implements some libc well known functions (::printf, ::memcpy, ::memset...)
 * 
 */

/** @defgroup system Base system */

/** @defgroup drivers Peripheral drivers */

/** @defgroup clock Clock
 *  @ingroup system
 * */

/** @defgroup leds LEDs
 *  @ingroup drivers
 * */

/** @defgroup eth Ethernet
 *  @ingroup drivers
 * */

/** @defgroup libc LIBC tiny subset
 * @ingroup system
 * The rflpc library implement a very small subset of the libc.
 */


/* Arm and NXP includes */
#include "nxp/LPC17xx.h"
#include "nxp/core_cm3.h"

/* Library configuration */
#include "config/config.h"

/* Base system includes */
#include "clock.h"
#include "debug.h"
#include "interrupt.h"
#include "pinconf.h"
#include "profiling.h"
#include "iap.h"


/* Tiny libc */
#include "tinylibc/printf.h"
#include "tinylibc/memcpy.h"
#include "tinylibc/memset.h"
#include "tinylibc/setjmp.h"
#include "tinylibc/rand.h"

/* Peripheral drivers */
#include "drivers/eth_const.h"
#include "drivers/ethernet.h"
#include "drivers/gpio.h"
#include "drivers/leds.h"
#include "drivers/rit.h"
#include "drivers/spi.h"
#include "drivers/sys_tick_timer.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/dma.h"

#endif
