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

/** @mainpage rfLPC Documentation
 *
 * @section description What is this library?
 * This library allows development on a LPC17xx board from NXP.
 * The release includes configuration for the MBED platform which uses a LPC1768.
 *
 * @section needs What is needed to use rfLPC?
 * In order to compile, you will need a gcc version tuned to build bare metal binaries.
 * I use the one which can be build using the shell script available here: https://github.com/esden/summon-arm-toolchain
 *
 * This release also includes two header files from ARM and NXP that defines C structures to access Cortex M3 and LPC17xx registers.
 *
 * @section inside What is provided?
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
 * @section missing What is missing?
 *
 * A lot of stuff ! Mainly the remainder of drivers for the other devices such as
 * - CAN
 * - DAC/ADC
 * - PWM
 * - ...
 * @section use What is using this library ?
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
 * @section how How to use it
 * 
 * To use the library, you just have to compile it by issuing a 'make' in the main folder. It will build the library as well as all the samples.
 * If everything builds, then you are ready to use it.
 * 
 * Otherwise, there are few things that you have to check:
 * - Do you have an arm compiler in your path? 
 * 	- is the Makefile.vars file modified according to this compiler?
 * - Have you modified the config/config file which has been generated when compiling the library for the first time? 
 * 	- if so, then the library may compile but some samples will not depending on what functionalities you have enabled
 * 
 * @section config Fine tuning of the library
 * 
 * The library can be configured so that some features are not included. This can save loads of code memory when you just need a few drivers.
 * @subsection config-file Automatic generation of the configuration file
 *
 * The configuration file is located in the config/config folder. When you clone the git repository, this file is NOT included.
 * However, if you just use make in the library folder, a default full configuration file is generated
 *
 * This file is a list of defines that will be enabled at compile time. 
 * The file is read by the rflpc-config script when generating the cflags. 
 * Each line represents a define that will be transformed to a -Dxxxx flag.
 * 
 * To generate the file, you can use the makefile in the config folder. There are two main rules for generating a config file 
 * - make empty_config
 * - make full_config
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
 * The second option (full_config) automatically extracts all the RFLPC_CONFIG_ENABLE_xxx macros from the library source code and add it to the config file.
 * Thus, all the functionalities of the library are included. At the moment, this produce a library that is about 8kB of code.
 * 
 * @subsection fine-tune Fine tuning the configuration file
 * 
 * The simplest way to fine tune the library is to start by a make full_config and then remove the line you do not want from the config/config file. 
 * You can either remove the lines completely or use the '#' character to make a line comment "à la" sh
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


/* Base system includes */
#include "config.h"
#include "clock.h"
#include "debug.h"
#include "interrupt.h"
#include "pinconf.h"
#include "profiling.h"


/* Tiny libc */
#include "tinylibc/printf.h"
#include "tinylibc/memcpy.h"
#include "tinylibc/memset.h"
#include "tinylibc/setjmp.h"

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
