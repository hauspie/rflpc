/* This file is part of rflpc.
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
 * TIMER0/1/2/3 device driver
 */
/*
  Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
  Created:
  Time-stamp: <2013-03-01 13:48:26 (hauspie)>
*/
#ifndef __RFLPC_TIMER_H__
#define __RFLPC_TIMER_H__

#ifdef RFLPC_CONFIG_ENABLE_TIMERS

#include "../nxp/LPC17xx.h"
#include "../clock.h"
#include "../tools.h"
#include "../interrupt.h"


/** @addtogroup timer Timers
 * @ingroup drivers
 *
 * All four timers provides the same functionality.
 * The prescale counter is incremented at each timer clock tick.
 * When the prescale counter reaches the prescale register value, the counter is incremented.
 * The value of the counter can then be used as time value or match against four match registers to trigger an interrupt.
 *
 * @{ */

/**
   Value to use for the timer value in functions
 */
typedef enum
{
    RFLPC_TIMER0, /**< Timer 0 */
    RFLPC_TIMER1, /**< Timer 1 */
    RFLPC_TIMER2, /**< Timer 2 */
    RFLPC_TIMER3,  /**< Timer 3 */
    RFLPC_TIMER_PWM /**< PWM Timer */
} rflpc_timer_t;


/** Match registers for interrupt generation */
typedef enum
{
    RFLPC_TIMER_MATCH0, /**< Match register 0 */
    RFLPC_TIMER_MATCH1, /**< Match register 1 */
    RFLPC_TIMER_MATCH2, /**< Match register 2 */
    RFLPC_TIMER_MATCH3, /**< Match register 3 */
} rflpc_timer_match_t;


/** Options for interrupt generation */
enum
{
    RFLPC_TIMER_IRQ_ON_MATCH = 1,    /**< Trigger an interrupt when counter matches the match register */
    RFLPC_TIMER_RESET_ON_MATCH = 2,  /**< Reset the timer when counter matches the match register */
    RFLPC_TIMER_STOP_ON_MATCH = 4,   /**< Stop the timer when the counter matches the match register */
};

/**
 * @brief Power and clock the given timer
 *
 * @param timer
 */
extern void rflpc_timer_enable(rflpc_timer_t timer);

/**
 * @brief disable the given timer
 *
 * @param timer
 */
extern void rflpc_timer_disable(rflpc_timer_t timer);


/** 
 * Query the running state of the timer.
 * 
 * @param timer 
 * 
 * @return 1 if running, 0 otherwise
 */
int rflpc_timer_running(rflpc_timer_t timer);

/**
 * @brief Sets the cpu clock divider for a given timer.
 *
 * @param timer
 * @param divider @sa ::rflpc_clock_divider_t
 */
extern void rflpc_timer_set_clock(rflpc_timer_t timer, rflpc_clock_divider_t divider);

/**
 * @brief Sets the timer interrupt callback. Also enables timer interrupt to be received.
 *
 * @param timer
 * @param callback
 */
extern void rflpc_timer_set_callback(rflpc_timer_t timer, rflpc_irq_handler_t callback);


/**
 * @brief starts the given timer.
 *
 * This does not modify timer register value. It just starts to count
 *
 * @param timer
 */
extern void rflpc_timer_start(rflpc_timer_t timer);

/**
 * Stops the timer.
 *
 * Does not modify timer registers value
 * @param timer
 */
extern void rflpc_timer_stop(rflpc_timer_t timer);

/** Resets the timer
    @param timer
*/
extern void rflpc_timer_reset(rflpc_timer_t timer);

/**
 * Returns the timer counter value. This value is incremented each time the
 * prescale counter matches the prescale register.
 *
 * @param timer
 *
 * @return
 */
extern uint32_t rflpc_timer_get_counter(rflpc_timer_t timer);

/**
 * Returns the prescale counter value. This value is incremented at each clock
 * cycle.
 *
 * @param timer
 *
 * @return
 */
extern uint32_t rflpc_timer_get_pre_scale_counter(rflpc_timer_t timer);

/**
 * Returns the value of the prescale register. This register contains the
 * number of clock cycle needed to increment the timer counter. That is, at
 * each clock cycle, the prescale counter is incrementer and when it reaches
 * the prescale register, the timer counter is incremented.
 *
 * @param timer
 *
 * @return
 */
extern uint32_t rflpc_timer_get_pre_scale_register(rflpc_timer_t timer);

/**
 * Sets the timer counter value. This value is incremented each time the
 * prescale counter matches the prescale register.
 *
 * @param timer
 * @param value
 *
 */
extern void rflpc_timer_set_counter(rflpc_timer_t timer, uint32_t value);

/**
 * Sets the prescale counter value. This value is incremented at each clock
 * cycle.
 *
 * @param timer
 * @param value
 *
 */
extern void rflpc_timer_set_pre_scale_counter(rflpc_timer_t timer, uint32_t value);

/**
 * Sets the value of the prescale register. This register contains the
 * number of clock cycle needed to increment the timer counter. That is, at
 * each clock cycle, the prescale counter is incrementer and when it reaches
 * the prescale register, the timer counter is incremented.
 *
 * @param timer
 * @param value
 *
 */
extern void rflpc_timer_set_pre_scale_register(rflpc_timer_t timer, uint32_t value);

/**
 * Set the match register value for a given timer. Interrupt can be generated
 * when timer counter reaches a match register's value
 *
 * @param timer
 * @param match_register
 * @param match_value
 *
 * @return
 */
extern void rflpc_timer_set_match_value(rflpc_timer_t timer, rflpc_timer_match_t match_register, uint32_t match_value);

/**
 * Enable IRQ generation when the timer counter reaches the value of a match register
 *
 * @param timer
 * @param match_register
 * @param options a bitwise ORed value of ::RFLPC_TIMER_IRQ_ON_MATCH, ::RFLPC_TIMER_RESET_ON_MATCH and ::RFLPC_TIMER_STOP_ON_MATCH
 *
 * @return
 */
extern void rflpc_timer_set_irq_on_match(rflpc_timer_t timer, rflpc_timer_match_t match_register, uint32_t options);


/**
 * Reset the irq for a given match register
 *
 * @param timer
 * @param match_register
 */
extern void rflpc_timer_reset_irq(rflpc_timer_t timer, rflpc_timer_match_t match_register);

/**
 * Check if an interrupt has been generated for a given match register
 *
 * @param timer
 * @param match_register
 *
 * @return true if an interrupt is pending for the given match register
 */
extern int rflpc_timer_test_irq(rflpc_timer_t timer, rflpc_timer_match_t match_register);

/** @} */

#endif /* ENABLE_TIMERS */

#endif
