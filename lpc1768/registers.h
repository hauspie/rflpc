#ifndef __LPC1768_REGISTERS_H__
#define __LPC1768_REGISTERS_H__

/** System control and status register (p. 28) */
#define LPC_SCS (*((uint32_t*) 0x400FC1A0UL))

/** Power Control Register (p. 63) */
#define LPC_PCONP (*((uint32_t*) 0x400FC0C4UL))

#endif
