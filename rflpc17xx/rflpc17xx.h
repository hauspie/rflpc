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
#include "memcpy.h"
#include "pinconf.h"
#include "printf.h"
#include "profiling.h"


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

#endif