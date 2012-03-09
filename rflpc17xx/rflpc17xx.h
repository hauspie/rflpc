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
 Time-stamp: <2011-07-13 14:18:07 (hauspie)>
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
 * - SPI
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
 * The first of our project which has been ported to the MBED is Smews: Smart & Mobile Embedded Web Server (http://smews.gforge.inria.fr)
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