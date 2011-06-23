/* This file is part of rfBareMbed.                        
 *									 
 * rfBareMbed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rfBareMbed is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rfBareMbed.  If not, see <http://www.gnu.org/licenses/>.	 
 */

#include <debug.h>
#include <drivers/uart.h>
#include <drivers/rit.h>
#include <drivers/sys_tick_timer.h>

#include <printf.h>
#include <interrupt.h>
#include <clock.h>

uint32_t data = (LED1|LED2);
uint32_t data2 = 0xfadebeef;
char test1 = 0xAB;
void *add_data = &data;
void *add_data2 = &test1;

int a,b,c,d;
char e;



int putchar(int c)
{
    rflpc_uart0_putchar(c);
    return c;
}

void test_data_bss()
{
    rflpc_led_set(LED1|LED2|LED3|LED4);
    RFLPC_DELAY(1000000);
    rflpc_led_clr(LED1|LED2|LED3|LED4);
    

    /* Check data section initialisation */
    if (data == (LED1 | LED2))
	rflpc_led_set(LED1);

    RFLPC_DELAY(1000000);
    if (test1 == 0xAB)
	rflpc_led_set(LED2);

    RFLPC_DELAY(1000000);
    if (add_data == &data)
	rflpc_led_set(LED3);

    RFLPC_DELAY(1000000);
    if (data2 == 0xfadebeef)
	rflpc_led_set(LED4);

    
    /* Check bss section initialisation */
    RFLPC_DELAY(1000000);
    if (a == 0 && b == 0 && c == 0 && d == 0)
	rflpc_led_val(LED1 | LED4);
    else
	rflpc_led_val(LED2 | LED3);
    RFLPC_DELAY(1000000);
}

void test_uart()
{
    char c = 'a';

    
    while (1)
    {
	rflpc_uart0_putchar(c);
	++c;
	if (c == 'z'+1)
	{
	    rflpc_uart0_putchar('\n');
	    rflpc_uart0_putchar('\r');
	    break;
	}
    }
    
}

void test_echo()
{
    printf("Testing uart0 polling\r\n");
    printf("Press Q to quit test\r\n");
    while (1)
    {
	char c = rflpc_uart0_getchar();
	printf("Received '%c'\n\r", c);
	if (c == 'Q')
	    break;
    }
}

void test_printf()
{
    int a = -3;
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


void print_interrupts(const uint32_t *addr)
{
    int i;
    for (i = 0; i < IRQn_COUNT ; ++i)
    {
	printf("%p: 0x%x\r\n", addr+i, addr[i]);
    }
}

RFLPC_IRQ_HANDLER uart0_rx()
{
    char c = rflpc_uart0_getchar();
    printf("Received '%c' via interrupt\r\n", c);
}
void test_echo_irq()
{
    printf("Testing uart0 reception via interruption\r\n");
    rflpc_uart0_set_rx_callback(uart0_rx);
}


RFLPC_IRQ_HANDLER _rit_callback()
{
    printf("RIT INTERRUPT: %x\r\n", rflpc_rit_get_counter_value());
    rflpc_rit_clear_pending_interrupt();
}
void test_rit()
{
    rflpc_rit_enable();
    rflpc_rit_set_callback(20000000, 0, 1, _rit_callback);
}

RFLPC_IRQ_HANDLER _sys_timer_callback()
{
    static int count = 0;
    static int count2 = 0;
    if (++count == 10)
    {
	count = 0;
	++count2;
	if (count2 == 10)
	{
	    printf("10s elapsed\r\n");
	    rflpc_sys_timer_stop();
	}
	else
	    printf("Systimer interrupt\r\n");
    }
}

void test_sys_timer()
{
    rflpc_sys_timer_init();
    rflpc_sys_timer_set_period(100000);
    rflpc_sys_timer_set_callback(_sys_timer_callback);
}

extern uint32_t _interrupt_start;
extern uint32_t _interrupt_end;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern uint32_t _text_end;


void print_sections()
{
    printf("Interrupt vector: %p %p\r\n", &_interrupt_start, &_interrupt_end);
    printf(".text: 0x00 %p\r\n", &_text_end);
    printf(".data: %p %p\r\n", &_data_start, &_data_end);
    printf(".bss: %p %p %x %x\r\n", &_bss_start, &_bss_end, _bss_start, _bss_end);
}

int main()
{
    int led[6] = {LED1, LED2, LED3, LED4, LED3, LED2};
    int i = 0;

    test_data_bss();

    if (rflpc_uart0_init() == -1)
	RFLPC_STOP(LED1 | LED3, 1000000);
    test_uart();

    printf("rfBareMbed sample test\r\n");
    printf("System clock is %d Hz\r\n", rflpc_clock_get_system_clock());

    test_printf();
/*    test_echo();*/
    test_echo_irq();
/*    test_rit();*/
    test_sys_timer();

    while (1)
    {
	RFLPC_DELAY(1000000);
	rflpc_led_val(led[i++]);
	if (i >= 6)
	    i = 0;
    }

    RFLPC_STOP(LED2 | LED3, 1000000);

    return 0;
}
