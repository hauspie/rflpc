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
  Time-stamp: <2014-05-30 09:34:26 (mickey)>
*/
#include "nhd_spi_lcd.h"


/* Data sheet available at: http://www.newhavendisplay.com/specs/NHD-C12832A1Z-FSW-FBW-3V3.pdf for spi interface
   http://www.newhavendisplay.com/app_notes/ST7565R.pdf for full controller */


static rflpc_pin_t _reset_pin;
static rflpc_pin_t _a0_pin;
static rflpc_pin_t _cs_pin;
static rflpc_spi_t _spi_port;
static nhd_display_size_t _display_size;

/** Controler commands */
#define LCD_TURN_OFF() _lcd_cmd(0xAE)
#define LCD_TURN_ON()  _lcd_cmd(0xAF)

/* Selectss a page, but do not change the column */
#define LCD_SELECT_PAGE(p) _lcd_cmd((0xB0 | ((p) & 0xf)))
/* Selects start column */
#define LCD_SELECT_COLUMN(c) do { _lcd_cmd(0x10 | (((c) >> 4) &0xf)); _lcd_cmd((c) & 0xf); } while(0)
/* Select start line */
#define LCD_SELECT_LINE(l) _lcd_cmd(0x40 | ((l) & 0x3f))

/* Starts a page. Selects the page AND switch to column 0 */
#define LCD_START_PAGE(p) do {LCD_SELECT_PAGE((p)); LCD_SELECT_COLUMN(0); LCD_SELECT_LINE(0); } while (0)

/* Select power mode */
#define LCD_POWER_MODE(booster, regulator, follower) _lcd_cmd(0x28 | (((booster) & 1) << 2) | (((regulator) & 1) << 1) | (((follower) & 1)))

#define LCD_ALL_PIXELS_ON() _lcd_cmd(0xA5)
#define LCD_ALL_PIXELS_OFF() _lcd_cmd(0xA4)

#define LCD_BRIGHTNESS(b) do {   _lcd_cmd(0x81); _lcd_cmd((b) & 0x3f); } while (0)

#define LCD_SOFT_RESET() _lcd_cmd(0xE2)

#define LCD_NOP() _lcd_cmd(0xE3)

static int _get_pre_scale_value(void)
{
   int pre_scale = 6;
   /* The LCD display can be driven using an SPI clock of maximum 20Mhz (min period 50ns, see datasheet). 
      To make things easier, we set the serial clock rate at 1, so that 1 bit is transmited for each clock edge.
      We clock the SPI at full speed to ensure calculations are good, but it should be make better to avoid
      clocking to much.

      We start at 6 because, according to LPC datasheet, clock sent
      by master must not exceed 1/12 of the spi clock
    */
   while ((rflpc_clock_get_system_clock() / (pre_scale*2)) > 20000000)
      pre_scale+=2;
   return pre_scale;
}


static void _lcd_cmd(uint8_t cmd)
{
   rflpc_gpio_clr_pin(_a0_pin);
   rflpc_gpio_clr_pin(_cs_pin);
   rflpc_spi_write(_spi_port, cmd);
   /* Wait for command to be send */
   while (! rflpc_spi_idle(_spi_port));
   rflpc_gpio_set_pin(_cs_pin);
}

static void _lcd_multiple_data(uint8_t *data, uint16_t size)
{
   rflpc_gpio_set_pin(_a0_pin);
   rflpc_gpio_clr_pin(_cs_pin);
   while (size--)
      rflpc_spi_write(_spi_port, *data++);
   while (!rflpc_spi_idle(_spi_port));
   rflpc_gpio_set_pin(_cs_pin);
}

static void _lcd_single_data(uint8_t data)
{
   rflpc_gpio_set_pin(_a0_pin);
   rflpc_gpio_clr_pin(_cs_pin);
   rflpc_spi_write(_spi_port, data);
   while (!rflpc_spi_idle(_spi_port));
   rflpc_gpio_set_pin(_cs_pin);
}


void nhd_spi_lcd_init(nhd_display_size_t display_size, rflpc_pin_t reset_pin, rflpc_pin_t a0, rflpc_pin_t cs, rflpc_spi_t port)
{
   
   _reset_pin = reset_pin;
   _a0_pin = a0;
   _spi_port = port;
   _cs_pin = cs;
   _display_size = display_size;

   /* Inits SPI port */
   /* The LCD needs a falling edge first clock and expect transmission to start at the edge, not prior */
   rflpc_spi_init(_spi_port, RFLPC_SPI_MASTER, RFLPC_CCLK, 8, _get_pre_scale_value(), 2, RFLPC_SPI_CPOL_FALLING_EDGE | RFLPC_SPI_CPHA_PHASE_FIRST_EDGE);
   
   /* The application board is a complete mess regarding the pin connections...
      They use SPI port 1 for clock and MOSI (fine) but...
      They use 
         - p6 for reset which is... MISO of SPI1 (not that bad, but strange anyway)
         - p11 for SPI CS which is the hardware controled CS of SPI port... 0 ! bingo (grrrrrr). This will force us
         to manage CS by hand... pffff
   */
      


   /* Inits GPIO pins */
   rflpc_gpio_set_pin_mode_output(_reset_pin, 1);
   rflpc_gpio_set_pin_mode_output(_a0_pin, 0);
   rflpc_gpio_set_pin_mode_output(_cs_pin, 1);
      
/* Perform a reset, reset is active on low */
   rflpc_gpio_clr_pin(_reset_pin);
   RFLPC_DELAY_MICROSECS(100); /* trigger reset, must be low for at least 1 µs, use 10 µs to be sure (ST7565R datasheet, p65) */
   rflpc_gpio_set_pin(_reset_pin);
   RFLPC_DELAY_MICROSECS(100); /* Reset time is 1 µs max. Wait for 10 to be sure */
   
   /* After hard reset (i.e. with the reset pin, what we just did), the controler is in the following state:
      1. Display OFF
      2. Normal display
      3. ADC select: Normal (ADC command D0 = “L”)
      4. Power control register: (D2, D1, D0) = (0, 0, 0)
      5. 4-line SPI interface internal register data clear
      6. LCD power supply bias rate:
         1/65 DUTY = 1/9 bias
         1/49,1/55,1/53 DUTY = 1/8 bias
         1/33 DUTY = 1/6 bias
      7. Power saving clear
      8. V0 voltage regulator internal resistors Ra and Rb separation
      9. Output conditions of SEG and COM terminals
         SEG=VSS, COM=VSS
      10. Read modify write OFF
      11. Display start line set to first line
      12. Column address set to Address 0
      13. Page address set to Page 0
      14. Common output status normal
      15. V0 voltage regulator internal resistor ratio set mode clear
      16. Electronic volume register set mode clear Electronic volume register :
          (D5, D4, D3, D2, D1, D0) = (1, 0. 0, 0, 0,0)
      17. Test mode clear 
   */
   LCD_TURN_OFF();
   _lcd_cmd(0x22); /* Internal voltage regulator resistance ratio set to have 4V V0 */
   _lcd_cmd(0xc8); /* reverse column fill direction. With this setting, fill is from left to right */
   LCD_POWER_MODE(1,1,1); /* booster on, regulator on, follower on */
   LCD_TURN_ON();
   LCD_ALL_PIXELS_OFF();
   LCD_START_PAGE(0);
}

#define DISPLAY_SIZE_HEIGHT(d) (((d) >> 8) & 0xff)
#define DISPLAY_SIZE_WIDTH(d) ((d) & 0xff)
#define DISPLAY_PAGE_COUNT(d) (DISPLAY_SIZE_HEIGHT((d)) >> 3)
void nhd_spi_lcd_clear(void)
{
   int p,i;
   
   for (p = 0 ; p < DISPLAY_PAGE_COUNT(_display_size) ; ++p)
   {
      LCD_START_PAGE(p);
      for (i = 0 ; i < DISPLAY_SIZE_WIDTH(_display_size) ; ++i)
         _lcd_single_data(0);
   }
}

void nhd_spi_lcd_display_buffer(uint8_t *buffer)
{
   int p;
   for (p = 0 ; p < DISPLAY_PAGE_COUNT(_display_size) ; ++p)
   {
      LCD_START_PAGE(p);
      _lcd_multiple_data(buffer + p*DISPLAY_SIZE_WIDTH(_display_size), DISPLAY_SIZE_WIDTH(_display_size));
   }
}
