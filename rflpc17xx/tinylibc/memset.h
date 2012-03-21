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
/** @file
 * An implementation of memset
 */
#ifndef __RFLPC_MEMSET_H__
#define __RFLPC_MEMSET_H__

#ifdef RFLPC_ENABLE_MEMSET

#include <stddef.h>

/** @ingroup libc
 * @{ */

/**
 * Fills the first n bytes of the memory area pointed to by dest
 * with the constant byte c.
 *
 * @note if the src and dest are aligned on 4 bytes, memset is done by 4 bytes
 * block, so it should be faster if the given pointers are aligned on a 4 byte
 * boundary 
 *
 * @param [out] dest destination buffer
 * @param [in]  c value to set
 * @param [in]  n number of bytes to copy
 *
 * @return
 */
extern void *memset(void *dest, int c, size_t n);

#endif /* ENABLE_MEMSET */

#endif
