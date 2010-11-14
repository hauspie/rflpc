#ifndef __LPC_INTERRUPT_H__
#define __LPC_INTERRUPT_H__

/** Enables interrupts (p. 716) */
static inline void lpc_enable_irq()
{
    __asm volatile("cpsie i");
}

/** Disable interrupts (p. 716) */
static inline void lpc_disable_irq()
{
    __asm volatile("cpsid i");
}


#endif
