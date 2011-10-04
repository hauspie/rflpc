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
  Time-stamp: <2011-08-31 16:16:02 (hauspie)>
*/
/** @file
 * Debug functions such as delay and stack dump.
 */
#ifndef __RFLPC_DEBUG_H__
#define __RFLPC_DEBUG_H__

#include "drivers/leds.h"

#include "config.h"

extern unsigned char _stack[RFLPC_STACK_SIZE];

/** @addtogroup debug Debug macro
 * @ingroup system
 * @{ */

/** wait until a counter reaches c.
 *  Used to wait some time.
 * @note Depends on CPU frequency
 */
#define RFLPC_DELAY(c) do {     \
   int i;uint32_t j;            \
   for (i=0 ; i < (c) ; ++i)    \
      j = LPC_SC->SCS;          \
   } while (0)

/** Stops execution by an infinite loop, switching between led pattern l and its opposite. */
#define RFLPC_STOP(l,c) do {    \
   uint32_t leds = (l);         \
   while(1) {                   \
      rflpc_led_val(leds);      \
      RFLPC_DELAY((c));         \
      leds=~leds;               \
      }                         \
   }                            \
   while(0)

/** Dumps 64 bytes from the current value of the stack pointer.
 * @note this maccro uses ::rflpc_printf()
 * */
#define RFLPC_DUMP_STACK() do {                                 \
	register uint8_t *mstack =(uint8_t*) __get_MSP();       \
	register int i;                                         \
	for (i = 0 ; i < 64 ; ++i)                              \
	{                                                       \
	    if (i % 16 == 0)                                    \
		printf("\n\r%p: ", mstack  + i);                \
	    printf("%02x ", mstack[i]);                         \
	}                                                       \
	printf("\n\r");                                         \
    } while(0)

/** @} */
#endif
