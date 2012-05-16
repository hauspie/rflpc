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
  Time-stamp: <2012-03-21 14:43:38 (hauspie)>

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
 *          LEDS configuration        *
 **************************************/

/** @name LEDs configuration */
/** @{ */
/** Which GPIO port is used for leds? */
#define RFLPC_LED_PORT   1
/** On which pin is the LED1 ? */
#define RFLPC_LED_1_PIN 18
/** On which pin is the LED2 ? */
#define RFLPC_LED_2_PIN 20
/** On which pin is the LED3 ? */
#define RFLPC_LED_3_PIN 21
/** On which pin is the LED4 ? */
#define RFLPC_LED_4_PIN 23
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
/** UART0 PORT */
#define RFLPC_UART0_PORT    0
/** UART0 TX Pin */
#define RFLPC_UART0_TXD_PIN 2
/** UART0 RX Pin */
#define RFLPC_UART0_RXD_PIN 3
#define RFLPC_UART0_PIN_FUNCTION 1 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/* UART2 settings */
/** UART2 PORT */
#define RFLPC_UART2_PORT    0
/** UART2 TX Pin */
#define RFLPC_UART2_TXD_PIN 10
/** UART2 RX Pin */
#define RFLPC_UART2_RXD_PIN 11
#define RFLPC_UART2_PIN_FUNCTION 1 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/* UART3 settings */
/** UART3 PORT */
#define RFLPC_UART3_PORT    0
/** UART3 TX Pin */
#define RFLPC_UART3_TXD_PIN 0
/** UART3 RX Pin */
#define RFLPC_UART3_RXD_PIN 1
#define RFLPC_UART3_PIN_FUNCTION 2 /**< Depending on the platform, the pins used for uart0 are not the same and they do not use the same value for configuring multi-purpose pins (cf. user manual PINSEL* registers) . */

/** @} */

/** @} */

#endif
