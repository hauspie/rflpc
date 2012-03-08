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
  Time-stamp: <2011-09-23 11:14:13 (hauspie)>
*/
#ifndef __RFLPC_SETJMP_H__
#define __RFLPC_SETJMP_H__

#include <stdint.h>

typedef uint32_t rflpc_jmp_buf_t[12];

/**
 * Saves  the  stack  context/environment  in  env for later use by ::rflpc_longjmp.
 *
 * @param env 
 * @return 0 if returning directly and non-zero when returning from ::rflpc_longjmp
 **/
extern int rflpc_setjmp(rflpc_jmp_buf_t env);

/**
 * Restores the stack context/environment saved in env by ::rflpc_setjmp.
 *
 * @param env 
 * @param val The value that will be returned by ::rflpc_setjmp when ::rflpc_longjmp is called
 * @return int
 **/

extern int rflpc_longjmp(rflpc_jmp_buf_t env, int val);

#ifdef setjmp
#undef setjmp
#endif

#ifdef longjmp
#undef longjmp
#endif

#ifdef jmp_buf
#undef jmp_buf
#endif

/** Macro to define LIBC compatible prototype for setjmp */
#define setjmp(env) rflpc_setjmp(env)
/** Macro to define LIBC compatible prototype for longjmp */
#define longjmp(env, val) rflpc_longjmp(env,val)
/** LIBC compliant set_jmp type*/
#define jmp_buf rflpc_jmp_buf_t

#endif