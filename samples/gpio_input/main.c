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
  Time-stamp: <2015-03-19 11:43:08 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define PIN MBED_DIP14

int main()
{
    rflpc_uart_init(RFLPC_UART0);

    printf("Testing GPIO input\r\n");

    rflpc_gpio_set_pin_mode_input(PIN,RFLPC_PIN_MODE_RESISTOR_PULL_DOWN);
    uint32_t val = rflpc_gpio_get_pin(PIN);
    while (1)
    {
       uint32_t current = rflpc_gpio_get_pin(PIN);
       if (current != val)
       {
          printf("Joystick is %s\r\n", current ? "pressed" : "released");
          val = current;
       }
    }
    return 0;
}
