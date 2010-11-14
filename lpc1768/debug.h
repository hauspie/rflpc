#ifndef __LPC_DEBUG_H__
#define __LPC_DEBUG_H__

#include "leds.h"

/** wait until a counter reaches c. Used to wait some time. Depends on CPU frequency */
#define LPC_DELAY(c) do {int i; for (i=0 ; i < (c) ; ++i); } while (0)

/** Stops execution by an infinite loop, switching between led pattern l and its opposite */
#define LPC_STOP(l,c) do {uint32_t leds = (l); while(1){LED_VAL(leds);LPC_DELAY((c));leds=~leds;}} while(0)

#endif
