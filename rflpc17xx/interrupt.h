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
  Time-stamp: <2011-07-13 14:35:39 (hauspie)>
*/

/** @file */

#ifndef __RFLPC_INTERRUPT_H__
#define __RFLPC_INTERRUPT_H__

#include "nxp/LPC17xx.h"

/** @addtogroup irq Interruption management
 * @ingroup system
 * @{
 */

/** Enables interrupts. (user manual p. 716) */
static inline void rflpc_irq_global_enable()
{
    __asm volatile("cpsie i");
}

/** Disable interrupts. (user manual p. 716) */
static inline void rflpc_irq_global_disable()
{
    __asm volatile("cpsid i");
}

/** Enables a specific interrupt. */
static inline void rflpc_irq_enable(IRQn_Type irq)
{
    NVIC->ISER[irq >> 5] |= (1 << irq);
}

/** Disables a specific interrupt. */
static inline void rflpc_irq_disable(IRQn_Type irq)
{
    NVIC->ICER[irq >> 5] |= (1 << irq);
}

/** Interrupt handler type */
typedef void (*rflpc_irq_handler_t)();

/** Define to use to declare a IRQ handler */
#define RFLPC_IRQ_HANDLER void __attribute__((interrupt("IRQ")))

/** Inits the interruption by relocating the interrupt vector to ram.
 * @warning This is automatically called by the _start routine. Do not call it in your program.
 */
void rflpc_irq_init();

/** Sets an handler for the given IRQ */
void rflpc_irq_set_handler(IRQn_Type irq, rflpc_irq_handler_t handler);

/** @} */

#endif
