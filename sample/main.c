#include <leds.h>

int main()
{
    int i;
    int clr;
    uint32_t leds = LED1|LED4;
    INIT_LEDS();
    while (1)
    {
	i++;
	if (i % 10000 == 0)
	{
	    leds = ~leds;
	    LED_VAL(leds);
	}
    }
    return 0;
}
