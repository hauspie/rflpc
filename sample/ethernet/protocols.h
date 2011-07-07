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
  
    Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
    Created: 
    Time-stamp: <2011-07-07 23:23:04 (mickey)>

    Basic protocol handling.  Not meant to be trully used, but just useful
    tools for basic protocol handling for the ethernet sample.

    Works ONLY with Ethernet and IPv4
 */
#ifndef __PROTOCOLS_H__
#define __PROTOCOLS_H__
#include <stdint.h>

typedef struct
{
    uint8_t addr[6];
} EthAddr;


#define ETH_HEADER_SIZE 14

typedef struct
{
    EthAddr src;
    EthAddr dst;
    uint16_t type;
} EthHead;


#define ARP_HEADER_SIZE 28

typedef struct
{
    uint16_t hard_type;
    uint16_t protocol_type;
    uint8_t hlen;
    uint8_t plen;
    uint16_t opcode;
    EthAddr sender_mac;
    uint32_t sender_ip;
    EthAddr target_mac;
    uint32_t target_ip;
} ArpHead;

typedef struct
{
    uint8_t version_length;
    uint8_t dscp_ecn;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_frag_offset;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t header_checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
} IpHead;

typedef struct
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
}IcmpHead;

void proto_eth_demangle(EthHead *eh, const uint8_t *data);
void proto_eth_mangle(EthHead *eh, uint8_t *data);

void proto_arp_demangle(ArpHead *ah, const uint8_t *data);
void proto_arp_mangle(ArpHead *ah, uint8_t *data);

void proto_ip_demangle(IpHead *ih, const uint8_t *data);
void proto_ip_mangle(IpHead *ih, uint8_t *data);

void proto_icmp_demangle(IcmpHead *ih, const uint8_t *data);
void proto_icmp_mangle(IcmpHead *ih, uint8_t *data);


#endif
