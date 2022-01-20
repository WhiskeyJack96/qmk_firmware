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
#include "features/semantic_actions.h"
#include "oneshot.h"
#include "quantum.h"
#include "oled_logos.h"
#include "features/casemodes.h"
#include "keymap_steno.h"

enum layers { _QWERTY = 0, _SYM, _NUM, _FUN, _NAV, _GAM, _GAM2, _STN };

enum custom_keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
};

// Aliases for readability
#define QWERTY DF(_QWERTY)

#define SYM MO(_SYM)
#define NAV MO(_NAV)
#define NUM MO(_NUM)
#define G2 MO(_GAM2)
#define FUN MO(_FUN)

const uint16_t PROGMEM ent_spc[]               = {KC_ESC, KC_SPC, COMBO_END};
const uint16_t PROGMEM num_sft[]               = {NUM, OSM(MOD_LSFT), COMBO_END};  // todo
combo_t                key_combos[COMBO_COUNT] = {
    COMBO(ent_spc, KC_ESC), COMBO(num_sft, NUM),  // keycodes with modifiers are possible too!
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    TG(_STN), KC_Q, KC_W, KC_E, KC_R, KC_T,                                                  KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MPLY,
    _______, KC_A, KC_S, KC_D, KC_F, KC_G,                                                   KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
    _______, KC_Z, KC_X, KC_C, KC_V, KC_B,         KC_ESC, _______,   KC_LEAD,       NUM,     KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, _______,
                        _______, _______, KC_ENT, KC_SPC, NAV, SYM, OS_SHFT, KC_BSPC, _______, _______
    ),              
    [_SYM] = LAYOUT(
    _______, KC_HASH, KC_EXLM, KC_ASTR, KC_AMPR, KC_PERC,                                     _______, _______, _______, _______, _______, _______,
    _______, KC_GRV,  KC_EQL,  KC_MINS, KC_PIPE, KC_CIRC,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, KC_SLSH, KC_RBRC, KC_RPRN, KC_RCBR, KC_EQL,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, KC_LBRC, KC_LCBR, KC_LPRN, _______, _______, _______, _______, _______
    ),
    [_NUM] = LAYOUT(
    _______, _______, KC_7, KC_8, KC_9, KC_BSLS,                                     _______, _______, _______, _______, _______, _______,
    _______, _______, KC_4, KC_5, KC_6, KC_MINS,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, _______, KC_1, KC_2, KC_3, KC_EQL,  _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
                               _______, _______, KC_DOT, KC_0, _______, _______, FUN, _______, _______, _______
    ),
    [_FUN] = LAYOUT(
    _______, _______, KC_F7, KC_F8, KC_F9, KC_F10,                                     _______, _______, _______, _______, _______, _______,
    _______, _______, KC_F4, KC_F5, KC_F6, KC_F11,                                    _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, _______, KC_F1, KC_F2, KC_F3, KC_F12, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
                               _______, _______, KC_F13, KC_F14, _______, _______, _______, _______, _______, _______
    ),
    [_NAV] = LAYOUT(
    _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, C(KC_T),                                     KC_HOME, C(KC_LEFT), KC_UP,   C(KC_RGHT), _______, _______,
    _______, OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, C(KC_G),                                     KC_END,  KC_LEFT,    KC_DOWN, KC_RGHT,    _______, _______,
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______, _______, _______, G(C(KC_LEFT)), _______, _______, _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, G(C(KC_RIGHT)), KC_TAB, KC_DEL, _______, _______
    ),
    [_GAM] = LAYOUT(
    _______, KC_T, KC_Q, KC_W, KC_E, KC_R,                                             KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MPLY,
    _______, KC_G, KC_A, KC_S, KC_D, KC_F,                                             KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
    _______, KC_B, KC_Z, KC_X, KC_C, KC_V,         KC_TAB, KC_LSFT, KC_LEAD,   _______,  KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_EQL,
                         _______, KC_ESC, KC_LCTL, KC_SPC, G2, _______, KC_ENT, KC_BSPC, KC_DEL, _______
    ),
    [_GAM2] = LAYOUT(
    _______, _______, KC_7, KC_8, KC_9, _______,                                     _______, _______, _______, _______, _______, _______,
    _______, _______, KC_4, KC_5, KC_6, _______,                                     _______, OS_CTRL, OS_SHFT, OS_ALT, OS_GUI, _______,
    _______, _______, KC_1, KC_2, KC_3, _______,  _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
                               _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_STN] = LAYOUT(
   STN_N1,  STN_N2,  STN_N3,  STN_N4,  STN_N5,   STN_N6,                                      STN_N7,  STN_N8,  STN_N9,  STN_NA,  STN_NB,  STN_NC,
   STN_FN,  STN_S1,  STN_TL,  STN_PL,  STN_HL,   STN_ST1,                                     STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR,  STN_DR,
   XXXXXXX, STN_S2,  STN_KL,  STN_WL,  STN_RL,   STN_ST2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR,  STN_ZR,
                              TG(_STN), STN_PWR, XXXXXXX, STN_A,   STN_O,   STN_E,   STN_U,     XXXXXXX, STN_RE1, STN_RE2
    )
};
// [_PLOVER] = LAYOUT_planck_grid(
//   STN_N1,  STN_N2,  STN_N3,  STN_N4,  STN_N5,  STN_N6,  STN_N7,  STN_N8,  STN_N9,  STN_NA,  STN_NB,  STN_NC ,
//   STN_FN,  STN_S1,  STN_TL,  STN_PL,  STN_HL,  STN_ST1, STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR,  STN_DR ,
//   XXXXXXX, STN_S2,  STN_KL,  STN_WL,  STN_RL,  STN_ST2, STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR,  STN_ZR ,
//   EXT_PLV, XXXXXXX, XXXXXXX, STN_A,   STN_O,   XXXXXXX, XXXXXXX, STN_E,   STN_U,   STN_PWR, STN_RE1, STN_RE2
// ),
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
    steno_set_mode(STENO_MODE_GEMINI); // or STENO_MODE_BOLT

}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case SYM:
    case NAV:
    case G2:
    case NUM:
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
    // Process case modes
    if (!process_case_modes(keycode, record)) {
        return false;
    }
    //Handle Callum Mods
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
    //OS independent actions
    return process_semantic_actions(keycode, record);
}

LEADER_EXTERNS();
bool     is_alt_tab_active = false;
uint16_t alt_tab_timer     = 0;
void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 500) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
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
    SEQ_TWO_KEYS(KC_E,KC_E) { // Email work
        SEND_STRING("jmikesell@apexclearing.com");
    }
    SEQ_TWO_KEYS(KC_C,KC_C) { // camelCase
        enable_xcase_with(OSM(MOD_LSFT));
    }
    SEQ_TWO_KEYS(KC_S,KC_C) { // snake_case
        enable_xcase_with(KC_UNDS);
    }
    SEQ_TWO_KEYS(KC_K,KC_C) { // kebab-case
        enable_xcase_with(KC_MINS);
    }
    SEQ_TWO_KEYS(KC_C,KC_W) { // CAPS word
        enable_caps_word();
    }
    // Screenshot
    SEQ_ONE_KEY(KC_S) {
        if (user_config.osIsWindows == 1) {
            tap_code16(S(G(KC_S)));
        } else if (user_config.osIsWindows == 0) {
            tap_code16(S(G(KC_4)));
        }
    }
    SEQ_ONE_KEY(KC_G) {
        layer_invert(_GAM);
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
        oled_write_P(PSTR("Kyria rev1.7\n"), false);
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
            case _GAM2:
                oled_write_P(PSTR("Game 2\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _FUN:
                oled_write_P(PSTR("Function\n"), false);
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
        if (IS_LAYER_OFF(_NAV)) {
            if (clockwise) {
                tap_code(KC_VOLD);
            } else {
                tap_code(KC_VOLU);
            }
        } else {
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(osKey(KC_LALT, KC_LGUI));
                }
                alt_tab_timer = timer_read();
                tap_code16(S(KC_TAB));
            } else {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(osKey(KC_LALT, KC_LGUI));
                }
                alt_tab_timer = timer_read();
                tap_code16(KC_TAB);
            }
        }
    }
    return false;
}
#endif