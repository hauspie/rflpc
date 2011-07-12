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

#ifndef __RFLPC_CLOCK_H__
#define __RFLPC_CLOCK_H__

#include "LPC17xx.h"


/** get the system clock in Hz */
extern uint32_t rflpc_clock_get_system_clock();
/** init the PLL so that the board is clocked at 96MHz */
extern void rflpc_clock_init(void);

#endif
