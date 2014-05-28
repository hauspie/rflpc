/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
*/

/*
  This file allows operations with the NXP LM75B "Digital temperature sensor
  and thermal watchdog". Product data sheet by NXP should be located in the
  datasheet folder of this library. Hardware manipulation is made by the I2C
  driver of the RFLPC library.
*/

#ifndef __LIBBOARD_LM75B_H__
#define __LIBBOARD_LM75B_H__

#include <rflpc17xx/drivers/i2c.h>

/* Default configuration for mbed application board. You should verify these
 * values match your own configuration. */
#define LM75B_I2C_PORT   RFLPC_I2C_PORT2
#define LM75B_I2C_ADDR   0x90

/** Initializes the I2C driver on the port defined by LM75B_I2C_PORT.
 * @return If initialization was successful, returns 0. Returns -1 otherwise.
 */
uint8_t libboard_lm75b_init();
int16_t libboard_lm75b_get_temp();

#endif /* __LIBBOARD_LM75B_H__ */
