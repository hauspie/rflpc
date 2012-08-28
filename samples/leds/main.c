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
  Time-stamp: <2012-08-28 12:32:26 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

int main()
{
    int j,i = 0;
    rflpc_led_init();
    

    while (1)
    {
	rflpc_led_binary_value((i++)%16);
	for (j = 0 ; j < 1000000 ; ++j);
    }
    return 0;
}
