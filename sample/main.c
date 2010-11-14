#include <debug.h>

uint32_t data = (LED1|LED2);
uint32_t data2 = 0xfadebeef;
char test1 = 0xAB;
void *add_data = &data;
void *add_data2 = &test1;

int a,b,c,d;

int main()
{
    INIT_LEDS();

    SET_LED(LED1|LED2|LED3|LED4);
    LPC_DELAY(1000000);
    CLR_LED(LED1|LED2|LED3|LED4);
    

    /* Check data section initialisation */
    if (data == (LED1 | LED2))
	SET_LED(LED1);

    LPC_DELAY(1000000);
    if (test1 == 0xAB)
	SET_LED(LED2);

    LPC_DELAY(1000000);
    if (add_data == &data)
	SET_LED(LED3);

    LPC_DELAY(1000000);
    if (data2 == 0xfadebeef)
	SET_LED(LED4);

    
    /* Check bss section initialisation */
    LPC_DELAY(1000000);
    if (a == 0 && b == 0 && c == 0 && d == 0)
	LED_VAL(LED1|LED4);
    else
	LED_VAL(LED2|LED3);

    return 0;
}
