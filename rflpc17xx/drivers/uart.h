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
  Time-stamp: <2012-03-08 16:13:23 (hauspie)>
*/
#ifndef __RFLPC_UART_H__
#define __RFLPC_UART_H__

#include "../nxp/LPC17xx.h"
#include "../interrupt.h"

/** @addtogroup uart UART
 * @ingroup drivers
 * This is the driver for the UART0/2/3. At the moment, this driver is only usable
 * when the CPU is clocked at 96Mhz and the UART mode is set to
 * - 115200 bauds
 * - 8 bits data
 * - no parity check
 * - 1 stop bit
 * @todo Allow fine configuration for the UART. Just have to calculate divider and multiplier depending on cpu clock and desired speed.
 * @{
 */

typedef enum { RFLPC_UART0 = 0, RFLPC_UART2 = 2, RFLPC_UART3 = 3 } rflpc_uart_t;


/**
 * Inits the UART e, using 11520 baud, 8 bits data, no parity and 1 stop bit.
 *
 * @return 0 if init is successful, -1 otherwise
 * @note at the moment, -1 is returned if the CPU is not clocked at 96Mhz
 **/
extern int rflpc_uart_init(rflpc_uart_t uart_num);

/** Sends a byte to the uart.
 *  @param uart_num the uart to use
 *  @param c character to send
 */
extern void rflpc_uart_putchar(rflpc_uart_t uart_num, char c);

/** Test the reception FIFO.
 *  @param uart_num the uart to use
 *  @return true if a byte is available
 */
extern int rflpc_uart_byte_available(rflpc_uart_t uart_num);

/** 
 * Gets a character from the uart.
 * @param uart_num the uart to use
 * @return the character read.
 * @warning This function loops while no data is available!
 */
extern char rflpc_uart_getchar(rflpc_uart_t uart_num);

/** Sets the uart rx callback.  
 *  This enables the uart0 interrupt and set the handler accordingly 
 *  @param uart_num the uart to use
 *  @param callback a pointer to a ::rflpc_irq_handler_t function
 */
extern void rflpc_uart_set_rx_callback(rflpc_uart_t uart_num, rflpc_irq_handler_t callback);

/** @} */

#endif
