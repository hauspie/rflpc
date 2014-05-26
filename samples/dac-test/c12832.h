/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
  Created: <2013-12-16 00:00:00 (bacara)>
*/
/*
  TODO: Add utilitary functions for DRAW mode (draw_[rectangle|circle])
 */

#ifndef _C12832_H_
#define _C12832_H_

/*
 * C12832 Library Operating Modes :
 * - OUTPUT:   Use the LCD screen display as a classical output stream.
 *             The function "lcd_putchar()" can be used into any output
 *             formatting function like printf().
 * - DRAW:     Use the LCD screen display as a drawing display. The library 
 *             allows to get/set any pixel value for example.
 * - PROGRESS: Use the LCD screen display as a progress bar display.
 */
typedef enum lcd_mode_e {
  OUTPUT, DRAW, PROGRESS
} lcd_mode_t;

/* LCD screen dimensions */
#define LCD_WIDTH     128
#define LCD_HEIGHT    32

/* Initialize the C12832 LCD screen for use */
void lcd_init();
/* Refresh the screen display according to buffer */
void lcd_refresh();
/* Reset the screen display */
void lcd_reset();
/* Change LCD screen library's operating mode */
void lcd_set_mode(lcd_mode_t mode);

/******************************/
/******    OUTPUT MODE    *****/
/******************************/
/* Put char at buffer's cursor current position then move it forward */
int lcd_putchar(int c);
/* Same as putchar, but highlight text */
int lcd_putchar_highlighted(int c);

/******************************/
/******     DRAW MODE     *****/
/******************************/
typedef enum lcd_line_e {
  HORIZONTAL, VERTICAL
} lcd_line_t;
typedef enum lcd_fill_mode_e {
  FILL, BLANK
} lcd_fill_mode_t;
/* Return any pixel value. Returns -1 in case of errors. */
char lcd_get_pixel(unsigned x, unsigned y);
/* Set any pixel value. */
void lcd_set_pixel(unsigned x, unsigned y, unsigned char value);
/* Draw a right line from (x,y) with specified length. */
void lcd_draw_line(unsigned x, unsigned y, unsigned char len, lcd_line_t kind);
/* Draw a rectangle with (x,y) as upper-left corner. */
void lcd_draw_rectangle(unsigned x, unsigned y, 
			unsigned width, unsigned height,
			lcd_fill_mode_t fillmode);

/******************************/
/******   PROGRESS MODE   *****/
/******************************/
/* Set the progress length (Max: LCD_WIDTH) */
void lcd_set_progress(unsigned char len);

#endif /* _C12832_H_ */
