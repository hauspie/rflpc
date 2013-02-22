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
  Time-stamp: <2013-02-22 10:52:08 (hauspie)>
*/
#ifndef __SIMPLE_NET__
#define __SIMPLE_NET__


extern void simple_net_ethernet_init();

extern void simple_net_set_rx_callback(void (*rx_callback)(uint8_t *data, uint16_t size));

extern uint8_t *simple_net_get_tx_buffer();
extern void simple_net_emit_buffer(uint16_t size);


#endif
