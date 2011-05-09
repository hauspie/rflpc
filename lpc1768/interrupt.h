/* This file is part of rfBareMbed.                        
 *									 
 * rfBareMbed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rfBareMbed is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rfBareMbed.  If not, see <http://www.gnu.org/licenses/>.	 
 */

#ifndef __LPC_INTERRUPT_H__
#define __LPC_INTERRUPT_H__

#include "LPC17xx.h"
#include "printf.h"

/** Enables interrupts (p. 716) */
static inline void lpc_enable_irq()
{
    __asm volatile("cpsie i");
}

/** Disable interrupts (p. 716) */
static inline void lpc_disable_irq()
{
    __asm volatile("cpsid i");
}

/** Enables a specific interrupt */
static inline void lpc_enable_interrupt(IRQn_Type irq)
{
    int bit = irq - WDT_IRQn;

    printf("setting bit %d for IRQ %d\r\n", bit, irq);
    
    NVIC->ISER[bit >> 5] |= (1 << bit);
}

/** Disables a specific interrupt */
static inline void lpc_disable_interrupt(IRQn_Type irq)
{
    int bit = irq - WDT_IRQn;

    NVIC->ICER[bit >> 5] |= (1 << bit);
}

/* Interrupt handler type */
typedef void (*lpc_interrupt_handler_t)();

/* Define to use to declare a IRQ handler */
#define LPC_IRQ_HANDLER void __attribute__((interrupt("IRQ")))

/* Inits the interruption by relocating the interrupt vector to ram */
void lpc_init_interrupts();

/* Sets an handler for the given IRQ */
void lpc_set_handler(IRQn_Type irq, lpc_interrupt_handler_t handler);


#endif
