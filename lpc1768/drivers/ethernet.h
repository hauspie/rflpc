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
#include "../LPC17xx.h"
#include "../interrupt.h"

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


/** Sets rx descriptors and status base address 

    @warning descriptors must be aligned on a word boundary. status must be
    aligned on a double word boundaryx 
*/
extern void rflpc_eth_set_rx_base_addresses(rfEthDescriptor *descriptors, rfEthRxStatus *status, int count);

/** Returns the pointers on the current rx packet descriptor.

    The returned descriptor is the one of the last received packet that has not
    been marked as processed by ::rflpc_eth_done_process_rx_packet();
    @return 0 if receive queue is empty, 1 if pointers are valid
 */
extern int rflpc_eth_get_current_rx_packet_descriptor(rfEthDescriptor **descriptor, rfEthRxStatus **status);

/** This function has to be called when a packet (which descriptor is returned
 * by ::rflpc_eth_get_current_rx_packet_descriptor) has been processed and can
 * be discarded
 */

extern void rflpc_eth_done_process_rx_packet();


/** Sets tx descriptors and status base address

    @warning descriptors and status must be aligned on a word boundary. 
 */
extern void rflpc_eth_set_tx_base_addresses(rfEthDescriptor *descriptos, rfEthTxStatus *status, int count);

/** returns the index of the current tx packet descriptor.

    The return descriptor is the one that is prepared by software before
    sending it. When ::rflpc_eth_done_process_tx_packet() is called, the packet 
    will be owned by the hardware and sent as soon as possible.

    @return 0 if no more descriptor are available (which means that all the
    buffers are owned by the hardware and waiting to be sent). 1 if pointers are valid
*/
extern int rflpc_eth_get_current_tx_packet_descriptor(rfEthDescriptor **descriptor, rfEthTxStatus **status);

/** When the packet has been generated, calling this function will make it
 * owned by the hardware and queued for emission
 */
extern void rflpc_eth_done_process_tx_packet();

/** returns the device MAC address
 */
extern void rflpc_eth_get_mac_address(uint8_t *addr);

/** sets the MAC address
 */
extern void rflpc_eth_set_mac_address(uint8_t *addr);

/** sets the interrupt handler */
static inline void rflpc_eth_set_irq_handler(rflpc_irq_handler_t c)
{
    rflpc_irq_disable(ENET_IRQn);
    rflpc_irq_set_handler(ENET_IRQn, c);
    rflpc_irq_enable(ENET_IRQn);
}

/** enable eth interrupts 

    This function will ADD new irq enable, not set all enable irqs to the ones given.

    @param irqs a bitwise ORed combination of RFLPC_ETH_IRQ_EN_* bits
 */
static inline void rflpc_eth_irq_enable(uint32_t irqs)
{
    LPC_EMAC->IntEnable |= irqs;
}

/** disable eth interrupts 
    This function will remove irq enable bits.
    
    @param irqs a bitwise ORed combination of RFLPC_ETH_IRQ_EN_* bits
*/
static inline void rflpc_eth_irq_disable(uint32_t irqs)
{
    LPC_EMAC->IntEnable &= ~irqs;
}

/** sets the irq enable register
    This function will disable all irqs and then only enable those given.

    @param irqs a bitwise ORed combination of RFLPC_ETH_IRQ_EN_* bits
*/
static inline void rflpc_eth_irq_enable_set(uint32_t irqs)
{
    LPC_EMAC->IntEnable = irqs;
}

/** clear given pending interrupts */
static inline void rflpc_eth_irq_clear(uint32_t irqs)
{
    LPC_EMAC->IntClear = irqs;
}

/** gets the interrupt status.
    @return a bitwise ORed of eth pending interrupt bits
 */
static inline uint32_t rflpc_eth_irq_get_status()
{
    return LPC_EMAC->IntStatus;
}

#endif
