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
  Author: Michael Hauspie <Michael.Hauspie@univ-lille1.fr>
  Created: Jun. 28 2011
  Time-stamp: <2012-03-21 10:51:35 (hauspie)>
*/
#ifdef RFLPC_CONFIG_ENABLE_ETHERNET

#include "ethernet.h"
#include "eth_const.h"
#include "gpio.h"
#include "leds.h"
#include "../nxp/LPC17xx.h"
#include "../clock.h"


#define ETH_DELAY do { int d = 100; for ( ; d != 0 ; --d); } while(0)

static const uint8_t clock_dividers[] = {4, 6, 8, 10, 14, 20, 28, 36, 40};

/* This function allows to write value to a PHY register through the RMII
 * interface
 */
static void _write_to_phy_register_with_addr(uint8_t addr, uint8_t reg, uint16_t value)
{
    LPC_EMAC->MADR = (addr << 8)| reg;
    /* request a write cycle */
    LPC_EMAC->MWTD = value;

    /* Wait for write operation to end */
    while (LPC_EMAC->MIND & RFLPC_ETH_MIND_BUSY);
}

/* This function allows to read a value from a PHY register through the RMII
 * interface
 */

static uint16_t _read_from_phy_register_with_addr(uint8_t addr, uint8_t reg)
{
    /* Use procedure from p. 156 */
    /* Write register and peripheral address */
    LPC_EMAC->MADR = (addr << 8) | reg;
    /* Request Read cycle */
    LPC_EMAC->MCMD = 1;
    /* Wait for valid value in MRDD */
    while ((LPC_EMAC->MIND & RFLPC_ETH_MIND_BUSY) | (LPC_EMAC->MIND & RFLPC_ETH_MIND_NOT_VALID));
    LPC_EMAC->MCMD = 0;
    return LPC_EMAC->MRDD;
}

static void _eth_setup_pins()
{

    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_TXD0,    1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_TXD1,    1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_TX_EN,   1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_CRS,     1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_RXD0,    1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_RXD1,    1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_RX_ER,   1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_REF_CLK, 1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_MDC,     1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
    rflpc_pin_set(RFLPC_ETH_PIN_PORT, RFLPC_ETH_PIN_MDIO,    1, RFLPC_PIN_MODE_RESISTOR_PULL_UP, 0);
}

#define _read_from_phy_register(reg) _read_from_phy_register_with_addr(RFLPC_ETH_PHY_ADDR, (reg))
#define _write_to_phy_register(reg, val) _write_to_phy_register_with_addr(RFLPC_ETH_PHY_ADDR, (reg), (val))

int rflpc_eth_init()
{
    int i;
    uint32_t divider;

    _eth_setup_pins();

    /* Power the ethernet device
       Set bit PCENET in PCONP register (p. 141 and 64) */
    LPC_SC->PCONP |= RFLPC_ETH_PCENET_BIT;


    /* Perform a full reset of the ethernet block.
       Procedure p. 207
    */
    /* Reset mac modules */
    LPC_EMAC->MAC1 = RFLPC_ETH_MAC1_SOFT_RESET;
    /* Reset datapaths */
    LPC_EMAC->Command = RFLPC_ETH_CMD_REG_RESET;
    /* wait for reset to perform */
    ETH_DELAY;
    /* Clear reset bit */
    LPC_EMAC->MAC1 = 0;

    /* Append CRC to the frame and PAD short frames (<64 bytes) and operate in full-duplex */
    LPC_EMAC->MAC2 = RFLPC_ETH_MAC2_FULL_DUPLEX | RFLPC_ETH_MAC2_CRC_ENABLE | RFLPC_ETH_MAC2_PAD_CRC_ENABLE;

    /* Back-to-back inter-packet gap. 0x15 is the recommended value in full-duplex (p. 152) */
    LPC_EMAC->IPGT = 0x15;
    /* Non Back-to-back inter-packet gap. 0x12 and 0xC are recommended values (p. 152) */
    LPC_EMAC->IPGR = 0x12 | (0xC << 8);

    /* Collision window. 0xF and 0x37 are recommended values (p. 153) */
    LPC_EMAC->CLRT = 0xF | (0x37 << 8);

    /* Set the RMII interface to use 100Mbps mode */
    LPC_EMAC->SUPP = RFLPC_ETH_SUPP_100MBPS;

    /* Set max frame length */
    LPC_EMAC->MAXF = RFLPC_ETH_MAX_FRAME_LENGTH;

    /* Set MIIM clock multiplier to a value compatible with system clock
       (p. 154)
       IEEE 802.3 defines the MII clock to be no faster than 2.5MHz
    */
    divider = rflpc_clock_get_system_clock() / RFLPC_ETH_MAX_CLOCK;
    for (i = 0 ; i < sizeof(clock_dividers); ++i)
    {
	if (clock_dividers[i] >= divider)
	    break;
    }
    ++i; /* after the incrementation, i hold the value to store in the MII
	  * configuration register (p. 154) */
    /* set the clock and reset MII to use new parameters */
    LPC_EMAC->MCFG = ((i & 0xF) << 2) | RFLPC_ETH_MCFG_RESET_MIIM;
    LPC_EMAC->MCFG &= ~(RFLPC_ETH_MCFG_RESET_MIIM);

    /* Enable RMII interface and disable rx filter */
    LPC_EMAC->Command = RFLPC_ETH_CMD_RMII | RFLPC_ETH_CMD_FULL_DUPLEX | RFLPC_ETH_CMD_PASS_RX_FILTER;

    /* Put the PHY in reset */
    _write_to_phy_register(RFLPC_ETH_PHY_BMCR, RFLPC_ETH_BMCR_RESET);

    /* Wait for PHY to finish reset */
    while (_read_from_phy_register(RFLPC_ETH_PHY_BMCR) & RFLPC_ETH_BMCR_RESET);

    /* Read PHY Identifier to generate MAC address */


    return 1;
}

int rflpc_eth_link_state()
{
    return (_read_from_phy_register(RFLPC_ETH_PHY_BMSR) & RFLPC_ETH_BMSR_LINK_STATUS) == RFLPC_ETH_BMSR_LINK_STATUS;
}

int rflpc_eth_link_auto_negociate(int max_desired_mode)
{
    int mode;
    uint16_t bmcr;
#ifdef RFLPC_ETH_USE_EXTENDED_MII
    uint16_t anar;
#endif


    /* Do not perform autonegociation if link is down */
    if (!rflpc_eth_link_state())
	return -1;

#ifdef RFLPC_ETH_USE_EXTENDED_MII
    /* To set maximum mode, we set the ANAR register with desired value */
    anar = _read_from_phy_register(RFLPC_ETH_PHY_ANAR);
    /* remove all caps */
    anar &= ~(RFLPC_ETH_ANAR_10_FD | RFLPC_ETH_ANAR_TX | RFLPC_ETH_ANAR_TX_FD);
    anar |= RFLPC_ETH_ANAR_10; /* minimum is 10 Mbps, half duplex */

    switch (max_desired_mode)
    {
	case RFLPC_ETH_LINK_MODE_100FD:
	    anar |= RFLPC_ETH_ANAR_TX_FD;
	case RFLPC_ETH_LINK_MODE_100HD:
	    anar |= RFLPC_ETH_ANAR_TX;
	    break;
	case RFLPC_ETH_LINK_MODE_10FD:
	    anar |= RFLPC_ETH_ANAR_10_FD;
	    break;
	default:
	    break;
    }
    _write_to_phy_register(RFLPC_ETH_PHY_ANAR, anar);
#endif

    bmcr = _read_from_phy_register(RFLPC_ETH_PHY_BMCR);
    /* Enable auto-negociation */
    bmcr |= RFLPC_ETH_BMCR_ENABLE_AUTO_NEG;
    /* force auto negociation restart */
    bmcr |= RFLPC_ETH_BMCR_RESTART_AUTO_NEG;
    _write_to_phy_register(RFLPC_ETH_PHY_BMCR, bmcr);
    /* wait for auto-negociation to finish  */
    while ( !(_read_from_phy_register(RFLPC_ETH_PHY_BMSR) & RFLPC_ETH_BMSR_AUTO_NEG_COMPLETE));

    if (!rflpc_eth_link_state())
	return -1;

    /* retrieve auto-negociated parameters */
    mode = rflpc_eth_get_link_mode();
    if (mode & RFLPC_ETH_LINK_MODE_SPEED_BIT) /* 100 Mbps */
	LPC_EMAC->SUPP = RFLPC_ETH_SUPP_100MBPS;
    else
	LPC_EMAC->SUPP = RFLPC_ETH_SUPP_10MBPS;
    if (mode & RFLPC_ETH_LINK_MODE_DUPLEX_BIT) /* Full duplex */
    {
	LPC_EMAC->MAC2 |= RFLPC_ETH_MAC2_FULL_DUPLEX;
	LPC_EMAC->Command |= RFLPC_ETH_CMD_FULL_DUPLEX;
    }
    else
    {
	LPC_EMAC->MAC2 &= ~RFLPC_ETH_MAC2_FULL_DUPLEX;
	LPC_EMAC->Command &= ~RFLPC_ETH_CMD_FULL_DUPLEX;
    }
    return 0;
}

void rflpc_eth_set_link_mode(int mode)
{
    uint16_t bmcr = _read_from_phy_register(RFLPC_ETH_PHY_BMCR);
    uint32_t mac2 = LPC_EMAC->MAC2;
    uint32_t cmd = LPC_EMAC->Command;
    uint32_t supp;
    /* disable autonegociation */
    bmcr &= ~RFLPC_ETH_BMCR_ENABLE_AUTO_NEG;
    _write_to_phy_register(RFLPC_ETH_PHY_BMCR, bmcr);
    bmcr = _read_from_phy_register(RFLPC_ETH_PHY_BMCR);

    /* duplex */
    if (mode & RFLPC_ETH_LINK_MODE_DUPLEX_BIT)
    {
	/* full duplex */
	mac2 |= RFLPC_ETH_MAC2_FULL_DUPLEX;
	cmd |= RFLPC_ETH_CMD_FULL_DUPLEX;
	bmcr |= RFLPC_ETH_BMCR_DUPLEX_MODE;
    }
    else
    {
	/* half duplex */
	mac2 &= ~RFLPC_ETH_MAC2_FULL_DUPLEX;
	cmd &= ~RFLPC_ETH_CMD_FULL_DUPLEX;
	bmcr &= ~RFLPC_ETH_BMCR_DUPLEX_MODE;
    }

    /* speed */
    if (mode & RFLPC_ETH_LINK_MODE_SPEED_BIT)
    {
	/* 100 Mbps */
	supp = RFLPC_ETH_SUPP_100MBPS;
	bmcr |= RFLPC_ETH_BMCR_SPEED_SELECT;
    }
    else
    {
	/* 10 Mbps */
	supp = 0;
	bmcr &= ~RFLPC_ETH_BMCR_SPEED_SELECT;
    }
    _write_to_phy_register(RFLPC_ETH_PHY_BMCR, bmcr);
    LPC_EMAC->MAC2 = mac2;
    LPC_EMAC->Command = cmd;
    LPC_EMAC->SUPP = supp;
}

int rflpc_eth_get_link_mode()
{
#ifdef RFLPC_ETH_USE_EXTENDED_MII
    uint16_t physts = _read_from_phy_register(RFLPC_ETH_PHY_PHYSTS);

    if (physts & RFLPC_ETH_PHYSTS_SPEED_STATUS) /* 10 Mbps */
    {
	if (physts & RFLPC_ETH_PHYSTS_DUPLEX_STATUS) /* full duplex */
	    return RFLPC_ETH_LINK_MODE_10FD;
	return RFLPC_ETH_LINK_MODE_10HD;
    }
    /* 100 Mbps */
    if (physts & RFLPC_ETH_PHYSTS_DUPLEX_STATUS) /* full duplex */
	return RFLPC_ETH_LINK_MODE_100FD ;
    return RFLPC_ETH_LINK_MODE_100HD;
#else /* detect mode from BMCR register (not accurate) */
    uint16_t bmcr = _read_from_phy_register(RFLPC_ETH_PHY_BMCR);
    if (bmcr & RFLPC_ETH_BMCR_SPEED_SELECT) /* 100 Mbps */
    {
	if (bmcr & RFLPC_ETH_BMCR_DUPLEX_MODE) /* full duplex */
	    return RFLPC_ETH_LINK_MODE_100FD;
	return RFLPC_ETH_LINK_MODE_100HD; /* half */
    }
    /* 10 Mbps */
    if (bmcr & RFLPC_ETH_BMCR_DUPLEX_MODE) /* full duplex */
	return RFLPC_ETH_LINK_MODE_10FD;
    return RFLPC_ETH_LINK_MODE_10HD;
#endif
}

void rflpc_eth_set_rx_base_addresses(rflpc_eth_descriptor_t *descriptors, rfEthRxStatus *status, int count)
{
    /* Turn reception off while modifying descriptors */
    LPC_EMAC->Command &= ~RFLPC_ETH_CMD_RX_ENABLE;
    LPC_EMAC->MAC1 &= ~RFLPC_ETH_MAC1_RECEIVE_ENABLE ;

    LPC_EMAC->RxDescriptor = (uint32_t) descriptors;
    LPC_EMAC->RxStatus = (uint32_t) status;
    LPC_EMAC->RxDescriptorNumber = count - 1; /* stored in -1 encoding */
    LPC_EMAC->RxConsumeIndex = LPC_EMAC->RxProduceIndex; /* force the queue to be empty */

    /* Now that the receive datapath is set, enable reception */
    LPC_EMAC->Command |= RFLPC_ETH_CMD_RX_ENABLE;
    LPC_EMAC->MAC1 |= RFLPC_ETH_MAC1_RECEIVE_ENABLE ;
}

void rflpc_eth_set_tx_base_addresses(rflpc_eth_descriptor_t *descriptors, rfEthTxStatus *status, int count)
{
    /* Turn transmission off while modifying descriptors */
    LPC_EMAC->Command &= ~RFLPC_ETH_CMD_TX_ENABLE;

    LPC_EMAC->TxDescriptor = (uint32_t) descriptors;
    LPC_EMAC->TxStatus = (uint32_t) status;
    LPC_EMAC->TxDescriptorNumber = count - 1; /* stored in -1 encoding */
    LPC_EMAC->TxProduceIndex = LPC_EMAC->TxConsumeIndex; /* force the queue to be empty */

    /* Turn transmission on */
    LPC_EMAC->Command |= RFLPC_ETH_CMD_TX_ENABLE;
}

void rflpc_eth_get_mac_address(uint8_t *addr)
{
    addr[0] = (LPC_EMAC->SA0 >> 8) & 0xFF;
    addr[1] = (LPC_EMAC->SA0) & 0xFF;

    addr[2] = (LPC_EMAC->SA1 >> 8) & 0xFF;
    addr[3] = (LPC_EMAC->SA1) & 0xFF;

    addr[4] = (LPC_EMAC->SA2 >> 8) & 0xFF;
    addr[5] = (LPC_EMAC->SA2) & 0xFF;

}
void rflpc_eth_set_mac_address(const uint8_t *addr)
{
    LPC_EMAC->SA0 = addr[0] << 8 | addr[1];
    LPC_EMAC->SA1 = addr[2] << 8 | addr[3];
    LPC_EMAC->SA2 = addr[4] << 8 | addr[5];
}

#include "../tinylibc/printf.h"
#define DUMP_REGISTER(a) printf("%s: %x (%d)\r\n", #a,a,a);
void rflpc_eth_dump_internals()
{
    DUMP_REGISTER(LPC_EMAC->Command);
    DUMP_REGISTER(LPC_EMAC->MAC1);
    DUMP_REGISTER(LPC_EMAC->MAC2);
    DUMP_REGISTER(LPC_EMAC->SUPP);
    DUMP_REGISTER(LPC_EMAC->IntStatus);



    DUMP_REGISTER(LPC_EMAC->TxDescriptor);
    DUMP_REGISTER(LPC_EMAC->TxStatus);
    DUMP_REGISTER(LPC_EMAC->TxDescriptorNumber);
    DUMP_REGISTER(LPC_EMAC->TxProduceIndex);
    DUMP_REGISTER(LPC_EMAC->TxConsumeIndex);


    DUMP_REGISTER(LPC_EMAC->RxDescriptor);
    DUMP_REGISTER(LPC_EMAC->RxStatus);
    DUMP_REGISTER(LPC_EMAC->RxDescriptorNumber);
    DUMP_REGISTER(LPC_EMAC->RxProduceIndex);
    DUMP_REGISTER(LPC_EMAC->RxConsumeIndex);
}

#endif /* ENABLE_ETHERNET */
