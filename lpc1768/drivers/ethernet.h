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
#ifndef __RFLPC_ETHERNET_H__
#define __RFLPC_ETHERNET_H__

/*
  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Jun. 28 2011
  Time-stamp: <2011-07-03 00:32:33 (mickey)>
*/ 


/** Inits the ethernet device */
extern int rflpc_eth_init();

/** Returns the link state
    @return true if link is up, false if link is down
*/
extern int rflpc_eth_link_state();

/** functions that print information on chip and link.
    Will be removed when driver is functional, only used as debug function
*/
extern void rflpc_eth_print_infos();

#endif
