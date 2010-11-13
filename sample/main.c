#include <leds.h>

int main()
{
    int i = 0;
    uint32_t leds = LED1|LED2;
    INIT_LEDS();
    while (1)
    {
	i++;
	if (i % 100000 == 0)
	{
	    leds = ~leds;
	    LED_VAL(leds);
	}
    }
    return 0;
}
