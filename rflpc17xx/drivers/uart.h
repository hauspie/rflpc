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
 * UART0 driver
 */
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2011-07-13 14:39:46 (hauspie)>
*/
#ifndef __RFLPC_UART_H__
#define __RFLPC_UART_H__

#include "../nxp/LPC17xx.h"
#include "../interrupt.h"

/** @addtogroup uart0 UART0
 * @ingroup drivers
 * This is the driver for the UART0. At the moment, this driver is only usable
 * when the CPU is clocked at 96Mhz and the UART mode is set to
 * - 115200 bauds
 * - 8 bits data
 * - no parity check
 * - 1 stop bit
 * @todo Allow fine configuration for the UART. Just have to calculate divider and multiplier depending on cpu clock and desired speed.
 * @todo Extend this driver for using all other UARTs (1/2/3)
 * @{
 */

/**
 * Inits the UART, using 115200 baud, 8 bits data, no parity and 1 stop bit
 *
 * @return 0 if init is successful, -1 otherwise
 * @note at the moment, -1 is returned if the CPU is not clocked at 96Mhz
 **/
extern int rflpc_uart0_init();

/** Sends a byte to the uart0 */
static inline void rflpc_uart0_putchar(char c)
{
    /* Wait for THR to be empty before sending byte (p. 307) */
    while (!(LPC_UART0->LSR & (0x1UL << 5)));
    /* Add byte to fifo */
    LPC_UART0->THR = c & 0xFF;
}

/** Tells if a byte is available */
static inline int rflpc_uart0_byte_available()
{
    return (LPC_UART0->LSR & 0x1UL);
}

/** reads a byte from the uart0 */
static inline char rflpc_uart0_getchar()
{
    /* Wait for the RBR register to receive a byte (p. 307) */
    while (!rflpc_uart0_byte_available());
    /* read the byte from the FIFO */
    return LPC_UART0->RBR & 0xFF;
}

/** set the uart0 rx callback. This enables the uart0 interrupt and set the
 * handler accordingly */
extern void rflpc_uart0_set_rx_callback(rflpc_irq_handler_t callback);

/** @} */

#endif
