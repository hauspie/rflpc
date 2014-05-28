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
  Time-stamp: <2014-05-29 01:25:54 (mickey)>
*/
#include <rflpc17xx/rflpc17xx.h>


/* Data sheet available at: http://www.newhavendisplay.com/specs/NHD-C12832A1Z-FSW-FBW-3V3.pdf for 'easy' interface
   http://www.newhavendisplay.com/app_notes/ST7565R.pdf for full controller */


static rflpc_pin_t _reset_pin;
static rflpc_pin_t _a0_pin;
static rflpc_spi_t _spi_port;

static int _get_pre_scale_value(void)
{
   int pre_scale = 12;
   /* The LCD display can be driven using an SPI clock of maximum 20Mhz (min period 50ns, see datasheet). 
      To make things easier, we set the serial clock rate at 1, so that 1 bit is transmited for each clock edge.
      We clock the SPI at full speed to ensure calculations are good, but it should be make better to avoid
      clocking to much.

      We start at 12 because, according to LPC datasheet, clock sent
      by master must not exceed 1/12 of the spi clock
    */
   while ((rflpc_clock_get_system_clock() / pre_scale) > 20000000)
      pre_scale+=2;
   printf("pre_scale: %d\r\n", pre_scale);
   return pre_scale;
}


static void _lcd_cmd(uint8_t cmd)
{
   rflpc_gpio_clr_pin(_a0_pin);
   rflpc_spi_write(_spi_port, cmd);
   /* Wait for command to be send */
   while (! rflpc_spi_rx_fifo_empty(_spi_port));
}

void lcd_init(rflpc_pin_t reset_pin, rflpc_pin_t a0, rflpc_spi_t port)
{
   
   _reset_pin = reset_pin;
   _a0_pin = a0;
   _spi_port = port;

   /* Inits SPI port */
   /* The LCD needs a falling edge clock and expect transmission to start at the edge, not prior */
   rflpc_spi_init(_spi_port, RFLPC_SPI_MASTER, RFLPC_CCLK, 8, _get_pre_scale_value(), 1, RFLPC_SPI_CPOL_FALLING_EDGE | RFLPC_SPI_CPHA_PHASE_FIRST_EDGE);
   
   /* Inits GPIO pins */
   rflpc_gpio_set_pin_mode_output(_reset_pin, 0);
   rflpc_gpio_set_pin_mode_output(_a0_pin,0 );
   
   rflpc_gpio_clr_pin(_a0_pin);
   
/* Perform a reset, reset is active on low */
   rflpc_gpio_clr_pin(_reset_pin);
   RFLPC_DELAY_MICROSECS(50); /* trigger reset */
   rflpc_gpio_set_pin(_reset_pin);
   RFLPC_DELAY_MICROSECS(5000); /* 5 ms should be enough for reset to be effective */
   
   _lcd_cmd(0xAE); /* Turn LCD off */
   _lcd_cmd(0xA2); /* Voltage bias ratio to 1/9 */
   _lcd_cmd(0xA0); /* Set display RAM address SEG output to normal */
   _lcd_cmd(0xC8); /* Common output mode: normal */
   _lcd_cmd(0x22); /* Resistace ratio to set 3.5V (p34, table 11 of the ST7565R datasheet) */
   _lcd_cmd(0x2F); /* Internal power supply operating mode (p31, table 8 of the ST7565R datasheet)*/
   _lcd_cmd(0x40); /* Set display RAM start line address to 0 */
   _lcd_cmd(0xAF); /* LCD On */
   _lcd_cmd(0x81); /* V0 output voltage select */
   _lcd_cmd(0x17); /* MSB of display RAM column 111 */
   _lcd_cmd(0xA4); /* Display all point off */
}
