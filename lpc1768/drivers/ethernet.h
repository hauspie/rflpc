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
  Time-stamp: <2011-07-06 22:57:49 (mickey)>
*/ 
#include <stdint.h>

#include "eth_const.h"

/** Inits the ethernet device */
extern int rflpc_eth_init();

/** Returns the link state
    @return true if link is up, false if link is down
*/
extern int rflpc_eth_link_state();


/** Possible link modes 
    Bit 0 is 1 if 100Mbps, 0 if 10Mbps
    Bit 1 is 1 if Full Duplex, 0 if half
 */
#define RFLPC_ETH_LINK_MODE_SPEED_BIT   (1 << 0)
#define RFLPC_ETH_LINK_MODE_DUPLEX_BIT  (1 << 1)

#define RFLPC_ETH_LINK_MODE_100HD (RFLPC_ETH_LINK_MODE_SPEED_BIT)
#define RFLPC_ETH_LINK_MODE_10HD  (0)
#define RFLPC_ETH_LINK_MODE_100FD (RFLPC_ETH_LINK_MODE_SPEED_BIT | RFLPC_ETH_LINK_MODE_DUPLEX_BIT)
#define RFLPC_ETH_LINK_MODE_10FD  (RFLPC_ETH_LINK_MODE_DUPLEX_BIT)

/** Forces the MAC and PHY devices to operate on the given mode no matter the
 * capability of the linked partner

    @warning According to PHY datasheet, when forcing the device to use
    full-duplex without using auto-negociation the partner would not be able to
    detect full-duplex and thus will use half-duplex. So it is always better to
    use autonegociation. Verify which parameter is effectively set with
    ::rflpc_eth_get_link_mode
 */
extern void rflpc_eth_set_link_mode(int mode);

/** Force the physical link to perform auto negociation of mode.

    This function will start autonegociation, wait for it to finish and
    reconfigure MAC/PHY if needed (100Mbps/10Mbps, half duplex/full duplex...)
    @param max_desired_mode bit 0 tells speed, bit 1 tells duplex 

    @return 0 if auto-negociation performed successfully. -1 otherwise. If the
    link is down when calling this function, -1 will be returned immediatly.

    @warning This function is blocking and wait for the autonegociation to be complete. 
 */
extern int rflpc_eth_link_auto_negociate(int max_desired_mode);

/** returns the current link mode. The information is extracted from the PHY
    PHYSTS register.

    @return bit 0 tells speed, bit 1 tells duplex
*/
extern int rflpc_eth_get_link_mode();


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
