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
#ifdef __INTELLISENSE__
#    include "../../rev1/config.h"
#    include "../../rev1/rev1.h"
#    define QMK_KEYBOARD_H "rev1.h"
#    define OLED_ENABLE true
#    define ENCODER_ENABLE true
#endif

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "semantic_actions.h"
#include "oneshot.h"
#include "quantum.h"
#include "oled_logos.h"

uint16_t copy_paste_timer;

enum layers { _QWERTY = 0, _SYM, _NUM, _NAV, _GAM, _GAM2 };

enum custom_keycodes {
    KC_CCCV = SAFE_RANGE,
    OS_LEFT,
    OS_RIGHT,
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
};

// Aliases for readability
#define QWERTY DF(_QWERTY)

#define SYM MO(_SYM)
#define NAV MO(_NAV)

#define G2_SPC LT(_GAM2, KC_SPC)

#define NUM_DEL LT(_NUM, KC_DEL)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    TG(_GAM), KC_Q, KC_W, KC_E, KC_R, KC_T,                                           KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MPLY,
    _______, KC_A, KC_S, KC_D, KC_F, KC_G,                                           KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
    _______, KC_Z, KC_X, KC_C, KC_V, KC_B,          KC_NO,  KC_NO, KC_NO, KC_NO,     KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, _______,
                           _______, KC_ESC, KC_ENT, KC_SPC, NAV, SYM, OSM(MOD_LSFT), KC_BSPC, NUM_DEL, KC_LEAD
    ),              
    [_SYM] = LAYOUT(
    _______, KC_HASH, KC_EXLM, KC_ASTR, KC_AMPR, KC_PERC,                                     _______, _______, _______, _______, _______, _______,
    _______, KC_GRV,  KC_EQL,  KC_MINS, KC_PIPE, KC_CIRC,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, KC_SLSH, KC_RCBR, KC_RPRN, KC_RBRC, KC_EQL,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, KC_LBRC, KC_LCBR, KC_LPRN, _______, _______, _______, _______, _______
    ),
    [_NUM] = LAYOUT(
    _______, _______, KC_7, KC_8, KC_9, KC_BSLS,                                     _______, _______, _______, _______, _______, _______,
    _______, _______, KC_4, KC_5, KC_6, KC_MINS,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, _______, KC_1, KC_2, KC_3, KC_EQL,  _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
                               _______, _______, KC_DOT, KC_0, _______, _______, _______, _______, _______, _______
    ),
    [_NAV] = LAYOUT(
    _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, C(KC_T),                                     KC_HOME, C(KC_LEFT), KC_UP,   C(KC_RGHT), _______, _______,
    _______, OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, C(KC_G),                                     KC_END,  KC_LEFT,    KC_DOWN, KC_RGHT, _______, _______,
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, OS_LEFT, KC_TAB, OS_RIGHT, _______, _______
    ),
    [_GAM] = LAYOUT(
    _______, KC_Q, KC_W, KC_E, KC_R, KC_T,                                           KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MPLY,
    KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G,                                           KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
    KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B,          KC_NO,  KC_NO, KC_NO, KC_NO,     KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_EQL,
                            _______, KC_ESC, KC_LCTL, G2_SPC, KC_TAB, KC_LEAD, KC_ENT, KC_BSPC, KC_DEL, _______
    ),
    [_GAM2] = LAYOUT(
    _______, _______, KC_7, KC_8, KC_9, _______,                                     _______, _______, _______, _______, _______, _______,
    _______, _______, KC_4, KC_5, KC_6, _______,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, _______, KC_1, KC_2, KC_3, _______,  _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

typedef union {
    uint32_t raw;
    struct {
        bool osIsWindows;
    };
} user_config_t;

user_config_t user_config;
void keyboard_post_init_user(void) {
    // Call the post init code.

    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();
}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case SYM:
    case NAV:
    case G2_SPC:
    case NUM_DEL:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case SYM:
    case NAV:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    if (!user_config.osIsWindows) {
        update_oneshot(
            &os_gui_state, KC_LGUI, OS_CTRL,
            keycode, record
        );
        update_oneshot(
            &os_ctrl_state, KC_LCTRL, OS_GUI,
            keycode, record
        );
    } else { 
        update_oneshot(
            &os_ctrl_state, KC_LCTL, OS_CTRL,
            keycode, record
        );
        update_oneshot(
            &os_gui_state, KC_LGUI, OS_GUI,
            keycode, record
        );
    }
    switch (keycode) {
        case OS_LEFT:
            if (record->event.pressed) {
                tap_code16(leftWorkspaceKey());
                return false;
            }
            break;
        case OS_RIGHT:
            if (record->event.pressed) {
                tap_code16(rightWorkspaceKey());
                return false;
            }
            break;
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
        case C(KC_Z):
            if (record->event.pressed) {
                tap_code16(undoKey());
                return false;
            }
            break;
        case C(KC_T):
            if (record->event.pressed) {
                tap_code16(refactorKey());
                return false;
            }
            break;
        case C(KC_G):
            if (record->event.pressed) {
                tap_code16(selectNextKey());
                return false;
            }
            break;
        case C(KC_X):
            if (record->event.pressed) {
                tap_code16(cutKey());
                return false;
            }
            break;
        case C(KC_C):
            if (record->event.pressed) {
                tap_code16(copyKey());
                return false;
            }
            break;
        case C(KC_V):
            if (record->event.pressed) {
                tap_code16(pasteKey());
                return false;
            }
            break;
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
    }
    return true;
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
int osKey(int win, int mac) {
    if (user_config.osIsWindows == 1) {
        return win;
    }
    return mac;
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
        oled_write_P(PSTR("Kyria rev1.6\n"), false);
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
                oled_write_P(PSTR("Base \n"), false);
                break;
            case _GAM:
                oled_write_P(PSTR("Game\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _NUM:
                oled_write_P(PSTR("Number\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Symbol\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        oled_write_P(os_ctrl_state != os_up_unqueued ? PSTR("CTRL ") : PSTR("     "), false);
        oled_write_P(os_gui_state != os_up_unqueued ? PSTR("GUI  ") : PSTR("     "), false);
        oled_write_P(os_alt_state != os_up_unqueued ? PSTR("ALT  ") : PSTR("     "), false);
        oled_write_P(os_shft_state != os_up_unqueued ? PSTR("SHFT ") : PSTR("     "), false);

    } else {
        oled_write_raw_P(right_logo, sizeof(right_logo));
    }
}
#endif

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (biton32(layer_state)) {
            case _QWERTY:
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
