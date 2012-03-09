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
  Time-stamp: <2011-10-27 16:37:19 (hauspie)>
*/
#include "memcpy.h"
#include <stdint.h>

/* This memcpy uses 2 optimisations:

   1- If address are aligned on a 4 byte boundary, it perform as much 4 bytes copy as it can. Then, it copies byte per byte
   2- Each copy phase (word and byte), is unrolled using Duff's device

   It may be further optimized by either
   1- copy byte per byte until a 4 byte boundary aligned is found (if both buffers are similarly aligned)
   2- try to compose words from multiple word aligned reads on the source to write word aligned on destination
*/
#define IS_WORD_ALIGNED(a) (!((uint32_t)(a) & 0x3))

static void *rflpc_memcpy_unaligned_fast(void *dest, const void *src, size_t n)
{    
    uint8_t *bdest = dest;
    const uint8_t *bsrc = src;    
    int count = (n+7) / 8;
    switch (n%8)
    {
	case 0: do { *bdest++ = *bsrc++;
	    case 7: *bdest++ = *bsrc++;
	    case 6: *bdest++ = *bsrc++;
	    case 5: *bdest++ = *bsrc++;
	    case 4: *bdest++ = *bsrc++;
	    case 3: *bdest++ = *bsrc++;
	    case 2: *bdest++ = *bsrc++;
	    case 1: *bdest++ = *bsrc++;
	} while (--count > 0);
    }
    return dest;
}

static void *rflpc_memcpy_aligned_fast(void *dest, const void *src, size_t n)
{    
    uint32_t *wdest = dest;
    const uint32_t *wsrc = src;
    int count = ((n/4)+7)/8; /* loop iteration count */
    switch ((n/4)%8)
    {
	 case 0: do { *wdest++ = *wsrc++;
            case 7: *wdest++ = *wsrc++;
            case 6: *wdest++ = *wsrc++;
            case 5: *wdest++ = *wsrc++;
            case 4: *wdest++ = *wsrc++;
            case 3: *wdest++ = *wsrc++;
            case 2: *wdest++ = *wsrc++;
            case 1: *wdest++ = *wsrc++;
            } while (--count > 0);
    }
    n = n % 4; /* remainder of copy */    
    if (n)
	rflpc_memcpy_unaligned_fast(wdest,wsrc,n);
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    if (n == 0)
	return dest;
    if (IS_WORD_ALIGNED(dest) && IS_WORD_ALIGNED(src))
	return rflpc_memcpy_aligned_fast(dest, src, n);   
   return rflpc_memcpy_unaligned_fast(dest, src, n);
}
