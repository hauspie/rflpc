/* This file is part of rfBareMbed.                        
 *									 
 * rfBareMbed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rfBareMbed is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rfBareMbed.  If not, see <http://www.gnu.org/licenses/>.	 
 */

#ifndef __LPC_DEBUG_H__
#define __LPC_DEBUG_H__

#include "leds.h"

/** wait until a counter reaches c. Used to wait some time. Depends on CPU frequency */
#define LPC_DELAY(c) do {int i; for (i=0 ; i < (c) ; ++i); } while (0)

/** Stops execution by an infinite loop, switching between led pattern l and its opposite */
#define LPC_STOP(l,c) do {uint32_t leds = (l); while(1){LED_VAL(leds);LPC_DELAY((c));leds=~leds;}} while(0)

#endif
