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

#include "clock.h"
#include "interrupt.h"

static uint32_t _rflpc_system_clock = 4000000;


/** Send a feed sequence to the PLL0FEED register (p. 40) 
    This sequence validates the changes in the PLL0CON and PLL0CFG register
    @warning interrupts should be disabled to ensure that the sequence is atomic
 */
#define RFLPC_PLL0_DO_FEED() do { LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55; }while(0)


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
void rflpc_clock_init(void)
{
    
    /* Disable IRQs so that the FEED sequence of the PLL is atomic */
    lpc_disable_irq();

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


    /* Then, enable main oscillator and wait for it to be ready */
    /* Select its range, 1Mhz to 20Mhz, set 0 to bit 4 of SCS register (p. 28) */
    LPC_SC->SCS &= ~(1UL << 4);
    /* Enables it, set 1 to bit 5 */
    LPC_SC->SCS |= (1UL << 5);
	
    /* Wait for it to be stable by waiting a 1 on bit 6 */
    while ((LPC_SC->SCS & (1UL << 6)) == 0);

    /* Main oscillator is stable. Can now be used as source for PLL0 */
    /* Select main oscillator as source for PLL0. Set bit 1:0 of CLKSRC register to 01 (p. 34) */
    LPC_SC->CLKSRCSEL  = ((LPC_SC->CLKSRCSEL & ~(3UL)) | 1);



    /* Configure PLL0 with N=1 and M=12. The value is then 0xB (p. 37) */
    LPC_SC->PLL0CFG = 0xB;
    /* validate the configuration */
    RFLPC_PLL0_DO_FEED();
    
    /* Enable PLL0 */
    LPC_SC->PLL0CON |= (1UL);
    RFLPC_PLL0_DO_FEED();

    /* Set the CPU Clock divider (p. 55) */
    LPC_SC->CCLKCFG = 2; /* 2 to divide PLL0 output frequency (288Mhz) by 3 */

    /* Wait for PLL0 to lock desired frequency by monitoring bit 26 of register PLL0STAT (p. 39) */
    while (! (LPC_SC->PLL0STAT & (1 << 26)));

    /* Connect PLL0, set bit 1 to 1 */
    LPC_SC->PLL0CON |= (1UL << 1);
    /* validate connection */
    RFLPC_PLL0_DO_FEED();


    _rflpc_system_clock = 96000000;

    /* system is now working on PLL0, CPU at 96Mhz */
    /* Enables the IRQs */
    lpc_enable_irq();
}


uint32_t rflpc_clock_get_system_clock()
{
    return _rflpc_system_clock;
}
