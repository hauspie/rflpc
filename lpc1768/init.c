#include <stdint.h>


/* These symbols are defined by the linker script.
   They are used to find bounds of text, data and bss sections in flash/ram
*/
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _text_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


static void _zero_bss(void)
{
    uint32_t *dst = &_bss_start;
    while (dst < &_bss_end)
	*dst++ = 0;
}

static void _copy_data_section(void)
{
    uint32_t *src = &_text_end;
    uint32_t *dst = &_data_start;

    while (dst < &_data_end)
	*dst++ = *src++;
}

void _low_level_init(void)
{
    _copy_data_section();
    _zero_bss();
}
