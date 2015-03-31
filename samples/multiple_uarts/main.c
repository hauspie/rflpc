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
  Time-stamp: <2015-03-31 16:53:38 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>


#define UART RFLPC_UART2

int main()
{
   rflpc_uart_init(RFLPC_UART0);
   rflpc_uart_init_ex(UART, 52, 0, 1, 2); /* use this UART at 9600 bauds */
   
   while (1)
   {
      RFLPC_DELAY(1000);
      printf("Check UART2 (DIP27/28)\r\n");
      rflpc_uart_putchar(UART, 'a');
      rflpc_uart_putchar(UART, '\r');
      rflpc_uart_putchar(UART, '\n');
      rflpc_uart_putchar(UART, 'b');
      rflpc_uart_putchar(UART, '\r');
      rflpc_uart_putchar(UART, '\n');
   }
   return 0;
}
