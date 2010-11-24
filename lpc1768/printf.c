#include "uart.h"

#include <stdarg.h>

#define PUTCHAR(c) do { ++count; lpc_uart0_putchar((c)); } while(0)
#define PUTS(s) do {const char *str=s; while (*str) PUTCHAR(*str++);}while(0)


#define PUT_HEXA_DIGIT(d,a_letter) do {		\
	if ((d) >=0 && (d) <= 9)			\
	    PUTCHAR('0' + (d));				\
	else						\
	    PUTCHAR((a_letter) + ((d) - 0xA));		\
    } while (0)

#define PUT_HEXA_BYTE(b, a_letter, print_zero) do {		\
	if ((print_zero) || (b) != 0)				\
	{							\
	    PUT_HEXA_DIGIT(((b)>>4), (a_letter));		\
	    PUT_HEXA_DIGIT(((b) & 0xF), (a_letter));		\
	}							\
    } while (0)

#define PUT_HEXA_VAL(val,a_letter,bits, print_zero) do {		\
	int b = (bits) - 8;						\
	for ( ; b >= 0 ; b -= 8)					\
	    PUT_HEXA_BYTE(((val) >> b) & 0xFF, (a_letter), (print_zero)); \
    } while(0)


#define PUT_INT_DIGIT(i) do {			\
	PUTCHAR('0' + (i));			\
    } while(0);

#define PUT_INT_NUMBER(i) do {					\
	unsigned int max_pow = 1;				\
	unsigned int val = i;					\
	if (val > 1000000000)					\ /* prevent overflow */
	    max_pow = 1000000000;				\
	else							\
	{							\
	    while ((val) / (max_pow)) max_pow*=10;		\
	    max_pow /= 10;					\
	}							\
	for ( ; max_pow >= 1; max_pow /= 10)			\
	{							\
	    PUT_INT_DIGIT((val)/max_pow);			\
	    val = val % max_pow;				\
	}							\
    } while(0);

int lpc_printf(const char *format, ...)
{
    va_list args;
    int count = 0; /* number of char printed to uart */
    va_start(args, format);
    

    while (*format)
    {
	switch (*format)
	{
	    case '%':
		++format;
		switch(*format)
		{
		    case 'd':
		    case 'i':
		    {
			int i = (int) va_arg(args,int);
			if (i < 0)
			{
			    i = -i;
			    PUTCHAR('-');
			}
			PUT_INT_NUMBER(i);
		    }
		    break;
		    case 'u':
		    {
			unsigned int i = (unsigned int) va_arg(args,unsigned int);
			PUT_INT_NUMBER(i);
		    }
		    break;
		    case 'p':
		    case 'P':
		    {
			void *p = (void *)va_arg(args,void*);
			PUTCHAR('0');
			PUTCHAR('x');
			PUT_HEXA_VAL((uint32_t)p,(*format - ('p' - 'a')), sizeof(void*)<<3, 0);
		    }
		    break;
		    case 'x':
		    case 'X':
		    {
			unsigned int val = (unsigned int) va_arg(args, unsigned int);
			/* print the hexa val using lower case letter or upper
			 * case letter depending on 'x' or 'X' */
			PUT_HEXA_VAL(val,(*format - ('x' - 'a')),sizeof(unsigned int)<<3, 0);
		    }
		    break;
		    case 'c':
		    {
			char c = (char) va_arg(args, int);
			PUTCHAR(c);
			break;
		    }
		    case 's':
		    {
			const char *s = (const char*) va_arg(args, const char*);
			PUTS(s);
			break;
		    }
		    case '%':
			PUTCHAR('%');
		    break;

		    default:
		    {
			/* not handled arg, assume int and get next */
			int i = (int) va_arg(args, int);
			i = 0;
#ifdef LPC_VERBOSE_PRINTF
			PUTS("!Not handled format char: '");
			PUTCHAR(*format);
			PUTS("', treated as int!");
#endif
		    }
		    break;
		}
		break;
	    default:
		PUTCHAR(*format);
		break;
	}
	++format;
    }
    va_end(args);
    return count;
}
