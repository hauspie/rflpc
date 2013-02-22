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
  Time-stamp: <2013-02-22 15:33:57 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#include "simple_net.h"
#include "protocols.h"
#include "dhcp.h"

static uint8_t ip[4];

#define IP_OFFSET PROTO_MAC_HLEN
#define UDP_OFFSET (IP_OFFSET + PROTO_IP_HLEN)
#define DHCP_OFFSET (UDP_OFFSET + PROTO_UDP_HLEN)

void print_packet(uint8_t *data, uint16_t size)
{
    int i;
    for (i = 0 ; i  < size ; ++i)
    {
	if (i % 16 == 0)
	{
	    printf("\r\n %04x ", i);
	}
	printf("%02x ", data[i]);
    }
    printf("\r\n");
}

void print_ip(uint32_t ip_)
{
    uint8_t *ip = (uint8_t*)&ip_;
    printf("%d.%d.%d.%d", ip[3], ip[2], ip[1], ip[0]);
}

void packet_in(uint8_t *data, uint16_t size)
{
    EthHead eth;
    IpHead ip;
    UdpHead udp;
    DhcpHead dhcp;
    

/*    print_packet(data, size);*/
    proto_eth_demangle(&eth, data);
    printf("Eth received\r\n");
    
    if (eth.type != PROTO_IP)
	return;

    proto_ip_demangle(&ip, data + IP_OFFSET);
    printf("IP received\r\n");
    printf("Src: ");
    print_ip(ip.src_addr);
    printf("\r\nDst: ");
    print_ip(ip.dst_addr);
    printf("\r\n");

    if (ip.protocol != PROTO_UDP)
	return;

    proto_udp_demangle(&udp, data + UDP_OFFSET);
    printf("UDP received %d %d\r\n", udp.src_port, udp.dst_port);

    if (udp.dst_port != DHCP_CLIENT_PORT || udp.src_port != DHCP_SERVER_PORT)
	return;
    printf("Received DHCP packet\r\n");
    proto_dhcp_demangle(&dhcp, data + DHCP_OFFSET);
}

void dhcp_send(DhcpHead *dh, uint32_t src_ip, uint32_t dst_ip, uint8_t *dst_mac)
{
    uint8_t *tx;
    EthHead eth;
    IpHead ip;
    UdpHead udp;

    /* Ethernet */
    memcpy(eth.dst.addr, dst_mac, 6);
    eth.type = PROTO_IP;
    
    /* IP */
    ip.version_length = 0x45;
    ip.dscp_ecn = 0;
    ip.total_length = PROTO_IP_HLEN + PROTO_UDP_HLEN + DHCP_HLEN;
    ip.identification = 0;
    ip.flags_frag_offset = 0x4000;
    ip.ttl = 64;
    ip.protocol = PROTO_UDP;
    ip.header_checksum = 0;
    ip.src_addr = src_ip;
    ip.dst_addr = dst_ip;

    /* UDP */
    udp.src_port = DHCP_CLIENT_PORT;
    udp.dst_port = DHCP_SERVER_PORT;
    udp.len = DHCP_HLEN + PROTO_UDP_HLEN;
    udp.checksum = 0;

    /* Packet generation */
    tx = simple_net_get_tx_buffer();
    proto_eth_mangle(&eth, tx);
    proto_ip_mangle(&ip, tx + IP_OFFSET);
    proto_udp_mangle(&udp, tx + UDP_OFFSET);
    proto_dhcp_mangle(dh, tx + DHCP_OFFSET);

    print_packet(tx, DHCP_OFFSET + DHCP_HLEN);

    simple_net_emit_buffer(DHCP_OFFSET + DHCP_HLEN,1);
    printf("Paf\r\n");
}

void dhcp_request(void)
{
    DhcpHead dhcp;
    uint8_t bmac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    
    dhcp.op = BOOTREQUEST;
    

    dhcp_send(&dhcp, 0, 0xffffffff, bmac);
}

int main()
{
    rflpc_uart_init(RFLPC_UART0);
    
    simple_net_ethernet_init();
    simple_net_set_rx_callback(packet_in);

    dhcp_request();

    while (1)
    {
/*	int i;
	uint8_t *tx;
	tx = simple_net_get_tx_buffer();
	for (i = 0 ; i < 6 ; ++i)
	    tx[i] = 0xff;
	for (i = 6 ; i < 30 ; ++i)
	    tx[i] = rand() & 0xff;
	    simple_net_emit_buffer(30, 1);*/
    }
    return 0;
}
