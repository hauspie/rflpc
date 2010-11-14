#ifndef __LPC_PLL_H__
#define __LPC_PLL_H__

/** Clock selection register (p.34) */
#define LPC_CLKSRCSEL (*((uint32_t*)0x400FC10CUL))

/** CPU Clock divider configuration register (p. 55) */
#define LPC_CCLKCFG (*((uint32_t*)0x400FC104UL))

/** PLL0 Control register (p. 37) */
#define LPC_PLL0CON (*((uint32_t*)0x400FC080UL))

/** PLL0 Configuration register (p. 37) */
#define LPC_PLL0CFG (*((uint32_t*)0x400FC084UL))

/** PLL0 Status register (p. 39) */
#define LPC_PLL0STAT (*((uint32_t*)0x400FC088UL))

/** PLL0 Feed register (p. 40) */
#define LPC_PLL0FEED (*((uint32_t*)0x400FC08CUL))


/** Send a feed sequence to the PLL0FEED register (p. 40) 
    This sequence validates the changes in the PLL0CON and PLL0CFG register
    @warning interrupts should be disabled to ensure that the sequence is atomic
 */
#define LPC_PLL0_DO_FEED() do { LPC_PLL0FEED = 0xAA; LPC_PLL0FEED = 0x55; }while(0)

#endif
