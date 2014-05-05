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
  Author: Christophe Bacara <christophe.bacara@etudiant.univ-lille1.fr>
*/

#ifdef RFLPC_CONFIG_ENABLE_I2C

#include <rflpc17xx/rflpc17xx.h>

#include "i2c.h"

typedef struct
{
  struct {
    /* port inutile maintenant ! TODO: Ajuster structure */
    unsigned char sda_pin:5;
    unsigned char scl_pin:5;
    unsigned char pin_function:2;
  } gpio;      
  uint32_t volatile *pclksel;
  unsigned char pclk_bit:5;
  unsigned char pconp_bit:5;
  LPC_I2C_TypeDef *conf_addr;
} rflpc_i2c_config_t;

static const rflpc_i2c_config_t _config[3] = {
  { /* I2C0 */
    { RFLPC_I2C0_SDA_PIN, RFLPC_I2C0_SCL_PIN, RFLPC_I2C0_PIN_FUNC }, /* gpio */
    &LPC_SC->PCLKSEL0,   	/* PCLKSEL register */
    14, 			/* pclk_bit */
    7,			/* pconp_bit */
    LPC_I2C0,   		/* conf_addr */
  },    
  { /* I2C1 */
    { RFLPC_I2C1_SDA_PIN, RFLPC_I2C1_SCL_PIN, RFLPC_I2C1_PIN_FUNC }, /* gpio */
    &LPC_SC->PCLKSEL1,   	/* PCLKSEL register */
    6, 			/* pclk_bit */
    19,			/* pconp_bit */
    LPC_I2C1,   		/* conf_addr */
  },
  { /* I2C2 */
    { RFLPC_I2C2_SDA_PIN, RFLPC_I2C2_SCL_PIN, RFLPC_I2C2_PIN_FUNC }, /* gpio */
    &LPC_SC->PCLKSEL1,   	/* PCLKSEL register */
    20, 			/* pclk_bit */
    26,			/* pconp_bit */
    LPC_I2C2,   		/* conf_addr */
  },    
};

int rflpc_i2c_init(rflpc_i2c_port_t port, rflpc_i2c_mode_t mode, uint8_t addr)
{
  const rflpc_i2c_config_t *i2c = &_config[port];

  if (rflpc_clock_get_system_clock() != 96000000)
    return -1;

  /* Power the port */
  LPC_SC->PCONP |= (1UL << i2c->pconp_bit);

  /* Select the desired clock */
  /* To operate at 100kHz, we set the I2C peripheral clock to 12Mhz,
   * and then the sum I2CSCLL+I2CSCLH to 120 */

  /* Set peripheral clock to 12 Mhz (96/8) */
  *(i2c->pclksel) &= ~(RFLPC_CCLK_8 << i2c->pclk_bit);
  *(i2c->pclksel) |= (RFLPC_CCLK_8 << i2c->pclk_bit);
  /* 60 + 60 = 120 */
  i2c->conf_addr->I2SCLL = 60;
  i2c->conf_addr->I2SCLH = 60;
    
  /* Select appropriate pins. Documentation says that the pins should
     configured with neither pull-up nor pull-down and in opendrain */
  /* SDA */
  rflpc_pin_set(i2c->gpio.sda_pin, i2c->gpio.pin_function, RFLPC_PIN_MODE_RESISTOR_NONE, 1);
  /* SCL */
  rflpc_pin_set(i2c->gpio.scl_pin, i2c->gpio.pin_function, RFLPC_PIN_MODE_RESISTOR_NONE, 1);
    
  /* Switch to wanted mode */
  i2c->conf_addr->I2CONCLR = 0xFFFFFFFF; /* clear all config bits */
  if (mode == RFLPC_I2C_MODE_MASTER)
    i2c->conf_addr->I2CONSET = 0x40; /* p. 466, 450 */
  else
    {
      i2c->conf_addr->I2CONSET = 0x44; /* p. 466, 454 */
      i2c->conf_addr->I2ADR0 = addr;
      i2c->conf_addr->I2MASK0 = 0;
    }
  return 0;
}

#define RFLPC_I2C_WRITE_DAT(i2c, byte)    \
  { i2c->conf_addr->I2DAT = ((byte) & 0xFF); }
#define RFLPC_I2C_WRITE_CONSET(i2c, flag) \
  { i2c->conf_addr->I2CONSET = ((flag) & 0x7C); }
#define RFLPC_I2C_WRITE_CONCLR(i2c, flag) \
  { i2c->conf_addr->I2CONCLR = ((flag) & 0x6C); }

#define RFLPC_I2C_READ_STAT(i2c)        (i2c->conf_addr->I2STAT & 0xFF)
#define RFLPC_I2C_READ_DAT(i2c)         (i2c->conf_addr->I2DAT & 0xFF);

#define RFLPC_I2C_FLAG_AA    0x04
#define RFLPC_I2C_FLAG_SI    0x08
#define RFLPC_I2C_FLAG_STO   0x10
#define RFLPC_I2C_FLAG_STA   0x20
#define RFLPC_I2C_FLAG_I2EN  0x40

/* int rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr, uint8_t byte) */
int rflpc_i2c_write(rflpc_i2c_port_t port, uint8_t addr, uint8_t *data, uint8_t nbytes, uint8_t stop)
{
  const rflpc_i2c_config_t *i2c = &_config[port];
  uint8_t status;

  while ((status = RFLPC_I2C_READ_STAT (i2c))) {
    /* Dump this status */
    printf("0x%x ", status);
    
    switch (status) {
      /*************************************************************************
       * Status 0xF8: The interrupt flag SI is not yet set. This occurs between
       *              other states and when the I2C block is not involved in a 
       *              transfer. For now, this only means that no transfer has 
       *              been started yet.
       */
    case 0xF8:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);        /* Set STA bit */
      break;

      /*************************************************************************
       * Status 0x08: START condition has been transmitted. Slave address and W
       *              bit will now be transmitted.
       */
    case 0x08:
      RFLPC_I2C_WRITE_DAT (i2c, addr);                      /* Write to I2DAT */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      break;

      /*************************************************************************
       * Status 0x10: Repeated START condition has been transmitted. Slave 
       *       	      address and W bit will now be transmitted.
       */
    case 0x10:
      RFLPC_I2C_WRITE_DAT (i2c, addr);                      /* Write to I2DAT */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      break;

      /*************************************************************************
       * Status 0x18: Previous state was 0x08 or 0x10. Slave address and W bit
       *              has been transmitted, ACK has been received. The first
       *              data byte will be transmitted.
       */
    case 0x18:
      RFLPC_I2C_WRITE_DAT (i2c, *data);                /* Write byte to I2DAT */
      nbytes -= 1;                                      /* Update bytes count */
      data += 1;                                       /* Update data pointer */
      RFLPC_I2C_WRITE_CONCLR (i2c,                   /* Clear STA and SI flag */
			      (RFLPC_I2C_FLAG_STA | RFLPC_I2C_FLAG_SI));
      break;

      /*************************************************************************
       * Status 0x20: Slave address and W bit has been transmitted. NOT ACK has
       *              been received. A STOP condition will be transmitted.
       */
    case 0x20:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);       /* Set STO flag */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      return status;

      /*************************************************************************
       * Status 0x28: Data has been transmitted. ACK has been received. If the
       *              transmitted data was the last data byte then transmit a
       *              STOP condition, otherwise transmit the next data byte.
       */
    case 0x28:
      if (nbytes > 0) { 
	/* There is remaining bytes to be transmitted, then we need to send next
	 * byte of data. */
	RFLPC_I2C_WRITE_DAT (i2c, *data);              /* Write byte to I2DAT */
	nbytes -= 1;                                    /* Update bytes count */
	data += 1;                                     /* Update data pointer */
	RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);     /* Clear SI flag */
      }
      else {
	/* Last bytes just has been transmitted. Now, we need to know which 
	   condition to transmit next. */
	if (stop) {
	  /* Send a STOP condition */
	  RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);   /* Set STO flag */
	}
	else {
	  /* Send a repeated START condition */
	  RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);   /* Set STA flag */
	}

	RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);   /* Clear SI flag */

	return 0;
      }
      break;

      /*************************************************************************
       * Status 0x30: Data has been transmitted. NOT ACK has been received. A
       *    	      STOP condition will be transmitted.
       */
    case 0x30:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);       /* Set STO flag */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      return status;

      /*************************************************************************
       * Status 0x38: Arbitration has been lost during Slave address + W or
       *   	      data. The bus has been released and not addressed Slave 
       *              mode is entered. A new START condition will be transmitted
       *              when the bus is free again.
       */
    case 0x38:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);       /* Set STA flag */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      break;
    }

    /* Wait for SI flag to be set */
    do { } while ((i2c->conf_addr->I2CONSET & RFLPC_I2C_FLAG_SI) == 0);
  }

  return -1;
}

int rflpc_i2c_read(rflpc_i2c_port_t port, uint8_t addr, uint8_t *data, uint8_t nbytes)
{
  const rflpc_i2c_config_t *i2c = &_config[port];
  uint8_t status;

  while ((status = RFLPC_I2C_READ_STAT (i2c))) {
    /* Dump this status */
    printf("0x%x ", status);

    switch (status) {
      /*************************************************************************
       * Status 0xF8: The interrupt flag SI is not yet set. This occurs
       *              between other states and when the I2C block is not 
       *              involved in a transfer. For now, this only means that no
       *              transfer has been started yet.
       */
    case 0xF8:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STA);        /* Set STA bit */
      break;

      /*************************************************************************
      * Status 0x08: START condition has been transmitted. Slave address and
      *   	     R bit will now be transmitted.
      */
    case 0x08:
      RFLPC_I2C_WRITE_DAT (i2c, (addr | 1));                /* Write to I2DAT */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      break;

      /*************************************************************************
      * Status 0x10: Repeated START condition has been transmitted. Slave
      * 	     address and R bit will now be transmitted.
      */
    case 0x10:
      RFLPC_I2C_WRITE_DAT (i2c, (addr | 1));                /* Write to I2DAT */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      break;
      
      /*************************************************************************
       * Status 0x40: Previous state was 0x08 or 0x10. Slave address + R has
       *  	      been transmitted. ACK has been received. Data will be 
       *              received and ACK returned.
       */
    case 0x40:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);     /* Set the AA flag */
      RFLPC_I2C_WRITE_CONCLR (                      /* Clear STA and SI flags */
			      i2c, RFLPC_I2C_FLAG_STA | RFLPC_I2C_FLAG_SI);
      break;

      /*************************************************************************
       * Status 0x48: Slave address + R has been transmitted. NOT ACK has been
       *  	      received. A STOP condition will be transmitted.
       */
    case 0x48:
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);   /* Set the STO flag */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);   /* Clear the SI flag */
      return status;

      /*************************************************************************
       * Status 0x50: Data has been receveid. ACK has been returned. Data will
       *	      be read from I2DAT. Additionnal data will be received. If 
       *              this is the last data byte then NOT ACK will be returned,
       *              otherwise, ACK will be returned.
       */
    case 0x50:
      *data = RFLPC_I2C_READ_DAT (i2c);                 /* Read received data */
      printf("[%x] ", *data);
      data += 1;                                       /* Update data pointer */
      nbytes -= 1;                                             /* Update size */
      
      if (nbytes > 1) {
	/* There is more than one byte to be received. We need to return an
	 * ACK after receiving the next byte. */
  	RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_AA);       /* Set AA flag */
      }
      else { 
	/* The next byte we will receive is the last one we expect. We need to
	 * return a NOT ACK after receiving it.*/
  	RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_AA);     /* Clear AA flag */
      }

      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);   /* Clear the SI flag */
      break;

      /*************************************************************************
      * Status 0x58: Data has been received. NOT ACK has been returned. Data
      *   	     will be read from I2DAT. A STOP condition will be
      *              transmitted.
      */
    case 0x58:
      *data = RFLPC_I2C_READ_DAT (i2c);                 /* Read received data */
      printf("[%x] ", *data);
      RFLPC_I2C_WRITE_CONSET (i2c, RFLPC_I2C_FLAG_STO);       /* Set STO flag */
      RFLPC_I2C_WRITE_CONCLR (i2c, RFLPC_I2C_FLAG_SI);       /* Clear SI flag */
      return 0;
    }
    
    /* Wait for SI flag to be set */
    do { } while ((i2c->conf_addr->I2CONSET & RFLPC_I2C_FLAG_SI) == 0);
  }

  return -1;
}

int rflpc_i2c_get_status(rflpc_i2c_port_t port)
{
  const rflpc_i2c_config_t *i2c = &_config[port];
  return RFLPC_I2C_READ_STAT (i2c);
}


#endif
