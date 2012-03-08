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
  Time-stamp: <2012-03-08 16:11:38 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

#define BUFFER_SIZE 1024
#define MAGIC 0xCA
static uint8_t buffer[BUFFER_SIZE];

void fill_magic()
{
    int i;
    for (i = 0 ; i < BUFFER_SIZE ; ++i)
    {
	buffer[i] = MAGIC;
    }
}

int test_memset(uint8_t *ptr, int c, int n)
{
    int i;
    uint8_t *test_ptr;
    fill_magic();
    memset(ptr, c, n);
    printf("memset done\r\n");
    /* Test values */
    for (i = 0 ; i < n ; ++i)
    {
	if (ptr[i] != (c & 0xFF))
	{
	    printf("Value error: Found 0x%02x at %d instead of 0x%02x\r\n", ptr[i], i, c);
	    return i;
	}
    }
    /* Test overlap */
    test_ptr = buffer;
    while (test_ptr != ptr)
    {
	if (*test_ptr != MAGIC)
	{
	    printf("Overlap error: Found 0x%02x at %d instead of 0x%02x\r\n", *test_ptr, test_ptr-ptr, MAGIC);
	    return test_ptr - ptr;
	}
	test_ptr++;
    }
    test_ptr = ptr + n;
    while (test_ptr != buffer + BUFFER_SIZE)
    {
	if (*test_ptr != MAGIC)
	{
	    printf("Overlap error: Found 0x%02x at %d instead of 0x%02x\r\n", *test_ptr, test_ptr-ptr, MAGIC);
	    return test_ptr - ptr;
	}
	test_ptr++;
    }
    return 0;
}



int main()
{
    rflpc_uart_init(RFLPC_UART0);
    printf("memset sample test build on %s %s\r\n", __DATE__, __TIME__);
    
    printf("Testing aligned memset of multiple of 4 bytes\r\n");
    test_memset(buffer + 24, 0x42, BUFFER_SIZE - 48);
    printf("Testing non-aligned memset of multiple of 4 bytes\r\n");
    test_memset(buffer + 25, 0x42, BUFFER_SIZE - 48);
    printf("Testing aligned memset of non-multiple of 4 bytes\r\n");
    test_memset(buffer + 24, 0x42, BUFFER_SIZE - 41);
    printf("Testing non-aligned memset of non-multiple of 4 bytes\r\n");
    test_memset(buffer + 25, 0x42, BUFFER_SIZE - 41);
    
    while (1);
    return 0;
}
