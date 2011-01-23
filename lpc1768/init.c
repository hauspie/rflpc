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

#include <stdint.h>
#include "registers.h"
#include "pll.h"
#include "interrupt.h"
#include "leds.h"
#include "debug.h"

/* These symbols are defined by the linker script.
   They are used to find bounds of text, data and bss sections in flash/ram
*/
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _text_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


static uint32_t _lpc_system_clock = 4000000;

uint32_t lpc_get_system_clock()
{
    return _lpc_system_clock;
}


/* Set all bss to zero */
static void _zero_bss(void)
{
    uint32_t *dst = &_bss_start;
    while (dst < &_bss_end)
	*dst++ = 0;
}

/* Copy data section from flash to ram */
static void _copy_data_section(void)
{
    uint32_t *src = &_text_end;
    uint32_t *dst = &_data_start;

    while (dst < &_data_end)
	*dst++ = *src++;
}

/* Initialise clock to use main oscillator for PLL0.

   CPU is set to 96Mhz using the 12Mhz source for PLL0. (see mbed schematic,
   main oscillator is a 12Mhz crystal connected to pins XTAL1 and XTAL2)

   As this setup is suitable for USB, so PPL0 can be used to clock USB (need a 48Mhz mutiple)

   The output of the PLL0 is given by:
   Fcco = (2xMxFin)/N where Fin is the frequency of the source (12Mhz), M the
   multiplier and N the pre-divider (p.41)
   Fcco = 2x12x12/1 = 288Mhz
   We thus have M=12 and N=1, so we have to write 0xB in the PLL0CFG register (p. 37)
   To get the 96Mhz CPU clock, we must set the cpu clock divider to 3 (288/3 = 96).
   This is done by setting CCLKSEL register to 2 (p. 55)
*/
static void _init_clock(void)
{
    
    /* Disable IRQs so that the FEED sequence of the PLL is atomic */
    lpc_disable_irq();

    /* Lets follow the setup sequence p. 46 */
    if (LPC_PLL0STAT & (1<<24)) /* if PLL0 connected, disconnect */
    {
	/* Disconnect PLL0, set bit 1 to 0 */
	LPC_PLL0CON &= ~(1UL << 1);
	/* send feed sequence to validate the disconnection */
	LPC_PLL0_DO_FEED();
    }
    /* Disable PLL0, set bit 0 to 0 */
    LPC_PLL0CON &= ~(1UL);
    /* feed sequence to validate disable */
    LPC_PLL0_DO_FEED();


    /* Then, enable main oscillator and wait for it to be ready */
    /* Select its range, 1Mhz to 20Mhz, set 0 to bit 4 of SCS register (p. 28) */
    LPC_SCS &= ~(1UL << 4);
    /* Enables it, set 1 to bit 5 */
    LPC_SCS |= (1UL << 5);
	
    /* Wait for it to be stable by waiting a 1 on bit 6 */
    while ((LPC_SCS & (1UL << 6)) == 0);

    /* Main oscillator is stable. Can now be used as source for PLL0 */
    /* Select main oscillator as source for PLL0. Set bit 1:0 of CLKSRC register to 01 (p. 34) */
    LPC_CLKSRCSEL  = ((LPC_CLKSRCSEL & ~(3UL)) | 1);



    /* Configure PLL0 with N=1 and M=12. The value is then 0xB (p. 37) */
    LPC_PLL0CFG = 0xB;
    /* validate the configuration */
    LPC_PLL0_DO_FEED();
    
    /* Enable PLL0 */
    LPC_PLL0CON |= (1UL);
    LPC_PLL0_DO_FEED();

    /* Set the CPU Clock divider (p. 55) */
    LPC_CCLKCFG = 2; /* 2 to divide PLL0 output frequency (288Mhz) by 3 */

    /* Wait for PLL0 to lock desired frequency by monitoring bit 26 of register PLL0STAT (p. 39) */
    while (! (LPC_PLL0STAT & (1 << 26)));

    /* Connect PLL0, set bit 1 to 1 */
    LPC_PLL0CON |= (1UL << 1);
    /* validate connection */
    LPC_PLL0_DO_FEED();


    _lpc_system_clock = 96000000;

    /* system is now working on PLL0, CPU at 96Mhz */
    /* Enables the IRQs */
    lpc_enable_irq();
}

void _low_level_init(void)
{
    INIT_LEDS();
    _copy_data_section();
    _zero_bss();
    _init_clock();
}
