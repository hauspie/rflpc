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
  Time-stamp: <2012-03-21 09:30:23 (hauspie)>
*/
#ifdef RFLPC_CONFIG_ENABLE_SETJMP

#include "setjmp.h"


int setjmp(jmp_buf env)
{
    int ret = 0;
    asm volatile(
	"mov 	r0, %[env]		\n\t"
	/* Backup all registers, except r0-r3 and pc */
	/* sp, can not be used in stmia's reglist so use r3 instead */
	"mov	r3, sp			\n\t"
	"stmia	r0!, {r3-r12,lr}	\n\t"	
	:
	: [env] "r" (env)
	);
    return ret;
}

int longjmp(jmp_buf env, int val)
{    
    asm volatile(
	"mov	r0, %[env]	\n\t"
	"ldmia	r0!, {r3-r12,lr}	\n\t"
	"mov	sp, r3		\n\t"
	: 
	: [env] "r" (env)
	);
    return val;
}

#endif /* ENABLE_SETJMP */
