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
  Time-stamp: <2012-03-21 09:30:02 (hauspie)>
*/
#ifndef __RFLPC_SETJMP_H__
#define __RFLPC_SETJMP_H__

#ifdef RFLPC_CONFIG_ENABLE_SETJMP

#include <stdint.h>

typedef uint32_t jmp_buf[12];

/**
 * Saves  the  stack  context/environment  in  env for later use by ::rflpc_longjmp.
 *
 * @param env 
 * @return 0 if returning directly and non-zero when returning from ::rflpc_longjmp
 **/
extern int setjmp(jmp_buf env);

/**
 * Restores the stack context/environment saved in env by ::rflpc_setjmp.
 *
 * @param env 
 * @param val The value that will be returned by ::rflpc_setjmp when ::rflpc_longjmp is called
 * @return int
 **/

extern int longjmp(jmp_buf env, int val);

#endif /* ENABLE_SETJMP */

#endif
