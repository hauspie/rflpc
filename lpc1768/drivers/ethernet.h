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
#include <stdint.h>

/** Inits the ethernet device */
extern int rflpc_eth_init();

/** Returns the link state
    @return true if link is up, false if link is down
*/
extern int rflpc_eth_link_state();



/** Force the physical link to perform auto negociation of mode.

    This function will start autonegociation, wait for it to finish and
    reconfigure MAC/PHY if needed (100Mbps/10Mbps, half duplex/full duplex...)
 */
extern void rflpc_eth_link_auto_negociate();


/** Possible link modes */
typedef enum
{
    RFLPC_ETH_LINK_MODE_100FD, /**<! 100Mbps Full Duplex */
    RFLPC_ETH_LINK_MODE_100HD, /**<! 100Mbps Half Duplex */
    RFLPC_ETH_LINK_MODE_10FD,  /**<! 10Mbps  Full Duplex */
    RFLPC_ETH_LINK_MODE_10HD,  /**<! 10Mbps  Half Duplex */
} rfEthLinkMode;

/** Sets the MAC and PHY devices to operate on the given mode

    @warning According to PHY datasheet, forcing the device to use full-duplex
    without using auto-negociation the partner would not be able to detect
    full-duplex and thus will use half-duplex. So it is always better to use
    autonegociation.  
*/
extern void rflpc_eth_link_set_mode(rfEthLinkMode mode);

/** functions that print information on chip and link.
    Will be removed when driver is functional, only used as debug function
*/
extern void rflpc_eth_print_infos();


/** This structure holds a descriptor which describes the fragment
    received or sent by the ethernet DMA
*/
typedef struct
{
    uint8_t *packet;		/**< pointer to buffer where the ethernet frame
				 * (or frame fragment) is stored */
    uint32_t control;		/**< control word where we find data buffer
				 * size and other information relative to
				 * reception/emission of the buffer */
} rfEthDescriptor;

/** This structure holds the reception status associated to a descriptor.
 */
typedef struct
{
    uint32_t status_info;	/**< Receive status return flags */
    uint32_t status_hash_crc;	/**< hash CRC calculated from source and
				 * destination address */
} rfEthRxStatus;


/** This structure holds the transmit status associated to a descriptor.
 */
typedef struct
{
    uint32_t status_info;
} rfEthTxStatus;


/** sets rx descriptors and status base address */
extern void rflpc_eth_set_rx_base_addresses(rfEthDescriptor *descriptors, rfEthRxStatus *status);

#endif
