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

#include "LPC17xx.h" /* for IRQn enum */
#include "interrupt.h"
#include "debug.h"
/* use a special section to put at start of ram (see link script) */
static volatile lpc_interrupt_handler_t _ram_interrupts[IRQn_COUNT] __attribute__ ((section(".ram_int_vector")));
/* this array is defined in init.c and is the rom interrupt vector */
extern void* _rom_interrupts[IRQn_COUNT];

LPC_IRQ_HANDLER _default_exception_handler()
{
    /* stops the execution with a O--O <-> -OO- led pattern. */
    LPC_STOP(LED1|LED4, 2000000);
}

LPC_IRQ_HANDLER _default_peripheral_handler()
{
    /* stops the execution with a --OO <-> OO-- led pattern. */
    LPC_STOP(LED1|LED2, 2000000);
}

void lpc_init_interrupts()
{
    /* copy the interrupt vector */
    int i;
    for (i = 0 ; i < IRQn_COUNT ; ++i)
    {
	_ram_interrupts[i] = (lpc_interrupt_handler_t) _rom_interrupts[i];
    }

    /* Set the VTOR register to remap the interrupt vector */
    lpc_disable_irq();
    SCB->VTOR = (uint32_t) _ram_interrupts;
    lpc_enable_irq();
}

void lpc_set_handler(IRQn_Type irq, lpc_interrupt_handler_t handler)
{
    _ram_interrupts[irq] = handler;
}
