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
#include "dhcp.h"


uint32_t ntohl(uint32_t val)
{
    uint8_t *c = (uint8_t*)&val;

    return (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | (c[3]);
}

void proto_dhcp_demangle(DhcpHead *dh, const uint8_t *data)
{
    memcpy(dh, data, sizeof(DhcpHead));

    dh->ciaddr = ntohl(dh->ciaddr);
    dh->yiaddr = ntohl(dh->yiaddr);
    dh->siaddr = ntohl(dh->siaddr);
    dh->giaddr = ntohl(dh->giaddr);
}
void proto_dhcp_mangle(DhcpHead *dh, uint8_t *data)
{
    dh->ciaddr = ntohl(dh->ciaddr);
    dh->yiaddr = ntohl(dh->yiaddr);
    dh->siaddr = ntohl(dh->siaddr);
    dh->giaddr = ntohl(dh->giaddr);

    memcpy(data, dh, sizeof(DhcpHead));

    dh->ciaddr = ntohl(dh->ciaddr);
    dh->yiaddr = ntohl(dh->yiaddr);
    dh->siaddr = ntohl(dh->siaddr);
    dh->giaddr = ntohl(dh->giaddr);
}

