#ifndef __LPC1768_GPIO_H__
#define __LPC1768_GPIO_H__

/* This files defines the GPIO control register constants.
   See user manual p. 122
*/

/* include fixed size int */
#include <stdint.h>

#define LPC_GPIO_BASE (0x2009C000UL)
#define LPC_GPIO0_BASE (LPC_GPIO_BASE)
#define LPC_GPIO1_BASE (LPC_GPIO_BASE + 0x20)
#define LPC_GPIO2_BASE (LPC_GPIO_BASE + 0x40)
#define LPC_GPIO3_BASE (LPC_GPIO_BASE + 0x60)
#define LPC_GPIO4_BASE (LPC_GPIO_BASE + 0x80)

/* GPIO0 Registers */

/** GPIO0 Pin direction control register */
#define LPC_GPIO0_DIR  (*((uint32_t*) LPC_GPIO0_BASE))

/** GPIO0 Mask register. A bit set to 0 in the mask enables the bit to be set in the PIN register */
#define LPC_GPIO0_MASK (*((uint32_t*) (LPC_GPIO0_BASE + 0x10)))
/** GPIO0 Pin register. Sets or reads the value of the GPIO pins. For pins to
 * be set/read, corresponding bit in the mask must be set to 0 */
#define LPC_GPIO0_PIN  (*((uint32_t*) (LPC_GPIO0_BASE + 0x14)))

/** GPIO0 Set register. Setting this register to 1 sets the pin enabled by the mask to 1 */
#define LPC_GPIO0_SET  (*((uint32_t*) (LPC_GPIO0_BASE + 0x18)))

/** GPIO0 clear register. Setting this register to 1 sets the pin enabled by the mask to 0 */
#define LPC_GPIO0_CLR  (*((uint32_t*) (LPC_GPIO0_BASE + 0x1C)))


/** GPIO1 Pin direction control register */
#define LPC_GPIO1_DIR  (*((uint32_t*) LPC_GPIO1_BASE))

/** GPIO1 Mask register. A bit set to 0 in the mask enables the bit to be set in the PIN register */
#define LPC_GPIO1_MASK (*((uint32_t*) (LPC_GPIO1_BASE + 0x10)))
/** GPIO1 Pin register. Sets or reads the value of the GPIO pins. For pins to
 * be set/read, corresponding bit in the mask must be set to 0 */
#define LPC_GPIO1_PIN  (*((uint32_t*) (LPC_GPIO1_BASE + 0x14)))

/** GPIO1 Set register. Setting this register to 1 sets the pin enabled by the mask to 1 */
#define LPC_GPIO1_SET  (*((uint32_t*) (LPC_GPIO1_BASE + 0x18)))

/** GPIO1 clear register. Setting this register to 1 sets the pin enabled by the mask to 0 */
#define LPC_GPIO1_CLR  (*((uint32_t*) (LPC_GPIO1_BASE + 0x1C)))


/** GPIO2 Pin direction control register */
#define LPC_GPIO2_DIR  (*((uint32_t*) LPC_GPIO2_BASE))

/** GPIO2 Mask register. A bit set to 0 in the mask enables the bit to be set in the PIN register */
#define LPC_GPIO2_MASK (*((uint32_t*) (LPC_GPIO2_BASE + 0x10)))
/** GPIO2 Pin register. Sets or reads the value of the GPIO pins. For pins to
 * be set/read, corresponding bit in the mask must be set to 0 */
#define LPC_GPIO2_PIN  (*((uint32_t*) (LPC_GPIO2_BASE + 0x14)))

/** GPIO2 Set register. Setting this register to 1 sets the pin enabled by the mask to 1 */
#define LPC_GPIO2_SET  (*((uint32_t*) (LPC_GPIO2_BASE + 0x18)))

/** GPIO2 clear register. Setting this register to 1 sets the pin enabled by the mask to 0 */
#define LPC_GPIO2_CLR  (*((uint32_t*) (LPC_GPIO2_BASE + 0x1C)))


/** GPIO3 Pin direction control register */
#define LPC_GPIO3_DIR  (*((uint32_t*) LPC_GPIO3_BASE))

/** GPIO3 Mask register. A bit set to 0 in the mask enables the bit to be set in the PIN register */
#define LPC_GPIO3_MASK (*((uint32_t*) (LPC_GPIO3_BASE + 0x10)))
/** GPIO3 Pin register. Sets or reads the value of the GPIO pins. For pins to
 * be set/read, corresponding bit in the mask must be set to 0 */
#define LPC_GPIO3_PIN  (*((uint32_t*) (LPC_GPIO3_BASE + 0x14)))

/** GPIO3 Set register. Setting this register to 1 sets the pin enabled by the mask to 1 */
#define LPC_GPIO3_SET  (*((uint32_t*) (LPC_GPIO3_BASE + 0x18)))

/** GPIO3 clear register. Setting this register to 1 sets the pin enabled by the mask to 0 */
#define LPC_GPIO3_CLR  (*((uint32_t*) (LPC_GPIO3_BASE + 0x1C)))


/** GPIO4 Pin direction control register */
#define LPC_GPIO4_DIR  (*((uint32_t*) LPC_GPIO4_BASE))

/** GPIO4 Mask register. A bit set to 0 in the mask enables the bit to be set in the PIN register */
#define LPC_GPIO4_MASK (*((uint32_t*) (LPC_GPIO4_BASE + 0x10)))
/** GPIO4 Pin register. Sets or reads the value of the GPIO pins. For pins to
 * be set/read, corresponding bit in the mask must be set to 0 */
#define LPC_GPIO4_PIN  (*((uint32_t*) (LPC_GPIO4_BASE + 0x14)))

/** GPIO4 Set register. Setting this register to 1 sets the pin enabled by the mask to 1 */
#define LPC_GPIO4_SET  (*((uint32_t*) (LPC_GPIO4_BASE + 0x18)))

/** GPIO4 clear register. Setting this register to 1 sets the pin enabled by the mask to 0 */
#define LPC_GPIO4_CLR  (*((uint32_t*) (LPC_GPIO4_BASE + 0x1C)))


#endif
