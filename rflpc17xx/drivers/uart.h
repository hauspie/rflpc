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

typedef enum RFLPC_UART_NUMBER { UART0 = 0, UART2 = 2, UART3 = 3 } rflpc_uart_number_e;


/**
 * Inits the UART, using 115200 baud, 8 bits data, no parity and 1 stop bit
 *
 * @return 0 if init is successful, -1 otherwise
 * @note at the moment, -1 is returned if the CPU is not clocked at 96Mhz
 **/
extern int rflpc_uart0_init();

/**
 * Inits the UART e, using 11520 baud, 8 bits data, no parity and 1 stop bit
 *
 * @return 0 if init is successful, -1 otherwise
 * @note at the moment, -1 is returned if the CPU is not clocked at 96Mhz
 **/
extern int rflpc_uart_init(rflpc_uart_number_e uart_num);

/** Sends a byte to the uart0 */
extern void rflpc_uart0_putchar(char c);

extern void rflpc_uart_putchar(rflpc_uart_number_e uart_num, char c);

/** Tells if a byte is available */
extern int rflpc_uart0_byte_available();

extern int rflpc_uart_byte_available(rflpc_uart_number_e uart_num);

/** reads a byte from the uart0 */
extern char rflpc_uart0_getchar();

extern char rflpc_uart_getchar(rflpc_uart_number_e uart_num);

/** set the uart0 rx callback. This enables the uart0 interrupt and set the
 * handler accordingly */
extern void rflpc_uart0_set_rx_callback(rflpc_irq_handler_t callback);

extern void rflpc_uart_set_rx_callback(rflpc_uart_number_e uart_num, rflpc_irq_handler_t callback);
/** @} */

#endif
