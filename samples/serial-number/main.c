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
  Created: 2012-03-28  
*/
#include <rflpc17xx/rflpc17xx.h>

int main()
{
    unsigned long serial[4];
    rflpc_uart_init(RFLPC_UART0);
    if (rflpc_iap_get_serial_number(serial) != 0)    
	printf("Failed to retrieve serial number\r\n");
    else    
	printf("My serial number: %x %x %x %x\r\n", serial[0], serial[1], serial[2], serial[3]);	    
    while (1);
    return 0;
}

/* 1818050e 534e1755 4b8cea31 f5000003 */
