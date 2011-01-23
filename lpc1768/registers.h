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

#ifndef __LPC1768_REGISTERS_H__
#define __LPC1768_REGISTERS_H__

/** System control and status register (p. 28) */
#define LPC_SCS (*((uint32_t*) 0x400FC1A0UL))

/** Power Control Register (p. 63) */
#define LPC_PCONP (*((uint32_t*) 0x400FC0C4UL))

/** Peripheral clock selection (p. 56) */
#define LPC_PCLKSEL0 (*((uint32_t*) 0x400FC1A8))
#define LPC_PCLKSEL1 (*((uint32_t*) 0x400FC1AC))

#endif
