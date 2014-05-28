/*
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
*/

#include "lm75b.h"

#define LM75B_TEMP_ADDR  0x00
#define LM75B_CONF_ADDR  0x01
#define LM75B_TOS_ADDR   0x02
#define LM75B_THYST_ADDR 0x03

/*
  Concerning the read operation on LM75B registers, we need to first write
  the "pointer byte" of the LM75B, i.e. set the internal pointer on the
  address of the register to be read/written. Then, we need to send a RESTART
  condition and send a read request to receive one or more bytes of data.

  Here is the I2C frame's look:
  Master: |S|.|.|.|.|.|.|D|w| |.|.|.|.|.|.|.|P| |R|
  Slave:                     A                 A
  Master:   |.|.|.|.|.|.|D|r| |d|d|d|d|d|d|d|d| (A | NA SP)
  Slave                      A                 

  S = Start condition, D = Slave device address, (N)A = (Not) Acknowledge
  R = Restart condition, SP = Stop condition, r/w = Read/Write bit.
*/

uint8_t libboard_lm75b_init()
{
   uint8_t temp_register_pointer = 0;
   rflpc_i2c_init(LM75B_I2C_PORT, RFLPC_I2C_MODE_MASTER, 0);
   /* Switch the pointer register of the lm75 to temperature register */
   rflpc_i2c_write(LM75B_I2C_PORT, LM75B_I2C_ADDR, &temp_register_pointer, 1, 0);
   return 0;
}

int16_t libboard_lm75b_get_temp()
{
   uint8_t r[2] = { 0, 0 };
   uint16_t temp = 0;

   rflpc_i2c_read(LM75B_I2C_PORT, LM75B_I2C_ADDR, r, 2, 1);

   /* Temperature is 11 bits long, and stored at MSBs. We first need to shift
      the value, and then, apply the following formula :
      If temp MSB (bit 11) = 0,
      T(°C) = temp x 0.125
      else
      T(°C) = -(2's complement of temp) x 0.125
   */
   temp = (r[0] << 3) | (r[1] >> 5);
   if (temp & 0x400) /* bit 11 is 1 -> negative value, extend the sign bits */
      temp |= 0xF800;
   temp = temp >> 3; /* For now, just divide by 8 and store resulting integer */
   
   return temp;
}
