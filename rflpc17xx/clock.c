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
  Time-stamp: <2011-07-13 14:18:07 (hauspie)>
*/


#include "clock.h"
#include "interrupt.h"


/* Internal RC Oscillator is 4Mhz. */
#define RFLPC_CLOCK_INTERNAL_OSCILLATOR_FREQUENCY 4000000
static uint32_t _rflpc_system_clock = RFLPC_CLOCK_INTERNAL_OSCILLATOR_FREQUENCY;


/** Send a feed sequence to the PLL0FEED register (p. 40)
    This sequence validates the changes in the PLL0CON and PLL0CFG register
    @warning interrupts should be disabled to ensure that the sequence is atomic
 */
#define RFLPC_PLL0_DO_FEED() do { LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55; }while(0)


/* Initialise clock to use main oscillator for PLL0.
*/
void rflpc_clock_init(void)
{

    /* Disable IRQs so that the FEED sequence of the PLL is atomic */
    rflpc_irq_global_disable();

    /* Lets follow the setup sequence p. 46 */
    if (LPC_SC->PLL0STAT & (1<<24)) /* if PLL0 connected, disconnect */
    {
	/* Disconnect PLL0, set bit 1 to 0 */
	LPC_SC->PLL0CON &= ~(1UL << 1);
	/* send feed sequence to validate the disconnection */
	RFLPC_PLL0_DO_FEED();
    }
    /* Disable PLL0, set bit 0 to 0 */
    LPC_SC->PLL0CON &= ~(1UL);
    /* feed sequence to validate disable */
    RFLPC_PLL0_DO_FEED();


/* If we use the main oscillator */
#ifdef RFLPC_CLOCK_USE_MAIN_OSCILLATOR
    /* Then, enable main oscillator and wait for it to be ready */
    /* Select its range, if 1Mhz to 20Mhz, set 0 to bit 4 of SCS register
     * otherwise, set to 1 (p. 28)*/
    if (RFLPC_CLOCK_MAIN_OSCILLATOR_FREQUENCY >= 1000000 && RFLPC_CLOCK_MAIN_OSCILLATOR_FREQUENCY <= 20000000)
	LPC_SC->SCS &= ~(1UL << 4);
    else
	LPC_SC->SCS |= 1UL << 4;
    /* Enables it, set 1 to bit 5 */
    LPC_SC->SCS |= (1UL << 5);

    /* Wait for it to be stable by waiting a 1 on bit 6 */
    while ((LPC_SC->SCS & (1UL << 6)) == 0);

    /* Main oscillator is stable. Can now be used as source for PLL0 */
    /* Select main oscillator as source for PLL0. Set bit 1:0 of CLKSRC register to 01 (p. 34) */
    LPC_SC->CLKSRCSEL  = ((LPC_SC->CLKSRCSEL & ~(3UL)) | 1);
#endif


    /* Configure PLL0 with N= INPUT_DIVIDER and M=PLL_MULTIPLIER (p. 37) */
    LPC_SC->PLL0CFG = (((RFLPC_CLOCK_INPUT_DIVIDER-1) << 16) | (RFLPC_CLOCK_PLL_MULTIPLIER-1));
    /* validate the configuration */
    RFLPC_PLL0_DO_FEED();

    /* Enable PLL0 */
    LPC_SC->PLL0CON |= (1UL);
    RFLPC_PLL0_DO_FEED();

    /* Set the CPU Clock divider (p. 55) */
    LPC_SC->CCLKCFG = RFLPC_CLOCK_CPU_DIVIDER - 1; /* divide PLL0 output frequency */

    /* Wait for PLL0 to lock desired frequency by monitoring bit 26 of register PLL0STAT (p. 39) */
    while (! (LPC_SC->PLL0STAT & (1 << 26)));

    /* Connect PLL0, set bit 1 to 1 */
    LPC_SC->PLL0CON |= (1UL << 1);
    /* validate connection */
    RFLPC_PLL0_DO_FEED();

#ifdef RFLPC_CLOCK_USE_MAIN_OSCILLATOR
    _rflpc_system_clock = (2*RFLPC_CLOCK_PLL_MULTIPLIER*(RFLPC_CLOCK_MAIN_OSCILLATOR_FREQUENCY/RFLPC_CLOCK_INPUT_DIVIDER))/RFLPC_CLOCK_CPU_DIVIDER;
#else
    _rflpc_system_clock = (2*RFLPC_CLOCK_PLL_MULTIPLIER*(RFLPC_CLOCK_INTERNAL_OSCILLATOR_FREQUENCY/RFLPC_CLOCK_INPUT_DIVIDER))/RFLPC_CLOCK_CPU_DIVIDER;
#endif
    /* system is now working on PLL0 */
    /* Enables the IRQs */
    rflpc_irq_global_enable();
}


uint32_t rflpc_clock_get_system_clock()
{
    return _rflpc_system_clock;
}
