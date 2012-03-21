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
  Time-stamp: <2012-03-21 09:21:56 (hauspie)>
*/
#ifdef RFLPC_ENABLE_RIT_TIMER

#include "rit.h"

void rflpc_rit_set_callback(uint32_t comp_value, uint32_t mask, int reset_on_match, rflpc_irq_handler_t c)
{
    LPC_RIT->RIMASK = mask;
    LPC_RIT->RICOMPVAL = comp_value;
    if (reset_on_match) /* Timer enable clear */
	LPC_RIT->RICTRL |= 1 << 1;

    rflpc_irq_set_handler(RIT_IRQn, c);
    rflpc_irq_enable(RIT_IRQn);
}

#endif /* ENABLE_RIT_TIMER */
