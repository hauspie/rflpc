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
/*
  This files defines the driver for the Repetitive Interrupt Timer (user manual, p. 501)
 */

#ifndef __RFLPC_RIT_H__
#define __RFLPC_RIT_H__

#include "../LPC17xx.h"
#include "../interrupt.h"

/** Enables the RIT. By default, it is turned off. See PCONP register, user manual, p.63 */
static inline void rflpc_rit_enable()
{
    LPC_SC->PCONP |= (1UL << 16);
}

/** Gets the current value of the rit counter */
static inline uint32_t rflpc_rit_get_counter_value() { return LPC_RIT->RICOUNTER; }

/** Sets the current value of the rit counter */
static inline void rflpc_rit_set_counter_value(uint32_t value) { LPC_RIT->RICOUNTER = value; }

/** clears the pending interrupt. Should be called at the end of your callback
 * to prevent the interruption to be generated again */
static inline void rflpc_rit_clear_pending_interrupt() { LPC_RIT->RICTRL |= 1; }

/** sets the RIT interrupt callback and configure the RIT interrupt.
    The interrupt handler will be called when the counter value reaches value for the bits zeroed in the mask
    If reset on match is true, then when the counter match the comp_value, the counter is reset to 0
    
    For example, if you want the timer to be called each time the 7th bit is equal to one:
    - comp_value should be set to 1 << 7
    - mask should be set to ~(1 << 7)
*/
extern void rflpc_rit_set_callback(uint32_t comp_value, uint32_t mask, char reset_on_match,  rflpc_irq_handler_t c);


#endif
