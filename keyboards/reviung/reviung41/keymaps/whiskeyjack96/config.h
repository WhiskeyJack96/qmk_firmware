/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define PERMISSIVE_HOLD
// Lets you roll mod-tap keys
#define IGNORE_MOD_TAP_INTERRUPT
#define LEADER_TIMEOUT 500

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define TAPPING_TOGGLE 2
// Configure the global tapping term (default: 200ms)
#define TAPPING_TERM 200
#define COMBO_COUNT 1
// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD

#ifdef ENCODER_ENABLE
#    define ENCODERS_PAD_A \
        { D5 }
#    define ENCODERS_PAD_B \
        { B7 }
#endif

#define ENCODER_DIRECTION_FLIP
