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

#include "uart.h"
#include "printf.h"

/* use a special section to put at start of ram (see link script) */
static volatile lpc_interrupt_handler_t _interrupts[IRQn_COUNT] __attribute__ ((section(".ram_int_vector")));


extern char _interrupt_start;
extern char _interrupt_end;

LPC_IRQ_HANDLER _default_handler()
{
    /* empty default handler */
    lpc_uart0_putchar('I');
    lpc_uart0_putchar('N');
    lpc_uart0_putchar('T');
    lpc_uart0_putchar('\r');
    lpc_uart0_putchar('\n');
}

void lpc_init_interrupts()
{
    /* copy the interrupt vector */
    char *src = &_interrupt_start;
    char *dst = (char *) _interrupts;
    while (src < &_interrupt_end)
	*dst++ = *src++;

    /* Set the VTOR register to remap the interrupt vector */
    lpc_disable_irq();
    SCB->VTOR = (uint32_t) _interrupts;
    lpc_enable_irq();
}

void lpc_set_handler(IRQn_Type irq, lpc_interrupt_handler_t handler)
{
    printf("Setting handler for %d (%p)\r\n", irq, _interrupts[irq]);
    _interrupts[irq] = handler;
}





