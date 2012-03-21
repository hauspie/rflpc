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
/** @file
 * Ethernet peripheral driver
 */
#ifndef __RFLPC_ETHERNET_H__
#define __RFLPC_ETHERNET_H__

#ifdef RFLPC_ENABLE_ETHERNET

/** @addtogroup eth
 * @{
 */

/*
  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Jun. 28 2011
  Time-stamp: <2012-03-21 09:04:06 (hauspie)>
*/
#include <stdint.h>
#include "../nxp/LPC17xx.h"
#include "../interrupt.h"
#include "eth_const.h"
/** Inits the ethernet device */
extern int rflpc_eth_init();

/** Returns the link state
    @return true if link is up, false if link is down
*/
extern int rflpc_eth_link_state();


/**
 * @name Link modes
 * These constants can be used to get or set the link mode using
 * ::rflpc_eth_get_link_mode and ::rflpc_eth_set_link_mode.
 */
/** @{ */
/** This bit indicates 100Mbps/10Mbps speed */
#define RFLPC_ETH_LINK_MODE_SPEED_BIT   (1 << 0)
/** This bit indicates Full/Half duplex mode */
#define RFLPC_ETH_LINK_MODE_DUPLEX_BIT  (1 << 1)
/** 100Mbps, Half Duplex */
#define RFLPC_ETH_LINK_MODE_100HD (RFLPC_ETH_LINK_MODE_SPEED_BIT)
/** 10Mbps, Half Duplex */
#define RFLPC_ETH_LINK_MODE_10HD  (0)
/** 100Mbps, Full Duplex */
#define RFLPC_ETH_LINK_MODE_100FD (RFLPC_ETH_LINK_MODE_SPEED_BIT | RFLPC_ETH_LINK_MODE_DUPLEX_BIT)
/** 10Mbps, Full Duplex */
#define RFLPC_ETH_LINK_MODE_10FD  (RFLPC_ETH_LINK_MODE_DUPLEX_BIT)
/** @} */

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

/** returns the current link mode.

    The information is extracted from the PHY PHYSTS register if
    ::RFLPC_ETH_PHY_USE_EXTENDED_MII_REGISTERS is defined. Otherwise,
    it uses the Control register which is less reliable and may return
    wrong mode (especially for the duplex mode)

    @return bit 0 tells speed, bit 1 tells duplex
*/
extern int rflpc_eth_get_link_mode();


/** This structure holds a descriptor which describes the fragment
    received or sent by the ethernet DMA
*/
typedef struct
{
    uint8_t *packet;		/**< pointer to buffer where the ethernet frame
				 * (or frame fragment) is stored */
   /** Control word where we find data buffer size and other information relative to reception/emission
    *  of the buffer.
    * Word format for reception:
    * - Bits 10:0 : Size of the buffer pointed by packet (-1 encoded)
    * - Bit 31 : IRQ on reception enabled/disabled
    *
    * Word format for transmission:
    * - Bits 10:0 : Size of the buffer to transmit (-1 encoded)
    * - Bit 26 : Override. If true, bits 30:27 will override the default values in MAC registers. Otherwise, bits 29:27 will be ignored
    * - Bit 27 : Huge (jumbo) frame
    * - Bit 28 : Pad short frame to 64 bytes
    * - Bit 29 : Append CRC to the frame
    * - Bit 30 : Last fragment
    * - Bit 31 : IRQ on transmission enabled/disabled
    */
    uint32_t control;
} rfEthDescriptor;

/** This structure holds the reception status associated to a descriptor.
 */
typedef struct
{
   /** Receive status return flags.
    * - Bits 10:0 : Size of the received data (-1 encoded)
    * - Bit 18 : Control frame
    * - Bit 19 : VLAN frame
    * - Bit 20 : Fail frame filter. The remainder of the frame will be discarded
    * - Bit 21 : Multicast frame
    * - Bit 22 : Broadcast frame
    * - Bit 23 : CRC error
    * - Bit 24 : Symbol error reported by PHY
    * - Bit 25 : Length Error
    * - Bit 26 : Range error
    * - Bit 27 : Alignement erro
    * - Bit 28 : Receive overrun
    * - Bit 29 : No more descriptor and the frame is to big to fit in the data buffer
    * - Bit 30 : Last flag. This is the last fragment of the frame
    * - Bit 31 : Error occured.
    */
    uint32_t status_info;
    /** hash CRC calculated from source and destination address.
     * - Bits 8:0 : CRC calcultated from the source address
     * - Bits 24:16 : CRC calcultated from the destination address
     */
    uint32_t status_hash_crc;
} rfEthRxStatus;


/** This structure holds the transmit status associated to a descriptor.
 */
typedef struct
{
   /** Transmission status info.
    * - Bits 24:21 : Collision count (number of retransmission that were needed to send the packet)
    * - Bit 25 : This packet incurred deferral (not a problem if not excessive defer)
    * - Bit 26 : Excessive defer. Packet was aborted due to excessive defer
    * - Bit 27 : Excessive collision. Packet was aborted due to excessive collision
    * - Bit 28 : Late collision. Packet aborted due to Out of window Collision
    * - Bit 29 : TX Underrun due to adapter not producing transmit data
    * - Bit 30 : No descriptor. Transmit stream interrupted because not enough descriptor
    * - Bit 31 : Error occurred during transmission.
    */
   uint32_t status_info;
} rfEthTxStatus;


/**
 * @brief Returns the size of a packet from the status_info field of a ::rfEthTxStatus or ::rfEthRxStatus
 *
 * @param [in] status_info The corresponding field in ::rfEthTxStatus or ::rfEthTxStatus
 * @return The size of the corresponding buffer
 **/
static inline uint32_t rflpc_eth_get_packet_size(uint32_t status_info)
{
    return (status_info & 0x7FF) + 1;
}

/**
 * @brief Sets the transmission control word of a ::rfEthDescriptor struct.
 * @note This function is only a small helper. It will set the last frame bit. If you want more control, DIY :)
 * @param [in]  size_to_send Size of the buffer to send
 * @param [out] control Pointer to the control word to send
 * @param [in] trigger_it Should the transmission of this buffer produce an interrupt
 * @param [in] last_fragment Is this fragment the last of the frame
 **/
static inline void rflpc_eth_set_tx_control_word(uint32_t size_to_send, uint32_t *control, int trigger_it, int last_fragment)
{
    *control = ((size_to_send-1) & 0x7FF) | ((last_fragment & 1) << 30) | ((trigger_it & 1) << 31);
}

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
static inline int rflpc_eth_get_current_rx_packet_descriptor(rfEthDescriptor **descriptor, rfEthRxStatus **status)
{
    if (LPC_EMAC->RxConsumeIndex == LPC_EMAC->RxProduceIndex) /* empty queue */
	return 0;
    *descriptor = ((rfEthDescriptor*)LPC_EMAC->RxDescriptor) + LPC_EMAC->RxConsumeIndex;
    *status = ((rfEthRxStatus*)LPC_EMAC->RxStatus) + LPC_EMAC->RxConsumeIndex;
    return 1;
}

/**
 * Returns true if a packet has been received and not yet processed available
 *
 *
 * @return true if a packet is available
 */
static inline int rflpc_eth_rx_available()
{
    if (LPC_EMAC->RxConsumeIndex == LPC_EMAC->RxProduceIndex) /* empty queue */
	return 0;
    return 1;
}


/** This function has to be called when a packet (which descriptor is returned
 * by ::rflpc_eth_get_current_rx_packet_descriptor) has been processed and can
 * be discarded
 */

static inline void rflpc_eth_done_process_rx_packet()
{
    if (LPC_EMAC->RxConsumeIndex == LPC_EMAC->RxProduceIndex) /* Queue is empty */
	return;
    if (LPC_EMAC->RxConsumeIndex == LPC_EMAC->RxDescriptorNumber) /* Wrap around */
	LPC_EMAC->RxConsumeIndex = 0;
    else
	LPC_EMAC->RxConsumeIndex++;
}


/** Sets tx descriptors and status base address

    @warning descriptors and status must be aligned on a word boundary.
 */
extern void rflpc_eth_set_tx_base_addresses(rfEthDescriptor *descriptos, rfEthTxStatus *status, int count);

/** Helper macro for ::rflpc_eth_get_current_tx_packet_descriptor */
#define TX_PRODUCE_INDEX_INC(inc) ((LPC_EMAC->TxProduceIndex + (inc))% (LPC_EMAC->TxDescriptorNumber+1))

/** returns the index of the current tx packet descriptor.

   @param [out] descriptor a pointer to a pointer of ::rfEthDescriptor
   @param [out] status a pointer to a pointer of ::rfEthTxStatus
   @param [in]  idx the descriptor to get. 0 is the first free, 1 the second free...

   The return descriptor is the one that is prepared by software before
    sending it. When ::rflpc_eth_done_process_tx_packet() is called, the packet
    will be owned by the hardware and sent as soon as possible.

    @return 0 if no more descriptor are available (which means that all the
    buffers are owned by the hardware and waiting to be sent). 1 if pointers are valid
*/

static inline int rflpc_eth_get_current_tx_packet_descriptor(rfEthDescriptor **descriptor, rfEthTxStatus **status, int idx)
{
    /* queue full */
    if (TX_PRODUCE_INDEX_INC(idx) == LPC_EMAC->TxConsumeIndex - 1 ||
	((TX_PRODUCE_INDEX_INC(idx) == LPC_EMAC->TxDescriptorNumber) && LPC_EMAC->TxConsumeIndex == 0))
    {
	return 0;
    }
    *descriptor = ((rfEthDescriptor*)LPC_EMAC->TxDescriptor) + TX_PRODUCE_INDEX_INC(idx);
    *status = ((rfEthTxStatus*)LPC_EMAC->TxStatus) + TX_PRODUCE_INDEX_INC(idx);
    return 1;
}

/**
 * @brief Returns the index of the last send buffer
 *
 * @return int
 **/
static inline int rflpc_eth_get_last_sent_packet_idx()
{
    int idx = LPC_EMAC->TxConsumeIndex - 1;
    if (idx < 0)
	return LPC_EMAC->TxDescriptorNumber;
    return idx;
}

/** When the packet has been generated, calling this function will make it
 * owned by the hardware and queued for emission
 * @param [in] count The number of descriptors to hand to the hardware
 */
static inline void rflpc_eth_done_process_tx_packet(int count)
{
    /* queue full */
    if (TX_PRODUCE_INDEX_INC(count) == LPC_EMAC->TxConsumeIndex - 1 ||
	((TX_PRODUCE_INDEX_INC(count) == LPC_EMAC->TxDescriptorNumber) && LPC_EMAC->TxConsumeIndex == 0))
    {
	return;
    }
    LPC_EMAC->TxProduceIndex = TX_PRODUCE_INDEX_INC(count);
}

/** returns the device MAC address
 */
extern void rflpc_eth_get_mac_address(uint8_t *addr);

/** sets the device MAC address
 */
extern void rflpc_eth_set_mac_address(const uint8_t *addr);

/** sets the interrupt handler of the ethernet peripheral */
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

/**
 * Force the generation of the given interrupt.
 * This can be used for triggering an interrupt by software
 *
 * @param irqs a bitwise ORed combination of RFLPC_ETH_IRQ_EN_* bits
 * @return void
 **/
static inline void rflpc_eth_irq_trigger(uint32_t irqs)
{
    LPC_EMAC->IntSet = irqs;
}

/**
 * Activate the hardware receive filter.
 * When activated, the hardware will
 * discard all packet whose destination address are not for the device (MAC
 * address filter).
 *
 * @param accept_unicast If true, accept ALL unicast frames
 * @param accept_multicast If true, also accept multicast frames
 * @param accept_broadcast If true, also accept broadcast frames
 */
static inline void rflpc_eth_activate_rx_filter(int accept_unicast, int accept_multicast, int accept_broadcast)
{
    LPC_EMAC->RxFilterCtrl = 0;
    if (accept_unicast)
       LPC_EMAC->RxFilterCtrl |= RFLPC_ETH_RXFILTER_UNICAST_EN;
    if (accept_multicast)
	LPC_EMAC->RxFilterCtrl |= RFLPC_ETH_RXFILTER_MULTICAST_EN;
    if (accept_broadcast)
	LPC_EMAC->RxFilterCtrl |= RFLPC_ETH_RXFILTER_BROADCAST_EN;
    LPC_EMAC->RxFilterCtrl |= RFLPC_ETH_RXFILTER_PERFECT_EN;
    /* activate filter */
    LPC_EMAC->Command &= ~RFLPC_ETH_CMD_PASS_RX_FILTER;
}
/**
 * Deactivates the hardware receive filter.
 * All received frames will now be received by the driver.
 */
static inline void rflpc_eth_deactivate_rx_filter()
{
    LPC_EMAC->Command |= RFLPC_ETH_CMD_PASS_RX_FILTER;
}

/**
 * @brief Use the ::printf function to dump the values of the MAC registers
 *
 * @return void
 **/
extern void rflpc_eth_dump_internals();


/** @} */

#endif /* ENABLE_ETHERNET */

#endif
