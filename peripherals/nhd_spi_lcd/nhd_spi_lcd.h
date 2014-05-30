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
  Time-stamp: <2014-05-30 09:42:08 (mickey)>
*/
#ifndef __NHD_SPI_LCD_H__
#define __NHD_SPI_LCD_H__

#include <rflpc17xx/rflpc17xx.h>

#define NHD_MAKE_SIZE(w,h) (((h) << 8) | (w))

typedef uint16_t nhd_display_size_t;

/**
 * Inits the LCD display.
 *
 * @param display_size the size of the display. use ::NHD_MAKE_SIZE to set a correct value
 * @param reset
 * @param a0
 * @param cs 
 * @param port
 **/
void nhd_spi_lcd_init(nhd_display_size_t display_size, rflpc_pin_t reset, rflpc_pin_t a0, rflpc_pin_t cs, rflpc_spi_t port);

/**
 * Displays the given buffer to the LCD display.
 *
 * @param buffer The buffer containing image data. The function
 *               assumes that the buffer is large enough to holds all pixels (i.e. width * (height/8) bytes)
 * @note each byte correspond to a 8 pixels height column. The lcd is filled from the top, left to right (thus 8 first lines first).
 *       pixel of coordinates (x,y) is the ((y & 0x7)) ith bit of the buffer[(y>>3)*width+x] byte.
 *
 **/
void nhd_spi_lcd_display_buffer(uint8_t *buffer);


#endif /* __NHD_SPI_LCD_H__ */
