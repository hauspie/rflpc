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
    Time-stamp: <2013-02-22 14:47:05 (hauspie)>

 */
#include "protocols.h"


#define GET_TWO(dst, src, idx) dst = src[idx++] << 8; dst |= src[idx++]
#define GET_FOUR(dst, src, idx) dst = src[idx++] << 24; dst |= src[idx++] << 16; dst |= src[idx++] << 8; dst |= src[idx++]

#define GET_MAC(dst, src, idx)			\
    dst[0] = src[idx++];			\
    dst[1] = src[idx++];			\
    dst[2] = src[idx++];			\
    dst[3] = src[idx++];			\
    dst[4] = src[idx++];			\
    dst[5] = src[idx++]


#define PUT_TWO(dst, idx, src) dst[idx++] = (src >> 8) & 0xFF; dst[idx++] = src & 0xFF
#define PUT_FOUR(dst, idx, src) dst[idx++] = (src >> 24) & 0xFF; dst[idx++] = (src >> 16) & 0xFF;dst[idx++] = (src >> 8) & 0xFF; dst[idx++] = src & 0xFF

#define PUT_MAC(dst, idx, src)			\
    dst[idx++] = src[0];			\
    dst[idx++] = src[1];			\
    dst[idx++] = src[2];			\
    dst[idx++] = src[3];			\
    dst[idx++] = src[4];			\
    dst[idx++] = src[5]


#define NTOHS(v) ((((v) >> 8)&0xFF) | (((v)&0xFF)<<8))

uint16_t proto_checksum(uint8_t *buffer, unsigned int bytes_count)
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

void proto_eth_demangle(EthHead *eh, const uint8_t *data)
{
    int idx = 0;
    /* Dst Address */
    GET_MAC(eh->dst.addr, data, idx);
    /* Source Address */
    GET_MAC(eh->src.addr, data, idx);
    /* Type */
    GET_TWO(eh->type, data, idx);
}
void proto_eth_mangle(EthHead *eh, uint8_t *data)
{
    int idx = 0;
    /* Dst Address */
    PUT_MAC(data, idx, eh->dst.addr);
    /* Source Address */
    PUT_MAC(data, idx, eh->src.addr);
    /* Type */
    PUT_TWO(data, idx, eh->type);
}

void proto_arp_demangle(ArpHead *ah, const uint8_t *data)
{
    int idx = 0;
    /* hardware type */
    GET_TWO(ah->hard_type, data, idx);
    /* protocol type */
    GET_TWO(ah->protocol_type, data, idx);
    ah->hlen = data[idx++];
    ah->plen = data[idx++];
    /* opcode */
    GET_TWO(ah->opcode, data, idx);
    /* sender mac */
    GET_MAC(ah->sender_mac.addr, data, idx);
    /* sender IP */
    GET_FOUR(ah->sender_ip, data, idx);
    /* target mac */
    GET_MAC(ah->target_mac.addr, data, idx);
    /* target IP */
    GET_FOUR(ah->target_ip, data, idx);
}
void proto_arp_mangle(ArpHead *ah, uint8_t *data)
{
    int idx = 0;
    PUT_TWO(data, idx, ah->hard_type);
    PUT_TWO(data, idx, ah->protocol_type);
    data[idx++] = ah->hlen;
    data[idx++] = ah->plen;
    PUT_TWO(data, idx, ah->opcode);
    PUT_MAC(data, idx, ah->sender_mac.addr);
    PUT_FOUR(data, idx, ah->sender_ip);
    PUT_MAC(data, idx, ah->target_mac.addr);
    PUT_FOUR(data, idx, ah->target_ip);
}

void proto_ip_demangle(IpHead *ih, const uint8_t *data)
{
    int idx = 0;
    ih->version_length = data[idx++];
    ih->dscp_ecn = data[idx++];
    GET_TWO(ih->total_length, data, idx);
    GET_TWO(ih->identification, data, idx);
    GET_TWO(ih->flags_frag_offset,data,idx);
    ih->ttl = data[idx++];
    ih->protocol = data[idx++];
    GET_TWO(ih->header_checksum, data, idx);
    GET_FOUR(ih->src_addr, data, idx);
    GET_FOUR(ih->dst_addr, data, idx);
}
void proto_ip_mangle(IpHead *ih, uint8_t *data)
{
    int idx = 0;
    data[idx++] = ih->version_length;
    data[idx++] = ih->dscp_ecn;
    PUT_TWO(data, idx, ih->total_length);
    PUT_TWO(data, idx, ih->identification);
    PUT_TWO(data, idx,ih->flags_frag_offset);
    data[idx++] = ih->ttl;
    data[idx++] = ih->protocol;
    PUT_TWO(data, idx, ih->header_checksum);
    PUT_FOUR(data, idx, ih->src_addr);
    PUT_FOUR(data, idx, ih->dst_addr);
    /* set checksum */
    if (ih->header_checksum  == 0) /* need to compute checksum */
    {
	ih->header_checksum = proto_checksum(data, (ih->version_length & 0xF)<<2);
	idx = 10;
	PUT_TWO(data, idx, ih->header_checksum);
    }
}

void proto_icmp_demangle(IcmpHead *ih, const uint8_t *data)
{
    int idx = 0;
    ih->type = data[idx++];
    ih->code = data[idx++];
    GET_TWO(ih->checksum, data, idx);
    if (ih->type == PROTO_ICMP_ECHO_REQUEST || ih->type == PROTO_ICMP_ECHO_REPLY)
    {
	GET_TWO(ih->data.echo.identifier, data, idx);
	GET_TWO(ih->data.echo.sn, data, idx);
	return;
    }
    GET_FOUR(ih->data.raw, data, idx);
    
}
void proto_icmp_mangle(IcmpHead *ih, uint8_t *data)
{
    int idx = 0;
    data[idx++] = ih->type;
    data[idx++] = ih->code;
    PUT_TWO(data, idx, ih->checksum);
    if (ih->type == PROTO_ICMP_ECHO_REQUEST || ih->type == PROTO_ICMP_ECHO_REPLY)
    {
	PUT_TWO(data, idx, ih->data.echo.identifier);
	PUT_TWO(data, idx, ih->data.echo.sn);
	return;
    }
    PUT_FOUR(data, idx, ih->data.raw);
}


void proto_udp_demangle(UdpHead *uh, const uint8_t *data)
{
		int idx = 0;
		GET_TWO(uh->src_port, data, idx);
		GET_TWO(uh->dst_port, data, idx);
		GET_TWO(uh->len,      data, idx);
		GET_TWO(uh->checksum, data, idx);
}

void proto_udp_mangle(UdpHead *uh, uint8_t *data)
{
		int idx = 0;
		PUT_TWO(data, idx, uh->src_port);
		PUT_TWO(data, idx, uh->dst_port);
		PUT_TWO(data, idx, uh->len);
		PUT_TWO(data, idx, uh->checksum);
}
