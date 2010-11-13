#include <leds.h>


#define DELAY(c) do { int i; for (i = 0 ; i < (c) ; ++i);} while(0)

uint32_t data = (LED1|LED2);
uint32_t data2 = 0xfadebeef;
char test1 = 0xAB;
void *add_data = &data;
void *add_data2 = &test1;

int a;

int main()
{
    INIT_LEDS();

    SET_LED(LED1|LED2|LED3|LED4);
    DELAY(100000);
    CLR_LED(LED1|LED2|LED3|LED4);
    

    /* Check data section initialisation */
    if (data == (LED1 | LED2))
	SET_LED(LED1);

    DELAY(100000);
    if (test1 == 0xAB)
	SET_LED(LED2);

    DELAY(100000);
    if (add_data == &data)
	SET_LED(LED3);

    DELAY(100000);
    if (data2 == 0xfadebeef)
	SET_LED(LED4);

    
    /* Check bss section initialisation */
    DELAY(100000);
    if (a == 0)
	LED_VAL(LED1|LED4);
    else
	LED_VAL(LED2|LED3);

    return 0;
}
