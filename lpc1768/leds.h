#ifndef __LPC1768_LEDS_H__
#define __LPC1768_LEDS_H__

/* Leds control.
   See schematic p. 3 for pin number

   LEDS are connected to GPIO1
   LED1 P1.18
   LED2 P1.20
   LED3 P1.21
   LED4 P1.23
*/
#include "gpio.h"

#define LED1 (1 << 18)
#define LED2 (1 << 20)
#define LED3 (1 << 21)
#define LED4 (1 << 23)


/* Init the leds. Sets the GPIO1 led pins to output pins */
#define INIT_LEDS() do { LPC_GPIO1_DIR |= (LED1 | LED2 | LED3 | LED4);} while(0)

/** Sets the given led(s) on */
#define SET_LED(l) do { LPC_GPIO1_MASK = ~(l); LPC_GPIO1_SET = (l);} while(0)
/** Clears the given led(s) (turn them off) */
#define CLR_LED(l) do { LPC_GPIO1_MASK = ~(l); LPC_GPIO1_CLR = (l);} while(0)

#define LED_VAL(l) do { LPC_GPIO1_MASK = ~(LED1|LED2|LED3|LED4); LPC_GPIO1_PIN = (l);} while(0)

#endif
