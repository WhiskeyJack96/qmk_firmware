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

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "test.h"
#include "quantum.h"
#include "process_key_override.h"

uint16_t copy_paste_timer;

enum layers {
    _QWERTY = 0,
    _NAV,
    _SYM,
};

enum custom_keycodes { KC_CCCV = SAFE_RANGE, KC_OS_WORK_LEFT, KC_OS_WORK_RIGHT, KC_TRACK_AUTOSHIFT };

// Aliases for readability
#define QWERTY DF(_QWERTY)

#define SYM MO(_SYM)
#define NAV MO(_NAV)

#define CTL_ESC MT(MOD_LCTL, KC_ESC)
#define SFT_SPC MT(MOD_LSFT, KC_SPC)
#define CTL_EQL MT(MOD_RCTL, KC_EQL)
#define SFT_QUOT MT(MOD_RSFT, KC_QUOTE)
#define CTL_SPC MT(MOD_LCTL, KC_SPC)

#define GUI_ESC MT(MOD_LGUI, KC_ESC)
#define GUI_QUOT MT(MOD_RGUI, KC_QUOTE)
#define GUI_ENT MT(MOD_RGUI, KC_ENT)

#define NAV_ENT LT(NAV, KC_ENT)
#define SYM_TAB LT(SYM, KC_TAB)

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPACE, KC_DELETE);
const key_override_t grv_key_override    = ko_make_basic(MOD_MASK_SHIFT, KC_TILD, KC_GRV);
const key_override_t curly_key_override  = ko_make_basic(MOD_MASK_CTRL, KC_TILD, KC_GRV);
// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override, &grv_key_override, NULL  // Null terminate the array of overrides!
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Bksp  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |      |  |      |      |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      | [ {  | LAlt/| Space| Nav  |  | Sym  | Space|      |  ] } |      |
 *                        |      |      | Enter|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
    MAGIC_TOGGLE_CTL_GUI, KC_Q, KC_W, KC_E, KC_R, KC_T,                      KC_Y,   KC_U,     KC_I,    KC_O,   KC_P,     KC_ASTG,
    KC_LSFT, KC_A,  KC_S, KC_D, KC_F, KC_G,                                  KC_H,   KC_J,     KC_K,    KC_L,   KC_SCLN, KC_QUOT,
    CTL_ESC, KC_Z,  KC_X, KC_C, KC_V, KC_B, KC_NO, KC_NO, KC_NO, KC_NO,    KC_N,   KC_M,     KC_COMM, KC_DOT, KC_SLSH, CTL_EQL,
                            KC_CCCV, KC_LBRC, KC_LALT, CTL_SPC, NAV_ENT,  SYM_TAB, SFT_SPC, KC_BSPC, KC_RBRC, KC_LEAD
    ),

/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                                     KC_HOME, C(KC_LEFT), KC_UP, C(KC_RGHT), _______, KC_MPLY,
    _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_END, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_INS,
    KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
                               _______, _______, _______, _______, _______, KC_OS_WORK_LEFT, KC_OS_WORK_RIGHT, _______, _______, _______
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
    KC_GRV,  KC_1,    KC_2,  KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
    KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR,  KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
    KC_PIPE, KC_BSLS, KC_LT, KC_GT,   KC_MINS, KC_LBRC, _______, _______, _______, _______, KC_RBRC, KC_UNDS, KC_COMM, KC_DOT,  KC_SLSH, KC_QUES,
                             _______, _______, _______, KC_LBRC, KC_RBRC, _______, SFT_SPC, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */
typedef union {
    uint32_t raw;
    struct {
        bool osIsWindows;
        bool asIsOn;
    };
} user_config_t;

user_config_t user_config;
void keyboard_post_init_user(void) {
    // Call the post init code.

    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_OS_WORK_LEFT:
            if (record->event.pressed) {
                tap_code16(leftWorkspaceKey());
                return false;
            }
            break;
        case KC_OS_WORK_RIGHT:
            if (record->event.pressed) {
                tap_code16(rightWorkspaceKey());
                return false;
            }
            break;
        // case C(KC_T):
        //     if (record->event.pressed) {
        //         tap_code16(refactorKey());
        //     }
        //     break;
        case G(KC_RGHT):
        case C(KC_RGHT):
            if (record->event.pressed) {
                tap_code16(nextWord());
                return false;
            }
            break;
        case G(KC_LEFT):
        case C(KC_LEFT):
            if (record->event.pressed) {
                tap_code16(lastWord());
                return false;
            }
            break;
        // case G(KC_C):    
        // case C(KC_C):ctrl_v_key_override
        //     if (record->event.pressed) {
        //         tap_code16(copyKey());
        //     }
        //     break;
        // case G(KC_V):
        // case C(KC_V):
        //     if (record->event.pressed) {
        //         tap_code16(pasteKey());
        //     }
        //     break;
        case KC_CCCV:  // One key copy/paste
            if (record->event.pressed) {
                copy_paste_timer = timer_read();
            } else {
                if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, copy
                    tap_code16(copyKey());
                } else { // Tap, paste
                    tap_code16(pasteKey());
                }
            }
            break;
        // case KC_TRACK_AUTOSHIFT:
        //     if (record->event.pressed) {
        //         user_config.asIsOn = !user_config.asIsOn;
        //         eeconfig_update_user(user_config.raw);
        //         tap_code16();
        //     }
    }
    return true;
}
int nextWord(void) {
    return osKey(C(KC_RGHT),A(KC_RGHT));
}
int lastWord(void) {
    return osKey(C(KC_LEFT),A(KC_LEFT));
}
int refactorKey(void) {
    return osKey(C(KC_T),G(KC_T));
}
int pasteKey(void) {
    return osKey(C(KC_V),G(KC_V));
}
int copyKey(void) {
    return osKey(C(KC_C),G(KC_C));
}
int leftWorkspaceKey(void) {
    return osKey(G(C(KC_LEFT)),G(KC_LEFT));
}
int rightWorkspaceKey(void) {
    return osKey(G(C(KC_RGHT)),G(KC_RGHT));
}

int osKey(int win,int mac) {
    if (user_config.osIsWindows == 1) {
        return win;
    }
    return mac;
}

LEADER_EXTERNS();
//
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();
    // Set current OS indicator to macOs
    SEQ_ONE_KEY(KC_M) {
        user_config.osIsWindows = false;
        eeconfig_update_user(user_config.raw);
    }
    // Set current OS indicator to Windows
    SEQ_ONE_KEY(KC_W) {
        user_config.osIsWindows = true;
        eeconfig_update_user(user_config.raw);
    }
    SEQ_ONE_KEY(KC_E) { // Email personal
        SEND_STRING("jacob.mikesell96@gmail.com");
    }
    SEQ_TWO_KEYS(KC_E,KC_E) { // Email personal
        SEND_STRING("jmikesell@apexclearing.com");
    }
    SEQ_TWO_KEYS(KC_D, KC_F) {  // Code block
        SEND_STRING("```" SS_LSFT("\n\n") "``` " SS_TAP(X_UP));
    }
    SEQ_ONE_KEY(KC_C) {  // Code block
        tap_code16(C(KC_C));
    }
    // Screenshot
    SEQ_ONE_KEY(KC_S) {
        if (user_config.osIsWindows == 1) {
            tap_code16(S(G(KC_S)));
        } else if (user_config.osIsWindows == 0) {
            tap_code16(S(G(KC_4)));
        }
    }
  }
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

void oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev1.5\n"), false);
        if (user_config.osIsWindows) {
            oled_write_P(PSTR("WANDOWS"), false);
        } else {
            oled_write_P(PSTR("MACARONI"), false);
        }

        oled_write_P(PSTR("\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM logo[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 
0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x00, 0x10, 0x10, 0x00, 0x08, 0x08, 0x08, 0x08, 0xff, 
0x9f, 0x08, 0x08, 0x08, 0x08, 0x00, 0x10, 0x10, 0x00, 0x20, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x18, 
0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x20, 0x40, 0xc0, 0x80, 0x00, 0x01, 0x01, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x01, 0x01, 0x01, 0x00, 0x80, 0xc0, 0x40, 0x20, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 
0x18, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x40, 0x40, 0x40, 0xc0, 0xc0, 0xe0, 0xa1, 0xa1, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x00, 
0x00, 0x03, 0x06, 0x0c, 0x18, 0x30, 0xa1, 0xe1, 0xe0, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x30, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x61, 0x21, 0x03, 0x03, 0x06, 0x1c, 0xf0, 0xc0, 
0xc0, 0xf0, 0x1c, 0x06, 0x03, 0x01, 0x31, 0x61, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 
0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x02, 0x00, 0x04, 0x1f, 0xff, 
0xff, 0x06, 0x04, 0x00, 0x02, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };
        // clang-format on
        oled_write_raw_P(logo, sizeof(logo));
    }
}
#endif

#ifdef ENCODER_ENABLE
// bool     is_alt_tab_active = false;
// uint16_t alt_tab_timer     = 0;

// void matrix_scan_user(void) {
//     if (is_alt_tab_active) {
//         if (timer_elapsed(alt_tab_timer) > 500) {
//             unregister_code(KC_LALT);
//             is_alt_tab_active = false;
//         }
//     }
// }

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (biton32(layer_state)) {
            case _QWERTY:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
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
    } else if (index == 1) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }

        // if (clockwise) {
        //     if (!is_alt_tab_active) {
        //         is_alt_tab_active = true;
        //         register_code(KC_LALT);
        //     }
        //     alt_tab_timer = timer_read();
        //     tap_code16(KC_TAB);
        // } else {
        //     if (!is_alt_tab_active) {
        //         is_alt_tab_active = true;
        //         register_code(KC_LALT);
        //     }
        //     alt_tab_timer = timer_read();
        //     tap_code16(S(KC_TAB));
        // }
    }
    return false;
}
#endif
