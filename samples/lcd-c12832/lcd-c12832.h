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
  Time-stamp: <2014-05-29 21:04:11 (mickey)>
*/
#ifndef __LCD_C12832_H__
#define __LCD_C12832_H__

#include <rflpc17xx/rflpc17xx.h>

/**
 * Inits the LCD display.
 *
 * @param reset
 * @param a0
 * @param cs 
 * @param port
 **/
void lcd_init(rflpc_pin_t reset, rflpc_pin_t a0, rflpc_pin_t cs, rflpc_spi_t port);

/**
 * Displays the given buffer to the LCD display.
 *
 * @param buffer The buffer containing image data. The function
 *               assumes that the buffer is large enough to holds all 128x32 pixels (i.e. 512 bytes)
 * @note each byte correspond to a 8 pixels height column. The lcd is filled from the top, left to right (thus 8 first lines first).
 *       pixel of coordinates (x,y) is the ((y & 0x7)) ith bit of the buffer[(y>>3)<<7+x] byte. You must 
 *
 **/
void lcd_display_buffer(uint8_t *buffer);


#endif // __LCD_C12832_H__
