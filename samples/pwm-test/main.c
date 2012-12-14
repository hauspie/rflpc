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
  Time-stamp: <2012-12-14 16:38:23 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define LED_PIN MBED_DIP26

int main()
{
#ifdef RFLPC_CONFIG_ENABLE_UART
    rflpc_uart_init(RFLPC_UART0);
#endif

#ifdef RFLPC_CONFIG_ENABLE_PRINTF
    printf("Hello World\r\n");
#endif

    rflpc_gpio_use_pin(LED_PIN);
    rflpc_gpio_set_pin_mode_output(LED_PIN);


    while (1)
    {
	int i;
	rflpc_gpio_set_pin(LED_PIN);
	for (i = 0 ; i < 20000000 ; ++i);
	rflpc_gpio_clr_pin(LED_PIN);
	for (i = 0 ; i < 20000000 ; ++i);
    }
    return 0;
}
