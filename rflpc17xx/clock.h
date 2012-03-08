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
/** @file
 * Clock initialization and defines.
 * @addtogroup clock
 * @ingroup system
 * @{
 */
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2011-09-12 15:47:02 (hauspie)>
*/
#ifndef __RFLPC_CLOCK_H__
#define __RFLPC_CLOCK_H__

#include "nxp/LPC17xx.h"


/**
   This enums defines the different cpu clock dividers for use as peripheral
   clocks.
   (See doc p. 57)
 */
typedef enum
{
    RFLPC_CCLK_4 = 0,		/**< Divide the CPU clock by 4. */
    RFLPC_CCLK   = 1,           /**< Use the CPU clock.         */
    RFLPC_CCLK_2 = 2,           /**< Divide the CPU clock by 2. */
    RFLPC_CCLK_8 = 3,           /**< Divide the CPU clock by 8. */
} rflpc_clock_divider_t;

/** returns the system clock in Hz */
extern uint32_t rflpc_clock_get_system_clock();
/** init the PLL0 to clock the board at its operational speed.
 * This function will use the defines:
 * - ::RFLPC_CLOCK_USE_MAIN_OSCILLATOR
 * - ::RFLPC_CLOCK_MAIN_OSCILLATOR_FREQUENCY
 * - ::RFLPC_CLOCK_INPUT_DIVIDER
 * - ::RFLPC_CLOCK_PLL_MULTIPLIER
 * - ::RFLPC_CLOCK_CPU_DIVIDER
 */
extern void rflpc_clock_init(void);
/** @} */
#endif
