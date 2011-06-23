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

#ifndef __RFLPC_UART_H__
#define __RFLPC_UART_H__

#include "../LPC17xx.h"
#include "../interrupt.h"

/* Inits the UART, using 115200 baud, 8 bits data, no parity and 1 stop bit */

/* returns 0 if successful, -1 otherwise */

extern int rflpc_uart0_init();

/** Sends a byte to the uart0 */
static inline void rflpc_uart0_putchar(char c)
{
    /* Wait for THR to be empty before sending byte (p. 307) */
    while (!(LPC_UART0->LSR & (0x1UL << 5)));
    /* Add byte to fifo */
    LPC_UART0->THR = c & 0xFF;
}

/** reads a byte from the uart0 */
static inline char rflpc_uart0_getchar()
{
    /* Wait for the RBR register to receive a byte (p. 307) */
    while (!(LPC_UART0->LSR & 0x1UL));
    /* read the byte from the FIFO */
    return LPC_UART0->RBR & 0xFF;
}

/** set the uart0 rx callback. This enables the uart0 interrupt and set the
 * handler accordingly */
extern void rflpc_uart0_set_rx_callback(rflpc_irq_handler_t callback);

#endif
