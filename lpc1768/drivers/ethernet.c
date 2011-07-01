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
  Time-stamp: <2011-06-30 23:17:25 (mickey)>
*/

#include "ethernet.h"
#include "../LPC17xx.h"
#include "../clock.h"

#define PCENET_BIT (1 << 30)
#define PINSEL2_BITS_MASK (0xF03F030F)
#define PINSEL2_BITS (0x50150105)



/* MAC Configuration Register 1, bits definition */
#define MAC1_RECEIVE_ENABLE  (1 << 0)      
#define MAC1_PASS_ALL_FRAMES (1 << 1)  
#define MAC1_RX_FLOW_CONTROL (1 << 2)  
#define MAC1_TX_FLOW_CONTROL (1 << 3)  
#define MAC1_LOOPBACK        (1 << 4)  
#define MAC1_RESET_TX        (1 << 8)  
#define MAC1_RESET_MCS_TX    (1 << 9)
#define MAC1_RESET_RX        (1 << 10)
#define MAC1_RESET_MCS_RX    (1 << 11)
#define MAC1_SIM_RESET       (1 << 14)
#define MAC1_SOFT_RESET      (1 << 15)

/* MAC Configuration Register 2, bits definition */
#define MAC2_FULL_DUPLEX            (1 << 0)
#define MAC2_FRAME_LENGTH_CHK       (1 << 1)   
#define MAC2_HUGE_FRAME_ENABLE      (1 << 2)    
#define MAC2_DELAYED_CRC            (1 << 3)
#define MAC2_CRC_ENABLE             (1 << 4)         
#define MAC2_PAD_ENABLE             (1 << 5)         
#define MAC2_VLAN_PAD_ENABLE        (1 << 6)    
#define MAC2_AUTO_DETECT_PAD_ENABLE (1 << 7)   
#define MAC2_PURE_PREAMBLE_ENFORCE  (1 << 8)     
#define MAC2_LONG_PREAMBLE_ENFORCE  (1 << 9)     
#define MAC2_NO_BACKOFF             (1 << 12)
#define MAC2_BACK_PRESSURE          (1 << 13)
#define MAC2_EXCESS_DEFER           (1 << 14)     


/* MAC Control register bits */
#define CTRL_RX_ENABLE        (1 << 0)
#define CTRL_TX_ENABLE        (1 << 1)
#define CTRL_REG_RESET        (1 << 3)
#define CTRL_TX_RESET         (1 << 4)
#define CTRL_RX_RESET         (1 << 5)
#define CTRL_PASS_RUNT_FRAMES (1 << 6)
#define CTRL_PASS_RX_FILTER   (1 << 7)
#define CTRL_TX_FLOW_CONTROL  (1 << 8)
#define CTRL_RMII             (1 << 9)
#define CTRL_FULL_DUPLEX      (1 << 10)

/* MII control register bits */
#define MCFG_SCAN_INCREMENT    (1 << 0)
#define MCFG_SUPPRESS_PREAMBLE (1 << 1)
#define MCFG_RESET_MIIM        (1 << 15)

#define ETH_MAX_FRAME_LENGTH 1538 /*   1500 bytes for payload 
				   * +    8 bytes for preamble/sfd
				   * +   12 bytes for src and dst address 
				   * +    2 bytes for length 
				   * +    4 bytes for CRC 
				   * +   12 bytes for minimum interframe gap */
#define ETH_MAX_CLOCK 2500000 /* Maximum allowed clock frequency for MII, defined by IEEE 802.3, see p. 154 */

static const uint8_t clock_dividers[] = {4, 6, 8, 10, 14, 20, 28, 36, 40, 44};

int rflpc_eth_init()
{
    int i;
    uint32_t divider;
    
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
    

    /* Reset all components (needed if function is called after a previous
     * initialization, no need to keep other bits value, put them to 0 so that 
     the register is in the same state as at boot time (p. 150) */
    LPC_EMAC->MAC1 = MAC1_SOFT_RESET;

    /* Reset datapaths (p. 158) (includes tx and rx DMA manager, tx retry and
     * flow control module, rw filter module (p. 143)*/
    LPC_EMAC->Command = CTRL_REG_RESET;

    /* wait a bit for ethernet to finish reset
       number of iteration of the loop is empirical
    */
    for (i = 100 ; i != 0 ; --i);

    /* Initialize MAC control register to enable padding of small frames
     * (i.e. frame < 64 bytes) and the appending of CRC at the end of the frame
     */
    LPC_EMAC->MAC2 = MAC2_CRC_ENABLE | MAC2_PAD_ENABLE;
    LPC_EMAC->MAXF = ETH_MAX_FRAME_LENGTH;

    /* Set MIIM clock multiplier to a value compatible with system clock
       (p. 154) 
       IEEE 802.3 defines the MII clock to be no faster than 2.5MHz
    */
    divider = rflpc_clock_get_system_clock() / ETH_MAX_CLOCK;
    for (i = 0 ; i < sizeof(clock_dividers); ++i)
    {
	if (clock_dividers[i] > divider)
	    break;
    }
    ++i; /* after the incrementation, i hold the value to store in the MII
	  * configuration register (p. 154) */
    /* set the clock and reset MII to use new parameters */
    LPC_EMAC->MCFG = (i & 0xF) << 2 | MCFG_RESET_MIIM;

    /* To be continued... */

    /* Now follow initialization procedure (p. 181) */
    /* Remove Soft reset condition from the MAC (p. 150 for the MAC1 register
     * description) */
    LPC_EMAC->MAC1 &= ~(MAC1_SOFT_RESET);

    

    return 1;
}
