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
  Time-stamp: <2013-02-22 11:04:44 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#include "simple_net.h"

void packet_in(uint8_t *data, uint16_t size)
{
    int i;
    printf("Received packet\r\n");

    for (i = 0 ; i < size ; ++i)
    {
	if (i % 16 == 0)
	{
	    printf("\r\n");
	}
	printf("%02x ", data[i]);
    }
    printf("\r\n");
}

int main()
{
    rflpc_uart_init(RFLPC_UART0);
    
    simple_net_ethernet_init();
    simple_net_set_rx_callback(packet_in);

    while (1)
    {
	int i;
	uint8_t *tx;
/*	tx = simple_net_get_tx_buffer();
	for (i = 0 ; i < 6 ; ++i)
	    tx[i] = 0xff;
	for (i = 6 ; i < 30 ; ++i)
	    tx[i] = rand() & 0xff;
	    simple_net_emit_buffer(30, 1);*/
    }
    return 0;
}
