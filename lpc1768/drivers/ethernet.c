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
  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Jun. 28 2011
  Time-stamp: <2011-06-28 23:26:59 (mickey)>
*/

#include "ethernet.h"
#include "../LPC17xx.h"

#define PCENET_BIT (1 << 30)
#define PINSEL2_BITS_MASK (0xF03F030F)
#define PINSEL2_BITS (0x50150105)


#define SOFT_RESET_BIT (1 << 15)


int rflpc_eth_init()
{
    /* Power the ethernet device
       Set bit PCENET in PCONP register (p. 141 and 64) */
    LPC_SC->PCONP |= PCENET_BIT;
    
    /* Configure Ethernet PINS through PINSEL2 register (p.141 and 109) 
       We use RMII interface to connect MAC to PHY.
       Thus, the needed pins are:
       ENET_TXD[0:1]   (0101 to bits  3:0)
       ENET_TX_EN      (01 to bits    9:8)
       ENET_CRS        (01 to bits   17:16)
       ENET_RXD[0:1]   (0101 to bits 21:18)
       ENET_RX_ER      (01 to bits   29:28)
       ENET_REF_CLK    (01 to bits   31:30)
       Value to set -> 0101 xxxx xx01 0101 xxxx xx01 xxxx 0101 (0x50150105)
       Mask         -> 1111 0000 0011 1111 0000 0011 0000 1111 (0xF03F030F)
     */
    LPC_PINCON->PINSEL2 &= ~PINSEL2_BITS_MASK; /* clear needed bits */
    LPC_PINCON->PINSEL2 |= PINSEL2_BITS;
    
    /* Now follow initialization procedure (p. 181) */

    /* Remove Soft reset condition from the MAC (p. 150 for the MAC1 register
     * description) */
    LPC_EMAC->MAC1 &= ~(SOFT_RESET_BIT);

    

    return 1;
}
