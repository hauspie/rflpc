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
  Time-stamp: <2012-12-17 10:28:30 (hauspie)>

  This file contains some macro tools used in rflpc

*/
/** @file
 * Macro tools for easy implementation of other macros.
 */
#ifndef __RFLPC_TOOLS_H__
#define __RFLPC_TOOLS_H__

/** Concatenate two tokens together */
#define TOKEN_PASTE2(x, y) x ## y

/** This macro allows concatenation of tokens even in recursive macro */
#define TOKEN_PASTE(x, y) TOKEN_PASTE2(x, y)


/** Sets a given bit to 1 */
#define RFLPC_SET_BIT(val, bit) (val) |= (1UL << (bit))
/** Clears a given bit to 0 */
#define RFLPC_CLR_BIT(val, bit) (val) &= ~(1UL << (bit))

/** Sets some bits values from the position of the LSB bit and the number of bits to set.
    The bit_val is the values of the bits BEFORE shift
*/
#define RFLPC_SET_BITS_VAL(val, bit_pos, bit_val, bit_count) do {	\
    uint32_t mask = ((1 << (bit_count)) - 1) << (bit_pos);		\
    /* Clear bits */							\
    (val) &= ~mask;							\
    (val) |= ((bit_val) << (bit_pos)) & mask;				\
} while(0)

#endif
