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

#ifndef __LPC_PLL_H__
#define __LPC_PLL_H__

#include "LPC17xx.h"


typedef struct
{
    __IO uint32_t CON;
    __IO uint32_t CFG;
    __I uint32_t STAT;
    __O uint32_t FEED;
} LPC_PLL0_TypeDef;

#define LPC_PLL0_BASE (0x400FC080UL)

#define LPC_PLL0      ((LPC_PLL0_TypeDef *) LPC_PLL0_BASE)

/** Send a feed sequence to the PLL0FEED register (p. 40) 
    This sequence validates the changes in the PLL0CON and PLL0CFG register
    @warning interrupts should be disabled to ensure that the sequence is atomic
 */
#define LPC_PLL0_DO_FEED() do { LPC_PLL0->FEED = 0xAA; LPC_PLL0->FEED = 0x55; }while(0)


extern uint32_t lpc_get_system_clock();

#endif
