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
  Time-stamp: <2012-03-08 16:11:38 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

void configure_timer(void)
{
   rflpc_timer_enable(RFLPC_TIMER0);
   rflpc_timer_set_clock(RFLPC_TIMER0,RFLPC_CCLK_8);
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER0, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */
   rflpc_timer_start(RFLPC_TIMER0);
}

void wait(int micros)
{
   uint32_t start = rflpc_timer_get_counter(RFLPC_TIMER0);
   while ((rflpc_timer_get_counter(RFLPC_TIMER0) - start) < micros);
   return;
}

int broadcast_uarts(int c)
{
    rflpc_uart_putchar(RFLPC_UART0, c);
    rflpc_uart_putchar(RFLPC_UART2, c);
    rflpc_uart_putchar(RFLPC_UART3, c);
    return c;
}

RFLPC_IRQ_HANDLER uart0_irq(void)
{
    while (rflpc_uart_byte_available(RFLPC_UART0))
	printf("Received on UART0: %c\r\n", rflpc_uart_getchar(RFLPC_UART0));
}

RFLPC_IRQ_HANDLER uart2_irq(void)
{
    while (rflpc_uart_byte_available(RFLPC_UART2))
	printf("Received on UART2: %c\r\n", rflpc_uart_getchar(RFLPC_UART2));
}
RFLPC_IRQ_HANDLER uart3_irq(void)
{
    while (rflpc_uart_byte_available(RFLPC_UART3))
	printf("Received on UART3: %c\r\n", rflpc_uart_getchar(RFLPC_UART3));
}


int main()
{
    configure_timer();
    rflpc_uart_init(RFLPC_UART0);
    rflpc_uart_init(RFLPC_UART2);
    rflpc_uart_init(RFLPC_UART3);
    rflpc_uart_set_rx_callback(RFLPC_UART0, uart0_irq);
    rflpc_uart_set_rx_callback(RFLPC_UART2, uart2_irq);
    rflpc_uart_set_rx_callback(RFLPC_UART3, uart3_irq);
    rflpc_printf_set_putchar(broadcast_uarts);
    printf("Hello World\r\n");
    printf("%d %d %d\r\n", RFLPC_UART0, RFLPC_UART2, RFLPC_UART3);
    int i = 0;
    while (1)
    {	
	printf("%d\r\n", i++);
	wait(1000000);
    }
    return 0;
}
