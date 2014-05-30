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
  Time-stamp: <2014-05-30 09:59:44 (mickey)>
*/
#include <rflpc17xx/rflpc17xx.h>
#include "protocol.h"

#define TX_BUFFER_COUNT 4
#define RX_BUFFER_COUNT 4

#define BUFFER_SIZE RFLPC_ETH_MAX_FRAME_LENGTH

#define UDP 17
#define PORT 60777

rflpc_eth_descriptor_t tx_descriptors[TX_BUFFER_COUNT]__attribute__ ((aligned (8)));
rflpc_eth_descriptor_t rx_descriptors[RX_BUFFER_COUNT]__attribute__ ((aligned (8)));

rflpc_eth_rx_status_t rx_status[RX_BUFFER_COUNT] __attribute__ ((aligned (8)));
rflpc_eth_tx_status_t tx_status[TX_BUFFER_COUNT] __attribute__ ((aligned (8)));

uint8_t rx_buffers[RX_BUFFER_COUNT*BUFFER_SIZE];
uint8_t tx_buffers[TX_BUFFER_COUNT*BUFFER_SIZE];

uint8_t _mac_addr[6];

uint32_t my_ip = 192 << 24 |  168 << 16 | 100 << 8 | 1;


void print_ip4(uint32_t ip4)
{
    printf("%d.%d.%d.%d", ip4 >> 24,
	   (ip4 >> 16) & 0xff,
	   (ip4 >> 8) & 0xff,
	   (ip4) & 0xff);
}

void process_packet(uint8_t *buffer, uint16_t buffer_size)
{
    rflpc_eth_descriptor_t *txd;
    rflpc_eth_tx_status_t *txs;
    uint8_t *ptr, *ptr2;

    if (eth_get_type(buffer) != 0x0800)
	return;
    if (ip4_get_dst(buffer) != my_ip)
	return;
    if (ip4_get_protocol(buffer) != UDP)
	return;
    if (udp_get_dst_port(buffer) != PORT)
	return;
    if (!rflpc_eth_get_current_tx_packet_descriptor(&txd, &txs, 0))
    {
	printf("Send queue full\r\n");
	return;
    }
    ptr = txd->packet;
    /* Ethernet */
    eth_set_src(ptr, _mac_addr);
    eth_set_dst(ptr, eth_get_src(buffer));
    eth_set_type(ptr, 0x0800);
    /* IP */
    ip4_set_header(ptr, my_ip, ip4_get_src(buffer), 8 + 3, UDP);
    /* UDP */
    udp_set_header(ptr, PORT, udp_get_src_port(buffer), 3);
    ptr2 = udp_get_payload(ptr);
    ptr2[0] = 'O';
    ptr2[1] = 'K';
    ptr2[2] = '\0';
    rflpc_eth_set_tx_control_word(ETH_HEADER_SIZE + 20 + 8 + 3, &txd->control, 1, 1);
    rflpc_eth_done_process_tx_packet(1);
}

RFLPC_IRQ_HANDLER eth_handler()
{
    if (rflpc_eth_irq_get_status() & RFLPC_ETH_IRQ_EN_RX_DONE) /* packet received */
    {
        rflpc_eth_descriptor_t *d;
        rflpc_eth_rx_status_t *s;
        while (rflpc_eth_get_current_rx_packet_descriptor(&d, &s))
        {
	    process_packet(d->packet, rflpc_eth_get_packet_size(s->status_info));
            rflpc_eth_done_process_rx_packet();
        }
    }
    else
    {
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



void ethernet_init()
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

/*    rflpc_eth_set_irq_handler(eth_handler);
    rflpc_eth_irq_enable_set(RFLPC_ETH_IRQ_EN_RX_DONE | RFLPC_ETH_IRQ_EN_TX_DONE | RFLPC_ETH_IRQ_EN_TX_UNDERRUN
	| RFLPC_ETH_IRQ_EN_TX_ERROR |  RFLPC_ETH_IRQ_EN_TX_FINISHED);
*/
    /* initialize rx descriptors */
    for (i = 0 ; i < RX_BUFFER_COUNT ; ++i)
    {
	rx_descriptors[i].packet = rx_buffers + (i*BUFFER_SIZE);
	rx_descriptors[i].control = (BUFFER_SIZE - 1) | (1 << 31); /* -1 encoding */
    }
    rflpc_eth_set_rx_base_addresses(rx_descriptors, rx_status, RX_BUFFER_COUNT);

    for (i = 0 ; i < TX_BUFFER_COUNT ; ++i)
    {
	tx_descriptors[i].packet = tx_buffers + (i*BUFFER_SIZE);
    }
    rflpc_eth_set_tx_base_addresses(tx_descriptors, tx_status, TX_BUFFER_COUNT);
}

RFLPC_IRQ_HANDLER uart_handler()
{
   rflpc_uart_getchar(RFLPC_UART0);
   rflpc_eth_dump_internals();
}

int main()
{
    rflpc_uart_init(RFLPC_UART0);
    printf("Hello World\r\n");
    ethernet_init();
    rflpc_uart_set_rx_callback(RFLPC_UART0, uart_handler);
    printf("%p %p\r\n", tx_status, (uint32_t)tx_status & ~(0x7));
    printf("%p %p\r\n", tx_descriptors, (uint32_t)tx_descriptors & ~(0x3));
    while (1)
    {
        rflpc_eth_descriptor_t *d;
        rflpc_eth_rx_status_t *s;
        while (rflpc_eth_get_current_rx_packet_descriptor(&d, &s))
        {
	    process_packet(d->packet, rflpc_eth_get_packet_size(s->status_info));
            rflpc_eth_done_process_rx_packet();
        }
    }
    return 0;
}
