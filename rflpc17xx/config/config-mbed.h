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

/** @file
 * Configure the library to use with the MBED platform (http://www.mbed.org)
 * */

/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created: 2011-07-04
  Time-stamp: <2014-05-28 15:40:09 (hauspie)>

  This files provides the configuration of the lib for the mbed platform

*/

#ifndef __RFLPC_CONFIG_MBED_H__
#define __RFLPC_CONFIG_MBED_H__

/** @addtogroup config MBED Platform configuration
 * @ingroup system
 * The following defines are used to configure the library to use with the MBED.
 * @{
 */

/**************************************
 *          Clock configuration       *
 **************************************/


/**
 * @name Clock configuration
 * Dividers and multipliers for controlling the CPU clock.
 * The PLL output clock frequency is given by:
    (2*PLL_MULTIPLIER*MAIN_OSCILLATOR_FREQUENCY)/INPUT_DIVIDER
    This clock is then divided by CPU_DIVIDER to clock the CPU.

    CPU is set to 96Mhz using the 12Mhz source for PLL0. (see mbed schematic,
    main oscillator is a 12Mhz crystal connected to pins XTAL1 and XTAL2)

    As this setup is suitable for USB, PPL0 can be used to clock USB (need a 48Mhz mutiple)

    With the given configuration (INPUT_DIVIDER=1, PLL_MULTIPLIER=12, CPU_DIVIDER=3),
    PLL ouput is 2*12*12000000/1 = 288Mhz
    Divided by 3 -> CPU is at 96 Mhz

    @warning Be carefull when modifing these parameters. Clock the ARM at more
    than 100Mhz is a bad idea...
*/
/** @{ */

/** If defined, the rflpc_clock_init() function will use the main oscillator. */
#define RFLPC_CLOCK_USE_MAIN_OSCILLATOR


#ifdef RFLPC_CLOCK_USE_MAIN_OSCILLATOR
/** The frequency of the main oscillator if used. */
#define RFLPC_CLOCK_MAIN_OSCILLATOR_FREQUENCY 12000000
#endif

/** PLL0 will divide its input by this value. */
#define RFLPC_CLOCK_INPUT_DIVIDER                    1
/** PLL0 will multiply its input by this value. */
#define RFLPC_CLOCK_PLL_MULTIPLIER                  12
/** The PLL0 output will be divided by this value to provide CPU clock. */
#define RFLPC_CLOCK_CPU_DIVIDER                      3
/** @} */




/**************************************
 *        DIP Pins                    *
 **************************************/
/** @name MBED DIP Pins.
    This represents the mapping between MBED breadboard and LPC17xx pins.
    They are never used in the library in itself, by can be used for application
    that targets only MBED
 */
/** @{ */
#define MBED_DIP5  RFLPC_PIN_P0_9
#define MBED_DIP6  RFLPC_PIN_P0_8
#define MBED_DIP7  RFLPC_PIN_P0_7
#define MBED_DIP8  RFLPC_PIN_P0_6
#define MBED_DIP9  RFLPC_PIN_P0_0
#define MBED_DIP10 RFLPC_PIN_P0_1
#define MBED_DIP11 RFLPC_PIN_P0_18
#define MBED_DIP12 RFLPC_PIN_P0_17
#define MBED_DIP13 RFLPC_PIN_P0_15
#define MBED_DIP14 RFLPC_PIN_P0_16
#define MBED_DIP15 RFLPC_PIN_P0_23
#define MBED_DIP16 RFLPC_PIN_P0_24
#define MBED_DIP17 RFLPC_PIN_P0_25
#define MBED_DIP18 RFLPC_PIN_P0_26
#define MBED_DIP19 RFLPC_PIN_P1_30
#define MBED_DIP20 RFLPC_PIN_P1_31
#define MBED_DIP21 RFLPC_PIN_P2_5
#define MBED_DIP22 RFLPC_PIN_P2_4
#define MBED_DIP23 RFLPC_PIN_P2_3
#define MBED_DIP24 RFLPC_PIN_P2_2
#define MBED_DIP25 RFLPC_PIN_P2_1
#define MBED_DIP26 RFLPC_PIN_P2_0
#define MBED_DIP27 RFLPC_PIN_P0_11
#define MBED_DIP28 RFLPC_PIN_P0_10
#define MBED_DIP29 RFLPC_PIN_P0_5
#define MBED_DIP30 RFLPC_PIN_P0_4

/** @} */



/**************************************
 *          LEDS configuration        *
 **************************************/

/** @name LEDs configuration */
/** @{ */
/** On which pin is the LED1 ? */
#define RFLPC_LED_1_PIN RFLPC_PIN_P1_18
/** On which pin is the LED2 ? */
#define RFLPC_LED_2_PIN RFLPC_PIN_P1_20
/** On which pin is the LED3 ? */
#define RFLPC_LED_3_PIN RFLPC_PIN_P1_21
/** On which pin is the LED4 ? */
#define RFLPC_LED_4_PIN RFLPC_PIN_P1_23
/** @} */

/**************************************
 *          Ethernet Configuration    *
 **************************************/
/**
 * @name PHY device configuration
 */
/** @{ */
/** MII Address of the PHY device.*/
#define RFLPC_ETH_PHY_ADDR (0x01)

/** Define this constant to use the MII extended register set.
    Disabling this will:

    - Disable fine control of auto-negociation (performing a link auto
      negociation will select a mode regardless the specified one (because of
      the non availability of the auto negociation ability (ANAR) register

    - Make the detection of the link mode not accurate (by using control
      registers which may not always reflect the real link mode)
*/
#define RFLPC_ETH_USE_EXTENDED_MII


/** @} */

/** @name UART Configuration */
/** @{ */
/* UART0 settings */
/** UART0 TX Pin */
#define RFLPC_UART0_TXD_PIN RFLPC_PIN_P0_2
/** UART0 RX Pin */
#define RFLPC_UART0_RXD_PIN RFLPC_PIN_P0_3
#define RFLPC_UART0_PIN_FUNCTION 1 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/* UART2 settings */
/** UART2 TX Pin */
#define RFLPC_UART2_TXD_PIN 10
/** UART2 RX Pin */
#define RFLPC_UART2_RXD_PIN 11
#define RFLPC_UART2_PIN_FUNCTION 1 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/* UART3 settings */
/** UART3 TX Pin */
#define RFLPC_UART3_TXD_PIN 0
/** UART3 RX Pin */
#define RFLPC_UART3_RXD_PIN 1
#define RFLPC_UART3_PIN_FUNCTION 2 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/** @} */


/** @} */

#endif
