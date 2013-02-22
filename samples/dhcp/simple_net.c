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
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2013-02-22 11:06:27 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define RX_BUFFER_SIZE  RFLPC_ETH_MAX_FRAME_LENGTH
#define RX_BUFFER_COUNT 8

#define TX_BUFFER_SIZE  RFLPC_ETH_MAX_FRAME_LENGTH
#define TX_BUFFER_COUNT 8

rflpc_eth_descriptor_t _rx_desc[RX_BUFFER_COUNT];
rflpc_eth_rx_status_t _rx_status[RX_BUFFER_COUNT];

rflpc_eth_descriptor_t _tx_desc[TX_BUFFER_COUNT];
rflpc_eth_tx_status_t _tx_status[TX_BUFFER_COUNT];

uint8_t rxbuffers[RX_BUFFER_SIZE][RX_BUFFER_COUNT];
uint8_t txbuffers[TX_BUFFER_SIZE][TX_BUFFER_COUNT];

static void (*_rx_callback)(uint8_t *data, uint16_t size) = NULL;

static uint8_t _mac_addr[6];

RFLPC_IRQ_HANDLER eth_handler()
{
    if (rflpc_eth_irq_get_status() & RFLPC_ETH_IRQ_EN_RX_DONE) /* packet received */
    {
	rflpc_eth_descriptor_t *d;
	rflpc_eth_rx_status_t *s;
	while (rflpc_eth_get_current_rx_packet_descriptor(&d, &s))
	{
	    /* done with it */
	    if (_rx_callback)
		_rx_callback(d->packet, rflpc_eth_get_packet_size(s->status_info));
	    rflpc_eth_done_process_rx_packet();
	}
    }
    rflpc_eth_irq_clear(rflpc_eth_irq_get_status());
}

void mbed_auto_set_mac(uint8_t *addr)
{
    unsigned long serial[4];

    if (rflpc_iap_get_serial_number(serial) == 0)
    {
	addr[0] = 2;
	addr[1] = 3;
	addr[2] = serial[0] & 0xFF;
	addr[3] = serial[1] & 0xFF;
	addr[4] = serial[2] & 0xFF;
	addr[5] = serial[3] & 0xFF;
    }
}



void simple_net_ethernet_init()
{
    int i;
    mbed_auto_set_mac(_mac_addr);
    rflpc_eth_init();
    rflpc_eth_set_mac_address(_mac_addr);
    rflpc_eth_get_mac_address(_mac_addr);
    printf("My mac: %x:%x:%x:%x:%x:%x\r\n", _mac_addr[0], _mac_addr[1], _mac_addr[2], _mac_addr[3], _mac_addr[4], _mac_addr[5]);
    rflpc_led_val(0);

    printf("Waiting for link to be up\r\n");
    while(!rflpc_eth_link_state());
    printf("Ok\r\n");

    rflpc_eth_set_irq_handler(eth_handler);
    rflpc_eth_irq_enable_set(RFLPC_ETH_IRQ_EN_RX_DONE);

    /* initialize rx descriptors */
    for (i = 0 ; i < RX_BUFFER_COUNT ; ++i)
    {
	_rx_desc[i].packet = rxbuffers[i];
	_rx_desc[i].control = (RX_BUFFER_SIZE - 1) | (1 << 31); /* -1 encoding */
    }
    rflpc_eth_set_rx_base_addresses(_rx_desc, _rx_status, RX_BUFFER_COUNT);

    for (i = 0 ; i < TX_BUFFER_COUNT ; ++i)
    {
	_tx_desc[i].packet = txbuffers[i];
    }
    rflpc_eth_set_tx_base_addresses(_tx_desc, _tx_status, TX_BUFFER_COUNT);
}


uint8_t *simple_net_get_tx_buffer()
{
    rflpc_eth_descriptor_t *txd;
    rflpc_eth_tx_status_t *txs;
    
    while (!rflpc_eth_get_current_tx_packet_descriptor(&txd, &txs,0));

    return txd->packet;
}

void simple_net_emit_buffer(uint16_t size, int auto_mac)
{
    rflpc_eth_descriptor_t *txd = NULL;
    rflpc_eth_tx_status_t *txs = NULL;
    
    rflpc_eth_get_current_tx_packet_descriptor(&txd, &txs,0);
    if (auto_mac)
	memcpy(txd->packet+6, _mac_addr, 6);
    rflpc_eth_set_tx_control_word(size, &txd->control, 0, 1);
    rflpc_eth_done_process_tx_packet(1);
}


void simple_net_set_rx_callback(void (*rx_callback)(uint8_t *data, uint16_t size))
{
    _rx_callback = rx_callback;
}


#define NTOHS(v) ((((v) >> 8)&0xFF) | (((v)&0xFF)<<8))


uint16_t simple_net_checksum(uint8_t *buffer, unsigned int bytes_count)
{
    uint32_t csum = 0;
    while (bytes_count >= 2)
    {
	csum += NTOHS(*((uint16_t*)buffer));
	buffer += 2;
	bytes_count -= 2;
    }
    if (bytes_count)
	csum += (*buffer << 8);
    /* add carry */
    while (csum & 0xFFFF0000)
	csum = (csum & 0xFFFF) + ((csum >> 16) & 0xFFFF);
    return (~csum) & 0xFFFF;
}
