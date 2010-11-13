#include <stdint.h>


extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _text_end;

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
}
