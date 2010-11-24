#include <debug.h>
#include <uart.h>

#include <printf.h>

uint32_t data = (LED1|LED2);
uint32_t data2 = 0xfadebeef;
char test1 = 0xAB;
void *add_data = &data;
void *add_data2 = &test1;

int a,b,c,d;



void test_data_bss()
{
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
    LPC_DELAY(1000000);
}

void test_uart()
{
    char c = 'a';

    
    while (1)
    {
	lpc_uart0_putchar(c);
	++c;
	if (c == 'z'+1)
	{
	    lpc_uart0_putchar('\n');
	    lpc_uart0_putchar('\r');
	    c = 'a';
	}
    }
    
}

void test_printf()
{
    unsigned int a = 0xFFFFFFFF;
    unsigned int b = 0xCAFED0CA;
    unsigned char byte = 0xFF;
    const char *str = "Pouet world!";
    char c = 'J';

    printf("Hello world\r\n");
    printf("Hello int world: '%d'\r\n", a);
    printf("Hello unsigned int world: '%u'\r\n", a);
    printf("Hello char world: '%c'\r\n", c);
    printf("Hello string world: '%s'\r\n", str);
    printf("Hello hexa world: '%x'\r\n", b);
    printf("Hello pointer world: '%p' '%P'\r\n", &b, str);
    printf("Hello multiple world: '%x' '%x' '%X' '%s'\r\n", b, byte,b, str);
}

int main()
{
    INIT_LEDS();
    if (lpc_uart0_init() == -1)
	LPC_STOP(LED1 | LED3, 1000000);

    test_data_bss();
/*    test_uart();*/
    test_printf();
    
    LPC_STOP(LED2 | LED3, 1000000);

    return 0;
}
