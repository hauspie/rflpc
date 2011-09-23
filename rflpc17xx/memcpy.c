/* This file is part of rflpc.                        
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
  Created: 2011-09-23
  Time-stamp: <2011-09-23 10:54:44 (hauspie)>
*/
#include <stdint.h>

void *memcpy(void *dest, const void *src, uint32_t n)
{
    uint32_t *wdest = (uint32_t*)dest;
    const uint32_t *wsrc = (const uint32_t*)src;
    uint8_t *bdest;
    const uint8_t *bsrc;
    if (!((uint32_t)wsrc & 0x3) && !((uint32_t)wdest & 0x3)) /* If aligned on 4 byte boundary */
    {
	while (n >= 4)
	{
	    *wdest++ = *wsrc++;
	    n -= 4;
	}
    }
    bdest = (uint8_t*) wdest;
    bsrc = (const uint8_t*) wsrc;
    while (n)
    {
	*bdest++ = *bsrc++;
	n--;
    }
    return dest;
}
