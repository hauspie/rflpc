/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
  Created: <2013-12-16 00:00:00 (bacara)>
*/

#include <rflpc17xx/drivers/gpio.h> /* GPIO support for pins : RST, A0, CS */
#include <rflpc17xx/drivers/spi.h>  /* SPI support for data transfers */
#include <rflpc17xx/tools.h>        /*  */
#include <rflpc17xx/debug.h>        /*  */

#include "c12832.h"
#include "fonts.h"

/*
 * __DELAY(delay):
 * Wait macro used to lose a certain amount of time, depending on your platform.
 * Each `delay` iteration uses 3 clock cycle. For example, on LPC1768 @ 96Mhz,
 * __DELAY (1600) will wait 50 microseconds.
 */
#define __DELAY(d)      \
  { unsigned delay_i; for (delay_i = 0; delay_i < d; ++delay_i)	asm(""); }

/*
 * Platform-dependent configuration
 * This MUST be customized, depending on your controller. 
 * Default config: LPC1768 - mbed application board
 */
#define SPI_PORT	RFLPC_SPI1
#define LCD_RST_PIN     RFLPC_PIN_P0_8  /* MBED_DIP6  */
#define LCD_A0_PIN	RFLPC_PIN_P0_6  /* MBED_DIP8  */
#define LCD_CS_PIN      RFLPC_PIN_P0_18 /* MBED_DIP11 */

/* Size of library's internal buffer: (128x32) / 8 */
#define __BUFFER_SIZE   512

/* Library internal control vars */
static unsigned char __buffer[__BUFFER_SIZE];     /* Internal buffer */
static unsigned int __buffer_cursor = 0;          /* Buffer's cursor index */
static lcd_mode_t __mode = OUTPUT;                /* Library mode */
/* static unsigned char *__font = (unsigned char **) default_font_8x8; */

/* Mode check macro: to be used into specific mode's functions */
#define __ONLY_FOR_MODE(m)             if (__mode != m) return;
#define __ONLY_FOR_MODE_RET(m,retval)  if (__mode != m) return retval;

/* Internal functions declarations */
static void __lcd_write_cmd(unsigned char cmd);
static void __lcd_write_data(unsigned char data);
static void __reset_buffer();

void lcd_init()
{
  /* GPIO initialization for RST, A0 and CS */
  rflpc_gpio_set_pin_mode_output(LCD_RST_PIN, 0);
  rflpc_gpio_set_pin_mode_output(LCD_A0_PIN, 0);
  rflpc_gpio_set_pin_mode_output(LCD_CS_PIN, 0);

  /* SPI port initialization */
  rflpc_spi_init(RFLPC_SPI1, RFLPC_SPI_MASTER, RFLPC_CCLK, 8, 2, 2);

  /* Start hardware internal reset (Datasheet p.6) */
  rflpc_gpio_clr_pin(LCD_A0_PIN);
  rflpc_gpio_set_pin(LCD_CS_PIN);
  rflpc_gpio_clr_pin(LCD_RST_PIN);
  __DELAY (1600);   /* 50 us */
  /* Internal reset has been started: set RST and wait a bit */
  rflpc_gpio_set_pin(LCD_RST_PIN);
  __DELAY (160000);   /* 5 ms */
  /* Now, reset is ended */

  /* Start initial sequence */
  __lcd_write_cmd(0xAE);       /* (1)  LCD Display: OFF */
  __lcd_write_cmd(0xA2);       /* (11) LCD drive voltage bias ratio: 1/9 */
  __lcd_write_cmd(0xA0);       /* (8)  Display RAM address SEG output correspondence: 0 */
  __lcd_write_cmd(0xC8);       /* (15) COM output scan direction: reverse */
  __lcd_write_cmd(0x22);       /* (17) Internal resistor ratio (Rb/Ra) mode: 2 */
  __lcd_write_cmd(0x2F);       /* (16) Internal power supply operating mode: 7 */
  __lcd_write_cmd(0x40);       /* (2)  Display RAM start line address: 0 */
  __lcd_write_cmd(0xAF);       /* (1)  LCD Display: ON */
  __lcd_write_cmd(0x81);       /* (18) V0 output voltage electronic volume register */
  __lcd_write_cmd(0x17);       /* (4)  MS4B of the display RAM column adress: 0111 */
  __lcd_write_cmd(0xA4);       /* (10) Display all points: Normal */

  /* Now, reset buffer */
  __reset_buffer();
  
  /* Finally, refresh the screen */
  lcd_refresh();
}

void lcd_refresh()
{
  unsigned i;

  /* Fill page 0 */
  __lcd_write_cmd(0x00);
  __lcd_write_cmd(0x10);
  __lcd_write_cmd(0xB0);

  rflpc_gpio_set_pin(LCD_A0_PIN);

  for (i = 0; i < LCD_WIDTH; ++i)
    __lcd_write_data(__buffer[i]);

  /* Fill page 1 */
  __lcd_write_cmd(0x00);
  __lcd_write_cmd(0x10);
  __lcd_write_cmd(0xB1);

  rflpc_gpio_set_pin(LCD_A0_PIN);

  for (; i < (LCD_WIDTH * 2); ++i)
    __lcd_write_data(__buffer[i]);

  /* Fill page 2 */
  __lcd_write_cmd(0x00);
  __lcd_write_cmd(0x10);
  __lcd_write_cmd(0xB2);

  rflpc_gpio_set_pin(LCD_A0_PIN);

  for (; i < (LCD_WIDTH * 3); ++i)
    __lcd_write_data(__buffer[i]);

  /* Fill page 3 */
  __lcd_write_cmd(0x00);
  __lcd_write_cmd(0x10);
  __lcd_write_cmd(0xB3);

  rflpc_gpio_set_pin(LCD_A0_PIN);

  for (; i < (LCD_WIDTH * 4); ++i)
    __lcd_write_data(__buffer[i]);
}

void lcd_reset()
{
  __reset_buffer();
  lcd_refresh();
}

void lcd_set_mode(lcd_mode_t mode)
{
  __mode = mode;
  lcd_reset();
}

int lcd_putchar(int c)
{
  unsigned char i;

  __ONLY_FOR_MODE_RET (OUTPUT, -1);

  switch (c) {
    /* Carriage return */
  case '\r':
    __buffer_cursor = 
      ((__buffer_cursor / LCD_WIDTH) * LCD_WIDTH) 
      % (__BUFFER_SIZE / LCD_WIDTH);
    break;
    /* Line feed */
  case '\n':
    __buffer_cursor =
      ((__buffer_cursor / LCD_WIDTH) + 1)
      % (__BUFFER_SIZE / LCD_WIDTH) * LCD_WIDTH;
    /* Backspace */
  case '\b':
    __buffer_cursor -= 8;
    /* Others */
  default:
    if (!FONT_IS_VALID_INDEX (default_font_8x8, (c - 31))) return -1;

    for (i = 0; i < FONT_GET_LENGTH (default_font_8x8); ++i, ++__buffer_cursor)
      __buffer[__buffer_cursor] = default_font_8x8[c - 31][i % 8];

    if (__buffer_cursor >= __BUFFER_SIZE)
      __buffer_cursor = 0;
    break;
  }

  return 0;
}

int lcd_putchar_highlighted(int c)
{
  unsigned char i;

  __ONLY_FOR_MODE_RET (OUTPUT, -1);

  switch (c) {
    /* Carriage return */
  case '\r':
    __buffer_cursor = 
      ((__buffer_cursor / LCD_WIDTH) * LCD_WIDTH) 
      % (__BUFFER_SIZE / LCD_WIDTH);
    break;
    /* Line feed */
  case '\n':
    __buffer_cursor =
      ((__buffer_cursor / LCD_WIDTH) + 1)
      % (__BUFFER_SIZE / LCD_WIDTH) * LCD_WIDTH;
    /* Others */
  default:
    if (!FONT_IS_VALID_INDEX (default_font_8x8, (c - 31))) return -1;

    for (i = 0; i < FONT_GET_LENGTH (default_font_8x8); ++i, ++__buffer_cursor)
      __buffer[__buffer_cursor] = ~(default_font_8x8[c - 31][i % 8]);

    if (__buffer_cursor >= __BUFFER_SIZE)
      __buffer_cursor = 0;
    break;
  }

  return 0;
}

char lcd_get_pixel(unsigned x, unsigned y)
{
  __ONLY_FOR_MODE_RET (DRAW, -1);

  /* Check if coordinates are valid */
  if (x < 0 || x > 127 || y < 0 || y > 31) return 0;

  return (__buffer[x + ((y / 8) * 128)] >> (y % 8)) & 1;
}

void lcd_set_pixel(unsigned x, unsigned y, unsigned char value)
{
  __ONLY_FOR_MODE (DRAW);

  /* Check if coordinates are valid */
  if (x < 0 || x > 127 || y < 0 || y > 31) return;

  if (value == 0)
    __buffer[x + ((y / 8) * 128)] &= ~(1 << (y % 8));
  else
    __buffer[x + ((y / 8) * 128)] |= (1 << (y % 8));
}

void lcd_draw_line(unsigned x, unsigned y, unsigned char len, lcd_line_t kind)
{
  /* Set first pixel */
  lcd_set_pixel(x, y, 1);

  if (len > 0)
    len -= 1;
  else if (len < 0)
    len += 1;
  
  /* Play with coordinates */
  while (len != 0) {
    if (len > 0) {
      /* Increment the good coordinate */
      switch (kind) {
      case HORIZONTAL:
	x += 1;
	break;
      case VERTICAL:
	y += 1;
	break;
      }
      /* Update len */
      len -= 1;
    }
    else if (len < 0) {
      /* Decrement the good coordinate */
      switch (kind) {
      case HORIZONTAL:
	x -= 1;
	break;
      case VERTICAL:
	y -= 1;
	break;
      }
      /* Update len */
      len += 1;
    }

    /* Set that pixel now */
    lcd_set_pixel(x, y, 1);
  }
}

void lcd_draw_rectangle(unsigned x, unsigned y, 
			unsigned width, unsigned height,
			lcd_fill_mode_t fillmode)
{
  unsigned offset;

  switch (fillmode) {
  case FILL:
    /* Draw the rectangle line per line. */
    for (offset = y + height; y < offset; ++y)
      lcd_draw_line(x, y, width, HORIZONTAL);
    break;
  case BLANK:
    /* Draw the rectangle perimeter only */
    lcd_draw_line(                x,                  y, width,  HORIZONTAL);
    lcd_draw_line(                x, (y + (height - 1)), width,  HORIZONTAL);
    lcd_draw_line(                x,                  y, height, VERTICAL);
    lcd_draw_line((x + (width - 1)),                  y, height, VERTICAL);
    break;
  }
}

void lcd_set_progress(unsigned char len)
{
  static unsigned char cursor = 0;

  __ONLY_FOR_MODE (PROGRESS);

  /* Do we have to fill ? */
  if (cursor < len) {
    for (; cursor < LCD_WIDTH && cursor < len; ++cursor) {
      __buffer[cursor] = 0xFF;
      __buffer[128 + cursor] = 0xFF;
      __buffer[256 + cursor] = 0xFF;
      __buffer[384 + cursor] = 0xFF;
    }
  }
  else { /* ... or blank ? */
    for (; cursor >= 0 && cursor >= len; --cursor) {
      __buffer[cursor] = 0;
      __buffer[128 + cursor] = 0;
      __buffer[256 + cursor] = 0;
      __buffer[384 + cursor] = 0;
    }
  }
}

void __lcd_write_cmd(unsigned char cmd)
{
  /* Write command : A0 = 0 && CS = 0 (Datasheet p.8) */
  rflpc_gpio_clr_pin(LCD_A0_PIN);
  rflpc_gpio_clr_pin(LCD_CS_PIN);

  /* Write cmd to SSP data register */
  LPC_SSP1->DR = cmd;
  do { } while ((LPC_SSP1->SR & 0x10) == 0x10);

  /* Once byte has been transmitted, set CS */
  rflpc_gpio_set_pin(LCD_CS_PIN);
}

void __lcd_write_data(unsigned char data)
{
  /* Write data : A0 = 1 && CS = 0 (Datasheet p.8) */
  rflpc_gpio_set_pin(LCD_A0_PIN);
  rflpc_gpio_clr_pin(LCD_CS_PIN);

  /* Write data to SSP data register */
  LPC_SSP1->DR = data;
  do { } while ((LPC_SSP1->SR & 0x10) == 0x10);

  /* Once byte has been transmitted, set CS */
  rflpc_gpio_set_pin(LCD_CS_PIN);
}

void __reset_buffer()
{
  unsigned i;

  for (i = 0; i < __BUFFER_SIZE; ++i)
    __buffer[i] = 0;
  
  __buffer_cursor = 0;
}
  
