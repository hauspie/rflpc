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
  Time-stamp: <2011-09-23 11:14:13 (hauspie)>
*/
#include <rflpc17xx/rflpc17xx.h>

/*
 * This sample allows to test the SPI driver.
 * To make it work, you have to connect the SPI0 to the SPI1.
 * SPI0 is used as master to send data to SPI1 (used as slave)
 * For the MBED platform, you have to connect:
 * DIP5 -> DIP11
 * DIP6 -> DIP12
 * DIP7 -> DIP13
 * DIP8 -> DIP14
 */

void configure_timer(void)
{
   rflpc_timer_enable(RFLPC_TIMER0);
   rflpc_timer_set_clock(RFLPC_TIMER0,RFLPC_CCLK_8);
   rflpc_timer_set_pre_scale_register(RFLPC_TIMER0, rflpc_clock_get_system_clock()/8000000); /* microsecond timer */
   rflpc_timer_start(RFLPC_TIMER0);
}

void wait(int micros)
{
   uint32_t start = rflpc_timer_get_counter(RFLPC_TIMER0);
   while ((rflpc_timer_get_counter(RFLPC_TIMER0) - start) < micros);
   return;
}

#define MASTER_SPI RFLPC_SPI0
#define SLAVE_SPI RFLPC_SPI1

int main()
{
    uint8_t i = 42;
    rflpc_uart0_init();
    configure_timer();
    printf("SPI simple sample build on %s %s\r\n", __DATE__, __TIME__);
    /* Init master SPI, set the SPI clock to 12 Mhz / (60*2) = 100 kHz */
    rflpc_spi_init(MASTER_SPI, RFLPC_SPI_MASTER, RFLPC_CCLK_8, 8, 60, 2);
    /* Init slave SPI */
    rflpc_spi_init(SLAVE_SPI, RFLPC_SPI_SLAVE, RFLPC_CCLK_8, 8, 0, 0);
    
    while (1)
    {
	uint8_t val;
	printf("Master -> Slave\r\n");
	printf("[M] Sending %d\r\n", i);
	rflpc_spi_write(MASTER_SPI, i++);	
	printf("[M] Value sent\r\n");
	
	/* Try to read the byte on the other SPI */
	val = rflpc_spi_read(SLAVE_SPI);
	printf("[S] Value read: %d\r\n", val);	
		
	wait(1000000);
    }
    return 0;
}
