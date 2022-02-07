/* Copyright 2020 KemoNine
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
#ifdef __INTELLISENSE__
#    include "../../config.h"
#    include "../../lynepad.h"
#    define QMK_KEYBOARD_H "lynepad.h"
#endif
#include QMK_KEYBOARD_H
// clang-format off
enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap (Base Layer) Default Layer
   * |----------------------------|
   * | 1 | 2  | 3  | 4 |          |
   * | 5 | 6  | 7  | 8 |          |
   * | 9 | 10 | 11 |              |
   * |----------------------------|
   */
[_BASE] = LAYOUT(
   _______, KC_UP,   _______,  TG(_RAISE),
   KC_LEFT, KC_DOWN, KC_RIGHT,  TG(_ADJUST),
   _______, _______, _______
  ),
[_LOWER] = LAYOUT(
   KC_Q,     KC_W,     KC_E,  TG(_RAISE),
   KC_A,     KC_S,     KC_D,  TG(_BASE),
   KC_Z,     KC_X,     KC_SPACE
  ),
[_RAISE] = LAYOUT(
   _______, KC_UP,   _______,  TG(_ADJUST),
   KC_LEFT, KC_DOWN, KC_RIGHT,  TG(_LOWER),
   _______, _______, _______
  ),
[_ADJUST] = LAYOUT(
   RGB_VAI, RGB_SAI,  RGB_HUI,  TG(_BASE),
   RGB_VAD, RGB_SAD,  RGB_HUD,  TG(_RAISE),
   RGB_MOD, RGB_RMOD, _______
  ),
};
// clang-format on

// Standard encoder functionality
bool encoder_update_user(uint8_t index, bool clockwise) {
    // Process encoder rotational movements
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    } else if (index == 1) { /* Second encoder */
        switch (biton32(layer_state)) {
            case _BASE:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (!clockwise) {
                    tap_code16(C(KC_Z));
                } else {
                    tap_code16(C(KC_Y));
                }
                break;
            default:
                // Scroll up/Scroll down
                if (clockwise) {
                    tap_code(KC_WH_U);
                } else {
                    tap_code(KC_WH_D);
                }
                break;
        }
    }
    return true;
}

// Encoder press / tilt event handling
// the core lynepad implementation will update the below variables on each matrix scan
// Update the various codes below for customizing the tilt / push config

extern int16_t enc1Center;
extern int16_t enc1CenterPrev;
extern int16_t enc2Center;
extern int16_t enc2CenterPrev;
extern int16_t enc2Up;
extern int16_t enc2UpPrev;
extern int16_t enc2Down;
extern int16_t enc2DownPrev;
extern int16_t enc2Left;
extern int16_t enc2LeftPrev;
extern int16_t enc2Right;
extern int16_t enc2RightPrev;

void matrix_scan_user(void) {
    if (enc1Center != enc1CenterPrev) {
        if (enc1Center < ENC_TILT_THRESHOLD) {
        } else {
            reset_keyboard();
        }
    }
    if (enc2Center != enc2CenterPrev) {
        if (enc2Center < ENC_TILT_THRESHOLD) {
            // register_code16(KC_MS_BTN3);
        } else {
            // unregister_code16(KC_MS_BTN3);
        }
        /*
         * Encoder sets ALL values when center is pressed so bail out at this point\
         * to avoid the rest of the encoder buttons registering events
         */
        return;
    }
    if (enc2Up != enc2UpPrev) {
        if (enc2Up < ENC_TILT_THRESHOLD) {
        } else {
            // tap_code(KC_AUDIO_VOL_UP);
        }
    }
    if (enc2Down != enc2DownPrev) {
        if (enc2Down < ENC_TILT_THRESHOLD) {
        } else {
            // tap_code(KC_AUDIO_VOL_DOWN);
        }
    }
    if (enc2Left != enc2LeftPrev) {
        if (enc2Left < ENC_TILT_THRESHOLD) {
        } else {
            // rgblight_toggle_noeeprom();
        }
    }
    if (enc2Right != enc2RightPrev) {
        if (enc2Right < ENC_TILT_THRESHOLD) {
        } else {
            // rgblight_step_noeeprom();
        }
    }
}
