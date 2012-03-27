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
  Created: 2012-03-27  
*/
/** @file 
 * A implementation of srand and rand function.
 * This implementation is directly taken from the glibc rand_r function implementation.
 * http://sourceware.org/git/?p=glibc.git;a=blob;f=stdlib/rand_r.c;hb=HEAD
 */

#ifndef __RFLPC_RAND_H__
#define __RFLPC_RAND_H__

#ifdef RFLPC_CONFIG_ENABLE_RAND

/** @ingroup libc
 * @{ */

#ifdef RAND_MAX
#undef RAND_MAX
#endif

/** The maximum value of a random number.
 * The ::rand function generate number in [0, ::RAND_MAX] range.
 */
#define RAND_MAX 2147483647

/** Sets the seed used by ::rand() to generate the pseudo-random sequence.
 * @param seed the seed to use.
 */
extern void srand(unsigned int seed);

/** Returns a pseudo-random integer in the range of 0 to ::RAND_MAX.
 * @warning this function is not reentrant!
 */
extern int rand(void);

/** Returns a pseudo-random integer in the range of 0 to ::RAND_MAX.
 * @note This function is reentrant as it uses a user defined pointer for its seed.
 * @param seed pointer to the current value of the seed.
 */
extern int rand_r(unsigned int *seed);

/** @} */

#endif /* ENABLE_RAND */

#endif
