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
#ifndef __SOUND_H__
#define __SOUND_H__


/** Configures the given pin to use it for sound generation.
    Must be called for each pin that will be used by other sound functions
*/
void sound_init_pin(rflpc_pin_t pin);

/** Plays a sound at a given frequency for a given time
    
    @param the pin to use as output pin
    @param frequency_by_100 the desired frequency multiplied by 1000 (this allows "precise" pitch)
    @param timems the length of the note in millisecond
 */
void sound_play(rflpc_pin_t pin, uint32_t frequency_by_1000, int timems);


/** Plays a note for a given time.

    @param note the note to play (uses MIDI encoding, 60 is middle C)
    @param timems the length of the note in millisecond
*/
void sound_play_note(rflpc_pin_t pin, uint8_t note, int timems);


#endif
