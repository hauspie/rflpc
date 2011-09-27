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
  Time-stamp: <2011-09-27 17:27:59 (hauspie)>
*/
#include "nxp/LPC17xx.h" /* for IRQn enum */
#include "config.h"
#include "interrupt.h"

#ifdef RFLPC_IRQ_DEBUG_ENABLE
#include "printf.h"
#include "debug.h"
#endif

/* use a special section to put at start of ram (see link script) */
static volatile rflpc_irq_handler_t _ram_interrupts[RFLPC_IRQn_COUNT] __attribute__ ((section(".ram_int_vector")));
/* this array is defined in init.c and is the rom interrupt vector */
extern void* _rom_interrupts[RFLPC_IRQn_COUNT];

RFLPC_IRQ_HANDLER _default_exception_handler()
{
#ifdef RFLPC_IRQ_DEBUG_ENABLE
    RFLPC_ASSERT_STACK();
    printf("CFSR: %x\r\n", SCB->CFSR);
    printf("HFSR: %x\r\n", SCB->HFSR);
    printf("DFSR: %x\r\n", SCB->DFSR);
    RFLPC_DUMP_STACK();
    /* stops the execution with a O--O <-> -OO- led pattern. */
    RFLPC_STOP(RFLPC_LED_1|RFLPC_LED_4, 2000000);
#endif
}

RFLPC_IRQ_HANDLER _default_peripheral_handler()
{
#ifdef RFLPC_IRQ_DEBUG_ENABLE
    /* stops the execution with a --OO <-> OO-- led pattern. */
    RFLPC_STOP(RFLPC_LED_1|RFLPC_LED_2, 2000000);
#endif
}

void rflpc_irq_init()
{
    /* copy the interrupt vector */
    int i;
    for (i = 0 ; i < RFLPC_IRQn_COUNT ; ++i)
    {
	_ram_interrupts[i] = (rflpc_irq_handler_t) _rom_interrupts[i];
    }

    /* Set the VTOR register to remap the interrupt vector */
    rflpc_irq_global_disable();
    SCB->VTOR = (uint32_t) _ram_interrupts;
    rflpc_irq_global_enable();
}

void rflpc_irq_set_handler(IRQn_Type irq, rflpc_irq_handler_t handler)
{
    /* +16 because of the system exception handler enum starting at -14 */
    _ram_interrupts[irq+16] = handler;
}
