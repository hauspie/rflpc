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
  Time-stamp: <2013-02-25 11:33:06 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#include "simple_net.h"
#include "protocols.h"
#include "dhcp.h"

static uint8_t ip[4];
const static uint8_t _bmac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#define IP_OFFSET PROTO_MAC_HLEN
#define UDP_OFFSET (IP_OFFSET + PROTO_IP_HLEN)
#define DHCP_OFFSET (UDP_OFFSET + PROTO_UDP_HLEN)

typedef struct
{
    EthHead eth;
    IpHead ip;
    UdpHead udp;
    DhcpHead dhcp;
}FullPacket;

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



void dhcp_send(DhcpHead *dh, uint32_t src_ip, uint32_t dst_ip, const uint8_t *dst_mac)
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
}


uint8_t *dhcp_create_option_array(uint8_t *options, uint8_t code, uint8_t len, void *val)
{
    options[0] = code;
    options[1] = len;
    memcpy(options+2, val, len);
    return options + len + 2;
}

uint8_t *dhcp_create_option_simple(uint8_t *options, uint8_t code, uint8_t len, uint32_t val)
{
    return dhcp_create_option_array(options, code, len, &val);
}

uint32_t dhcp_get_option_array(uint8_t *options, uint8_t code, void *array, uint8_t max_size)
{
    uint8_t *current = options;
    for ( ;current <= options + 60 && *current != OPTION_END; current += current[1]+2 /* 2 == size of code + len */)
    {
	if (*current == code)
	{
	    uint8_t len = current[1] > max_size ? max_size : current[1];
	    memcpy(array, current+2, len);
	    return code;
	}
    }
    return OPTION_END;
}

uint32_t dhcp_get_option_simple(uint8_t *options, uint8_t code)
{
    uint32_t val = 0;
    if (dhcp_get_option_array(options, code, &val, sizeof(uint32_t)) == OPTION_END)
	return OPTION_END;
    return val;
}

void dhcp_discover(void)
{
    DhcpHead dhcp;
    uint8_t *op;

    memset(&dhcp, 0, sizeof(DhcpHead));

    dhcp.op = BOOTREQUEST;
    dhcp.htype = 1; /* ethernet */
    dhcp.hlen = 6;
    dhcp.hops = 0;
    dhcp.xid = rand();
    
    memcpy(dhcp.chaddr, simple_net_get_mac(), 6);

    /* DHCP Magic Cookie */
    dhcp.options[0] = 99;
    dhcp.options[1] = 130;
    dhcp.options[2] = 83;
    dhcp.options[3] = 99;
    op = dhcp_create_option_simple(dhcp.options+4, OPTION_DHCP_MESSAGE_TYPE, 1, DHCPDISCOVER);
    op[0] = OPTION_END;

    dhcp_send(&dhcp, 0, 0xffffffff, _bmac);
}

void dhcp_request(FullPacket *p)
{
    DhcpHead dhcp_request;
    uint8_t *op;

    memset(&dhcp_request, 0, sizeof(DhcpHead));

    dhcp_request.op = BOOTREQUEST;
    dhcp_request.htype = 1; /* ethernet */
    dhcp_request.hlen = 6;
    dhcp_request.hops = 0;
    dhcp_request.xid = p->dhcp.xid;
    dhcp_request.ciaddr = p->dhcp.yiaddr;
    dhcp_request.siaddr = ntohl(dhcp_get_option_simple(p->dhcp.options+4, OPTION_DHCP_SERVER_IDENTIFIER));
    
    memcpy(dhcp_request.chaddr, simple_net_get_mac(), 6);

    dhcp_request.options[0] = 99;
    dhcp_request.options[1] = 130;
    dhcp_request.options[2] = 83;
    dhcp_request.options[3] = 99;
    op = dhcp_create_option_simple(dhcp_request.options+4, OPTION_DHCP_MESSAGE_TYPE, 1, DHCPREQUEST);
    op = dhcp_create_option_simple(op, OPTION_DHCP_REQUESTED_IP, 4, ntohl(dhcp_request.ciaddr));
    op = dhcp_create_option_simple(op, OPTION_DHCP_SERVER_IDENTIFIER, 4, ntohl(dhcp_request.siaddr));
    op[0] = OPTION_END;
    dhcp_send(&dhcp_request, dhcp_request.ciaddr, dhcp_request.siaddr, p->eth.src.addr);
}

void dhcp_handle_ack(FullPacket *p)
{
    printf("DHCP Server: "); print_ip(ntohl(dhcp_get_option_simple(p->dhcp.options+4, OPTION_DHCP_SERVER_IDENTIFIER))); printf("\r\n");

    printf("My ip: "); print_ip(p->dhcp.yiaddr); printf("\r\n");
    printf("Subnet mask: "); print_ip(ntohl(dhcp_get_option_simple(p->dhcp.options+4, OPTION_SUBNET_MASK))); printf("\r\n");
    printf("Router: "); print_ip(ntohl(dhcp_get_option_simple(p->dhcp.options+4, OPTION_ROUTER))); printf("\r\n");
    printf("Lease time: %d s\r\n", ntohl(dhcp_get_option_simple(p->dhcp.options+4, OPTION_DHCP_LEASE_TIME)));
}

void dhcp_process_packet(FullPacket *p)
{
    DhcpHead *dhcp = &p->dhcp;
    uint8_t *current = dhcp->options + 4;
        
    for ( ;current <= dhcp->options + 64 && *current != OPTION_END; current += current[1]+2 /* 2 == size of code + len */)
    {
	if (*current != OPTION_DHCP_MESSAGE_TYPE)
	    continue;
	switch(current[2])
	{
	    case DHCPOFFER:
		printf("DHCPOFFER ip: ");
		print_ip(dhcp->yiaddr);
		printf("\r\n");
		dhcp_request(p);
		return;
	    case DHCPACK:
		printf("ACK Received\r\n");
		dhcp_handle_ack(p);
		return;
	    default:
		break;
	}
    }
}

void packet_in(uint8_t *data, uint16_t size)
{
    FullPacket p;
    

/*    print_packet(data, size);*/
    proto_eth_demangle(&p.eth, data);
    printf("Eth received\r\n");

    if (!MAC_ADDR_EQUAL(p.eth.dst.addr, _bmac) && !MAC_ADDR_EQUAL(p.eth.dst.addr, simple_net_get_mac()))
	return;
    
    if (p.eth.type != PROTO_IP)
	return;

    proto_ip_demangle(&p.ip, data + IP_OFFSET);
    printf("IP received\r\n");
    printf("Src: ");
    print_ip(p.ip.src_addr);
    printf("\r\nDst: ");
    print_ip(p.ip.dst_addr);
    printf("\r\n");

    if (p.ip.protocol != PROTO_UDP)
	return;

    proto_udp_demangle(&p.udp, data + UDP_OFFSET);
    printf("UDP received %d %d\r\n", p.udp.src_port, p.udp.dst_port);

    if (p.udp.dst_port != DHCP_CLIENT_PORT || p.udp.src_port != DHCP_SERVER_PORT)
	return;
    printf("Received DHCP packet\r\n");
    proto_dhcp_demangle(&p.dhcp, data + DHCP_OFFSET);
    dhcp_process_packet(&p);
}



int main()
{
    rflpc_uart_init(RFLPC_UART0);
    
    simple_net_ethernet_init();
    simple_net_set_rx_callback(packet_in);

    dhcp_discover();

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
