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
#include <rflpc17xx/rflpc17xx.h>
#include "protocol.h"

#define NTOHS(v) ((((v) >> 8)&0xFF) | (((v)&0xFF)<<8))


uint16_t checksum(uint8_t *buffer, unsigned int bytes_count)
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

/* Ethernet */
const uint8_t *eth_get_src(const  uint8_t *buffer)
{
    return buffer + ETH_ADDR_SIZE;
}
const uint8_t *eth_get_dst(const  uint8_t *buffer)
{
    return buffer;
}
uint16_t eth_get_type(const uint8_t *buffer)
{
    const uint8_t *ptr = (buffer + 2*ETH_ADDR_SIZE);
    return ptr[0] << 8 | ptr[1];
}
uint8_t *eth_get_payload(uint8_t *buffer)
{
    return buffer + ETH_HEADER_SIZE;
}

void eth_set_src(uint8_t *buffer, const uint8_t *src)
{
    memcpy(buffer + ETH_ADDR_SIZE, src, ETH_ADDR_SIZE);
}
void eth_set_dst(uint8_t *buffer, const uint8_t *dst)
{
    memcpy(buffer, dst, ETH_ADDR_SIZE);
}
void eth_set_type(uint8_t *buffer, uint16_t type)
{
    uint8_t *ptr = (buffer + 2*ETH_ADDR_SIZE);
    ptr[0] = type >> 8;
    ptr[1] = type & 0xff;
}

/* IP */

static uint32_t ip4_get_addr(const uint8_t *buffer, uint8_t offset)
{
    const uint8_t *ptr = buffer + offset;
    return (ptr[0] << 24) |
	(ptr[1] << 16) |
	(ptr[2] << 8) |
	(ptr[3]);
}

uint32_t ip4_get_src(const uint8_t *buffer)
{
    return ip4_get_addr(buffer, 12 + ETH_HEADER_SIZE);
}
uint32_t ip4_get_dst(const uint8_t *buffer)
{
    return ip4_get_addr(buffer, 16 + ETH_HEADER_SIZE);
}
uint8_t  ip4_get_version(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE;
    return ptr[0] >> 4;
}
uint8_t  ip4_get_header_length(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE;
    return (ptr[0] & 0xf) << 2;
}

uint16_t ip4_get_total_lenght(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 2;
    return (ptr[0] << 8) | ptr[1];
}
uint16_t ip4_get_packet_id(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 4;
    return (ptr[0] << 8) | ptr[1];
}
uint8_t  ip4_get_flags(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 6;
    return ptr[0] >> 13;
}
uint8_t  ip4_get_fragment_offset(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 6;
    return ptr[0] & 0x1FF;
}
uint8_t  ip4_get_ttl(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 8;
    return ptr[0];
}


uint8_t  ip4_get_protocol(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 9;
    return ptr[0];
}
uint16_t  ip4_get_checksum(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + 10;
    return ptr[0] << 8 | ptr[1];
}

void ip4_set_addr(uint8_t *buffer, uint32_t addr)
{
    buffer[0] = (addr >> 24) & 0xff;
    buffer[1] = (addr >> 16) & 0xff;
    buffer[2] = (addr >> 8)  & 0xff;
    buffer[3] = addr & 0xff;
}

void ip4_set_header(uint8_t *buffer, uint32_t src, uint32_t dst, uint16_t payload_length, uint8_t protocol)
{
    uint8_t *ptr = buffer + ETH_HEADER_SIZE;
    static uint16_t pid = 0;
    uint16_t csum;
    const uint16_t total_length = payload_length + 20;
    
    ptr[0] = 0x45;  /* version/HL */
    ptr[1] = 0;     /* dscp/ecn   */
    ptr[2] = total_length >> 8;
    ptr[3] = total_length & 0xff;
    ptr[4] = pid >> 8;
    ptr[5] = pid++ & 0xff;
    ptr[6] = 0;
    ptr[7] = 0;
    ptr[8] = 255;
    ptr[9] = protocol;
    ptr[10] = 0; /* chksum */
    ptr[11] = 0; /* chksum */
    ip4_set_addr(ptr + 12, src);
    ip4_set_addr(ptr + 16, dst);

    /* checksum */
    csum = checksum(ptr, 20);
    ptr[10] = csum >> 8;
    ptr[11] = csum & 0xff;
}

/* UDP */
uint16_t udp_get_src_port(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + ip4_get_header_length(buffer);
    return ptr[0] << 8 | ptr[1];
}
uint16_t udp_get_dst_port(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + ip4_get_header_length(buffer) + 2;
    return ptr[0] << 8 | ptr[1];
}

uint16_t udp_get_length(const uint8_t *buffer)
{
    const uint8_t *ptr = buffer + ETH_HEADER_SIZE + ip4_get_header_length(buffer) + 4;
    return ptr[0] << 8 | ptr[1];
}


void udp_set_header(uint8_t *buffer, uint16_t src_port, uint16_t dst_port, uint16_t payload_length)
{
    uint8_t *ptr = buffer + ETH_HEADER_SIZE + ip4_get_header_length(buffer);

    ptr[0] = src_port >> 8;
    ptr[1] = src_port & 0xff;

    ptr[2] = dst_port >> 8;
    ptr[3] = dst_port & 0xff;

    ptr[4] = (payload_length+8) >> 8;
    ptr[5] = (payload_length+8) & 0xff;
    
    ptr[6] = 0;
    ptr[7] = 0;
}

uint8_t *udp_get_payload(uint8_t *buffer)
{
    return buffer + ETH_HEADER_SIZE + ip4_get_header_length(buffer) + 8;
}
