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
*/

#ifdef RFLPC_CONFIG_ENABLE_MEMSET

#include "memset.h"
#include <stdint.h>
/* This memset uses 2 optimisations:

   1- If address are aligned on a 4 byte boundary, it perform as much 4 bytes sets as it can. Then, it sets byte per byte
   2- Each set phase (word and byte), is unrolled using Duff's device   
*/
#define IS_WORD_ALIGNED(a) (!((uint32_t)(a) & 0x3))

static void *rflpc_memset_unaligned_fast(void *dest, int c, size_t n)
{    
    uint8_t *bdest = dest;    
    int count = (n+7) / 8;
    
    switch (n%8)
    {
	case 0: do { *bdest++ = c;
	    case 7: *bdest++ = c;
	    case 6: *bdest++ = c;
	    case 5: *bdest++ = c;
	    case 4: *bdest++ = c;
	    case 3: *bdest++ = c;
	    case 2: *bdest++ = c;
	    case 1: *bdest++ = c;
	} while (--count > 0);
    }
    return dest;
}

static void *rflpc_memset_aligned_fast(void *dest, int c, size_t n)
{    
    uint32_t *wdest = dest;
    const uint32_t val = c << 24 | c << 16 | c << 8 | c;
    int count = ((n/4)+7)/8; /* loop iteration count */
    
    switch ((n/4)%8)
    {
	 case 0: do { *wdest++ = val;
            case 7: *wdest++ = val;
            case 6: *wdest++ = val;
            case 5: *wdest++ = val;
            case 4: *wdest++ = val;
            case 3: *wdest++ = val;
            case 2: *wdest++ = val;
            case 1: *wdest++ = val;
            } while (--count > 0);
    }
    n = n % 4; /* remainder of copy */    
    if (n)
	rflpc_memset_unaligned_fast(wdest,c,n);
    return dest;
}

void *memset(void *dest, int c, size_t n)
{
    uint8_t *bdest = dest;
    if (n == 0)
	return dest;
    c &= 0xFF;
    while (n && !IS_WORD_ALIGNED(bdest))
    {
	*bdest++ = c;
	--n;
    }
    if (n)    
	return rflpc_memset_aligned_fast(bdest, c, n);   
   return dest;
}

#endif /* ENABLE_MEMSET */
