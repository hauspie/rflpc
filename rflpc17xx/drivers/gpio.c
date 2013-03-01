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
#include "gpio.h"
#include "../tools.h"

void rflpc_gpio_use_pin(rflpc_pin_t pin)
{
    /* Use GPIO function for pin */
    rflpc_pin_set(pin, 0, 0, 0);
}

/* The pointers are const, not what is pointed, hence the const AFTER the '*' */
static LPC_GPIO_TypeDef * const _gpio_ports[] = 
{
    LPC_GPIO0,
    LPC_GPIO1,
    LPC_GPIO2,
    LPC_GPIO3,
    LPC_GPIO4,
};

/** Macro used to convert a digit in the corresponding pointer */
#define RFLPC_GPIO_BASE(pin) _gpio_ports[RFLPC_PIN_GET_PORT((pin))]

/** Sets a pin to input mode */
void rflpc_gpio_set_pin_mode_input(rflpc_pin_t pin)
{
    rflpc_gpio_use_pin(pin);
    RFLPC_CLR_BIT(RFLPC_GPIO_BASE(pin)->FIODIR, RFLPC_PIN_GET_PIN(pin));
}

/** Sets a pin to output mode */
void rflpc_gpio_set_pin_mode_output(rflpc_pin_t pin, uint8_t val)
{
    rflpc_gpio_use_pin(pin);
    rflpc_gpio_set_pin_val(pin, val);
    RFLPC_SET_BIT(RFLPC_GPIO_BASE(pin)->FIODIR, RFLPC_PIN_GET_PIN(pin));
}

/** Set a pin (put a logical 1 on it) */
void rflpc_gpio_set_pin(rflpc_pin_t pin)
{
    RFLPC_GPIO_BASE(pin)->FIOMASK = ~(1 << (RFLPC_PIN_GET_PIN(pin)));
    RFLPC_GPIO_BASE(pin)->FIOSET = 0xffffffff;
}

/** Clear a pin (put a logical 0 on it) */
void rflpc_gpio_clr_pin(rflpc_pin_t pin)
{
    RFLPC_GPIO_BASE(pin)->FIOMASK = ~(1 << RFLPC_PIN_GET_PIN(pin));
    RFLPC_GPIO_BASE(pin)->FIOCLR = 0xffffffff;
}

/** Set multiple pins from a mask */
void rflpc_gpio_set_pins_from_mask(uint8_t gpio, uint32_t mask)
{
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOMASK = ~(mask);
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOSET |= (mask);
}

/** Clean multiple pins from a mask */
void rflpc_gpio_clr_pins_from_mask(uint8_t gpio, uint32_t mask)
{
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOMASK = ~(mask);
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOCLR |= (mask);
}

/** Sets the value of a pin (0 or 1) */
void rflpc_gpio_set_pin_val(rflpc_pin_t pin, int val)
{
    if (val)
	rflpc_gpio_set_pin(pin); 
    else 
	rflpc_gpio_clr_pin(pin);
}

/** Sets the value of multiple pins of a given port from a mask and a value */
void rflpc_gpio_set_val(uint8_t gpio, uint32_t val, uint32_t mask)
{
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOMASK = (mask);
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOPIN = (val);
}

/** Gets the value of a pin */
int rflpc_gpio_get_pin(rflpc_pin_t pin)
{
    RFLPC_GPIO_BASE(pin)->FIOMASK = ~(1 << (RFLPC_PIN_GET_PIN(pin)));
    return (RFLPC_GPIO_BASE(pin)->FIOPIN >> RFLPC_PIN_GET_PIN(pin)) & 1;
}
/** Gets the value of the entire port */
uint32_t rflpc_gpio_get_val(uint8_t gpio)
{
    RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOMASK = 0;
    return RFLPC_GPIO_BASE(RFLPC_PIN_MAKE(gpio,0))->FIOPIN;
}
