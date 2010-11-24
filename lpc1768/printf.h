#ifndef __LPC_PRINTF_H__
#define __LPC_PRINTF_H__



/** This function is similar to stdio's printf.
    However, it only handles:
    - %d
    - %x
    - %X
    - %p
    - %c
    - %s

    If another format char is used, int is assumed to skip
    argument, but nothing is printed unless LPC_VERBOSE_PRINTF
    is defined when compiling liblpc1768.a

*/
extern int lpc_printf(const char *format,...);

#ifdef printf
#undef printf
#endif
#define printf lpc_printf

#endif
