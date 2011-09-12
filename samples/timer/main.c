/* This file is part of rflpc. Copyright 2010-2011 Michael Hauspie                        
 *									 
 * rflpc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by	 
 * the Free Software Foundation, either version 3 of the License, or	 
 * (at your option) any later version.					 
 * 									 
 * rflpc is distributed in the hope that it will be useful,		 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	 
 * GNU General Public License for more details.				 
 * 									 
 * You should have received a copy of the GNU General Public License	 
 * along with rflpc.  If not, see <http://www.gnu.org/licenses/>.	 
 */
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 
  Time-stamp: <2011-07-13 14:27:20 (hauspie)>
*/
#include <debug.h>
#include <drivers/uart.h>
#include <drivers/rit.h>
#include <drivers/sys_tick_timer.h>
#include <drivers/ethernet.h>

#include <printf.h>
#include <interrupt.h>
#include <clock.h>

uint32_t data = (RFLPC_LED_1|RFLPC_LED_2);
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
    rflpc_led_set(RFLPC_LED_1|RFLPC_LED_2|RFLPC_LED_3|RFLPC_LED_4);
    RFLPC_DELAY(1000000);
    rflpc_led_clr(RFLPC_LED_1|RFLPC_LED_2|RFLPC_LED_3|RFLPC_LED_4);
    

    /* Check data section initialisation */
    if (data == (RFLPC_LED_1 | RFLPC_LED_2))
	rflpc_led_set(RFLPC_LED_1);

    RFLPC_DELAY(1000000);
    if (test1 == 0xAB)
	rflpc_led_set(RFLPC_LED_2);

    RFLPC_DELAY(1000000);
    if (add_data == &data)
	rflpc_led_set(RFLPC_LED_3);

    RFLPC_DELAY(1000000);
    if (data2 == 0xfadebeef)
	rflpc_led_set(RFLPC_LED_4);

    
    /* Check bss section initialisation */
    RFLPC_DELAY(1000000);
    if (a == 0 && b == 0 && c == 0 && d == 0)
	rflpc_led_val(RFLPC_LED_1 | RFLPC_LED_4);
    else
	rflpc_led_val(RFLPC_LED_2 | RFLPC_LED_3);
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
    for (a = 1 ; a <= 100000 ; a*=10)
	printf("%d\r\n", a);
    for (a = 1 ; a <= 0x100000 ; a *= 16)
	printf("%x\r\n", a);
}


void print_interrupts(const uint32_t *addr)
{
    int i;
    for (i = 0; i < RFLPC_IRQn_COUNT ; ++i)
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

volatile static int request_autoneg = 0;
volatile static int autoneg_mode = RFLPC_ETH_LINK_MODE_100FD;

RFLPC_IRQ_HANDLER _test_ethernet_serial_handler()
{
    char c = rflpc_uart0_getchar();

    switch (c)
    {
	case 's':
	    request_autoneg = 1;
	    autoneg_mode = autoneg_mode ^ RFLPC_ETH_LINK_MODE_SPEED_BIT;
	    break;
	case 'd':
	    request_autoneg = 1;
	    autoneg_mode = autoneg_mode ^ RFLPC_ETH_LINK_MODE_DUPLEX_BIT;

	break;
    }
}

void test_ethernet()
{
    int old_link_state = 10;
    rflpc_eth_init();

    rflpc_led_val(0);

    printf("Autonegociating to 100Mbps full duplex\r\n");
    rflpc_eth_link_auto_negociate(RFLPC_ETH_LINK_MODE_100FD);
    printf("Waiting for link to be up\r\n");
    while(!rflpc_eth_link_state());

    printf("Ok. Press:\r\n");
    printf("- 's' to toggle speed\r\n");
    printf("- 'd' to toggle duplex\r\n");
    rflpc_uart0_set_rx_callback(_test_ethernet_serial_handler);

    

    while (1)
    {
	int current_link_state;
	
	if (request_autoneg)
	{
	    int mode;
	    printf("Starting auto-negociation to %d Mbps %s duplex\r\n", autoneg_mode & RFLPC_ETH_LINK_MODE_SPEED_BIT ? 100 : 10, autoneg_mode & RFLPC_ETH_LINK_MODE_DUPLEX_BIT ? "Full" : "Half");
	    if (rflpc_eth_link_auto_negociate(autoneg_mode) != -1)
	    {
		mode = rflpc_eth_get_link_mode();
		printf("Done -> now in %d Mbps %s duplex\r\n", mode & RFLPC_ETH_LINK_MODE_SPEED_BIT ? 100 : 10, mode & RFLPC_ETH_LINK_MODE_DUPLEX_BIT ? "Full" : "Half");
	    }
	    else
		printf("Failed auto-negociation\r\n");
	    request_autoneg = 0;
	}

	current_link_state = rflpc_eth_link_state();
	if (current_link_state != old_link_state)
	{
	    old_link_state = current_link_state;
	    printf("Eth link is : %s\r\n", current_link_state ? "Up" : "Down");
	    if (current_link_state)
		rflpc_led_set(RFLPC_LED_1);
	    else
		rflpc_led_clr(RFLPC_LED_1);
	}
    }
}

void test_led()
{
    int i;
    rflpc_led_set(RFLPC_LED_1);
    RFLPC_DELAY(1000000);
    rflpc_led_set(RFLPC_LED_2);
    RFLPC_DELAY(1000000);
    rflpc_led_set(RFLPC_LED_3);
    RFLPC_DELAY(1000000);
    rflpc_led_set(RFLPC_LED_4);
    RFLPC_DELAY(1000000);

    for (i = 0 ; i < 16 ; ++i)
    {
	rflpc_led_binary_value(i);
	RFLPC_DELAY(1000000);
    }
}

int main()
{
    int led[6] = {RFLPC_LED_1, RFLPC_LED_2, RFLPC_LED_3, RFLPC_LED_4, RFLPC_LED_3, RFLPC_LED_2};
    int i = 0;

    /* test_led(); */
    test_data_bss();

    if (rflpc_uart0_init() == -1)
	RFLPC_STOP(RFLPC_LED_1 | RFLPC_LED_3, 1000000);
    test_uart();

    printf("rflpc sample test\r\n");
    printf("System clock is %d Hz\r\n", rflpc_clock_get_system_clock());

/*    test_printf();*/
/*    test_echo();*/
/*    test_echo_irq();*/
/*    test_rit();*/
/*    test_sys_timer();*/
    test_ethernet();

    while (1)
    {
	RFLPC_DELAY(1000000);
	rflpc_led_val(led[i++]);
	if (i >= 6)
	    i = 0;
    }

    RFLPC_STOP(RFLPC_LED_2 | RFLPC_LED_3, 1000000);

    return 0;
}
