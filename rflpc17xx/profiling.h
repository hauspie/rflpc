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
 * Author: Michael Hauspie <michael.hauspie@univ-lille1.fr>
 * Created: 2011-09-29
 * Time-stamp: <2011-09-29 14:18:07 (hauspie)>
 */
#ifndef __RFLPC_PROFILING_H__
#define __RFLPC_PROFILING_H__

#ifdef RFLPC_ENABLE_PROFILING

#include "drivers/timer.h"

#ifndef RFLPC_PROFILE_PRESCALE
   #define RFLPC_PROFILE_PRESCALE (rflpc_clock_get_system_clock() / 1000000)
#endif

#define RFLPC_PROFILE_INIT(timer) do {rflpc_timer_enable(timer); rflpc_timer_set_clock(timer, RFLPC_CCLK); rflpc_timer_set_pre_scale_register(timer,RFLPC_PROFILE_PRESCALE); rflpc_timer_start(timer); } while(0)

#define RFLPC_PROFILE_DECLARE_COUNTER(counter) volatile uint32_t rflpc_profile_total_##counter; volatile uint32_t rflpc_profile_start_##counter;
#define RFLPC_PROFILE_DECLARE_EXTERN_COUNTER(counter) extern volatile uint32_t rflpc_profile_total_##counter;extern volatile uint32_t rflpc_profile_start_##counter;

#define RFLPC_PROFILE_START_COUNTER(counter,timer) do { rflpc_profile_start_##counter = rflpc_timer_get_counter(timer);} while(0)

#define RFLPC_PROFILE_STOP_COUNTER(counter,timer) do { rflpc_profile_total_##counter += rflpc_timer_get_counter(timer) - rflpc_profile_start_##counter;} while(0)

#define RFLPC_PROFILE_GET_TOTAL(counter) rflpc_profile_total_##counter

#endif
#endif