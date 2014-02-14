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
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include <stdint.h>

/**
   @warning: In all function, buffer pointer must point to the start of the
   ETHERNET header
*/


#define ETH_ADDR_SIZE 6
#define ETH_HEADER_SIZE ((ETH_ADDR_SIZE)*2 + 2)

uint16_t checksum(uint8_t *buffer, unsigned int bytes_count);

/* Ethernet */
const uint8_t *eth_get_src(const  uint8_t *buffer);
const uint8_t *eth_get_dst(const  uint8_t *buffer);
uint16_t eth_get_type(const uint8_t *buffer);
uint8_t *eth_get_payload(uint8_t *buffer);
void eth_set_src(uint8_t *buffer, const uint8_t *src);
void eth_set_dst(uint8_t *buffer, const uint8_t *dst);
void eth_set_type(uint8_t *buffer, uint16_t type);

/* IP */
uint32_t ip4_get_src(const uint8_t *buffer);
uint32_t ip4_get_dst(const uint8_t *buffer);
uint8_t  ip4_get_version(const uint8_t *buffer);
uint8_t  ip4_get_header_length(const uint8_t *buffer);
uint16_t ip4_get_total_lenght(const uint8_t *buffer);
uint16_t ip4_get_packet_id(const uint8_t *buffer);
uint8_t  ip4_get_flags(const uint8_t *buffer);
uint8_t  ip4_get_fragment_offset(const uint8_t *buffer);
uint8_t  ip4_get_ttl(const uint8_t *buffer);
uint8_t  ip4_get_protocol(const uint8_t *buffer);
uint16_t  ip4_get_checksum(const uint8_t *buffer);

void ip4_set_header(uint8_t *buffer, uint32_t src, uint32_t dst, uint16_t payload_length, uint8_t protocol);

/* UDP */
uint16_t udp_get_src_port(const uint8_t *buffer);
uint16_t udp_get_dst_port(const uint8_t *buffer);
uint16_t udp_get_length(const uint8_t *buffer);
uint8_t *udp_get_payload(uint8_t *buffer);

void udp_set_header(uint8_t *buffer, uint16_t src_port, uint16_t dst_port, uint16_t payload_length);



#endif
