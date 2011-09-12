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
  Time-stamp: <2011-09-12 15:47:25 (hauspie)>
*/

#include <stdint.h>
#include <stdlib.h>
#include "../clock.h"
#include "../interrupt.h"
#include "../pinconf.h"

#include "uart.h"

/* For now, we suppose that the CCLK is 96 Mhz. Thus,
   we set PCLK to 12 Mhz by setting it to CCLK/8.
   Then, (p. 315) we have to set 
   DIVADDVAL = 5
   MULVAL = 8
   DLM = 0
   DLL = 4
   UART rate will then be set to 115384
*/

#define RFLPC_UART0_PORT    0
#define RFLPC_UART0_TXD_PIN 2
#define RFLPC_UART0_RXD_PIN 3

int rflpc_uart0_init()
{
    /* Enable UART (user manual, p. 63) */
    LPC_SC->PCONP |= (1UL << 3);
    
    /* set the peripheral clock to 12 Mhz */
    /* First, check if system clock is 96 Mhz */
    if (rflpc_clock_get_system_clock() != 96000000)
	return -1;

    /* set UART CLOCK to 12 Mhz */
    /* Bits 6 and 7 are for UART0, 0x3 value is for CCLK/8 */
    LPC_SC->PCLKSEL0 |= (RFLPC_CCLK_8 << 6);
    
    /* Set line control values (p. 306) */
    /* 8 data bits, no parity, 1 stopbit */
    /* 3 is for 8 data bits, 7 is to enable DLL and DLM registers (for selecting baud rate) */
    LPC_UART0->LCR = (3 | (1 << 7));

    /* Set DLL=4, DLM=0 (p. 315) */
    LPC_UART0->DLL = 4;
    LPC_UART0->DLM = 0;

    /* Set DIVADDVAL = 5 and MULVAL = 8 (p. 315) */
    LPC_UART0->FDR = 5 | (8 << 4);

    /* Enable UART0 Fifo and clear TX & RX (p. 305) */
    LPC_UART0->FCR = 7;

    /* Set pin mode to UART0 (TXD0, RXD0) (p. 108 and p. 299) */
    rflpc_pin_set(0, RFLPC_UART0_RXD_PIN, 1, 0, 0);
    rflpc_pin_set(0, RFLPC_UART0_TXD_PIN, 1, 0, 0);

    /* Reset the DLAB bit in U0LCR to enable access to transmit and receive
     * registers (p. 301) */
    LPC_UART0->LCR &= ~(0x1UL << 7);

    return 0;
}

void rflpc_uart0_set_rx_callback(rflpc_irq_handler_t  callback)
{
    /* set the uart0 interrupt handler */
    rflpc_irq_set_handler(UART0_IRQn, callback);
    /* enable the interrupt vector */
    rflpc_irq_enable(UART0_IRQn);
    /* enable the uart0 irq generation (user manual p. 302) */
    LPC_UART0->IER |= 1;
}
