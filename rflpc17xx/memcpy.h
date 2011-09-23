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
  Time-stamp: <2011-09-23 10:46:31 (hauspie)>
*/
#ifndef __RFLPC_MEMCPY_H__
#define __RFLPC_MEMCPY_H__

#include <stdint.h>

/** 
 * @brief Copies memory from one buffer to another
 *
 * @note if the src and dest are aligned on 4 bytes, copy is done by 4 bytes
 * block, so it should be faster if the given pointers are aligned on a 4 byte
 * boundary
 *
 * @param dest 
 * @param src 
 * @param n 
 * 
 * @return 
 */
extern void *memcpy(void *dest, const void *src, uint32_t n);

#endif
