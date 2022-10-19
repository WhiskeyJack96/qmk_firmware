/* Copyright 2020 gtips
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
#    include "../../reviung41.h"
#    define QMK_KEYBOARD_H "reviung.h"
#endif

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "./features/semantic_actions.h"
#include "./features/casemodes.h"

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST };

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define BSPC_RAISE LT(_RAISE, KC_BSPC)
#define SPC_SFT MT(MOD_LSFT, KC_SPC)
#define ENT_SFT MT(MOD_LSFT, KC_ENT)
#define PLAY_CTL MT(MOD_LCTL, KC_MPLY)
#define GRV_GUI MT(MOD_LGUI, KC_GRV)

#define ADJUST MO(_ADJUST)
// clang-format off
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    // TD_TRIPLE_TAP,
    // TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    TAB_LOWER
    // SOME_OTHER_DANCE
};
// const uint16_t PROGMEM lower_combo[] = {TD(TAB_LOWER), G(C(KC_LEFT)), COMBO_END};
// combo_t key_combos[COMBO_COUNT] = {
//     COMBO(lower_combo, LOWER),
// };
td_state_t cur_dance(qk_tap_dance_state_t *state, bool interuptable);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_reviung41(
    PLAY_CTL, KC_Q,     KC_W,     KC_E,     KC_R,      KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LEAD,
    GRV_GUI,  KC_A,     KC_S,     KC_D,     KC_F,      KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LALT,  KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_ESC,
                                G(C(KC_LEFT)), TD(TAB_LOWER),  SPC_SFT,    BSPC_RAISE,  G(C(KC_RIGHT)) 
  ),
  [_LOWER] = LAYOUT_reviung41(
    _______,  _______,  _______,  _______,  _______,  C(KC_T),           KC_HOME,  C(KC_LEFT), KC_UP,   C(KC_RGHT), KC_END,   KC_DEL,
    _______,  _______,  _______,  _______,  _______,  C(KC_G),           KC_MPRV,  KC_LEFT,    KC_DOWN, KC_RGHT,    KC_MNXT,  _______,
    _______,  C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V),  _______,           _______,  KC_VOLD,    _______, KC_VOLU,    _______,  KC_ENT,
                                            _______,  _______,  ENT_SFT, _______,  _______
  ),
  [_RAISE] = LAYOUT_reviung41(
    _______,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,             KC_6,     KC_7,    KC_8,     KC_9,    KC_0,     KC_DEL,
    _______,  KC_MINS,  KC_EQL,   KC_LBRC,  KC_RBRC,  KC_BSLS,          _______,  _______, _______,  _______, _______,  _______,
    _______,  _______,  _______,  KC_MINS,  KC_EQL,   _______,          _______,  _______, _______,  _______, _______,  KC_ENT,
                                            _______,   LOWER,  ENT_SFT, _______,  _______
  ),
  [_ADJUST] = LAYOUT_reviung41(
    XXXXXXX,   KC_F1,   KC_F2,    KC_F3,    KC_F4,     KC_F5,              KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
    RGB_VAI,   RGB_SAI, RGB_HUI,  RGB_MOD,  XXXXXXX,   RGB_TOG,            KC_F12,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CAPS,
    RGB_VAD,   RGB_SAD, RGB_HUD,  RGB_RMOD, XXXXXXX,   XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_NUM,
                                            _______,   _______,  XXXXXXX,  _______,  _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

typedef union {
    uint32_t raw;
    struct {
        bool osIsWindows;
    };
} user_config_t;

user_config_t user_config;
void          keyboard_post_init_user(void) {
    // Call the post init code.

    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process case modes
    if (!process_case_modes(keycode, record)) {
        return false;
    }

    // OS independent actions
    return process_semantic_actions(keycode, record);
}

LEADER_EXTERNS();
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
        SEQ_TWO_KEYS(KC_E, KC_E) { // Email work
            SEND_STRING("jmikesell@apexclearing.com");
        }
        SEQ_TWO_KEYS(KC_C, KC_C) { // camelCase
            enable_xcase_with(OSM(MOD_LSFT));
        }
        SEQ_TWO_KEYS(KC_S, KC_C) { // snake_case
            enable_xcase_with(KC_UNDS);
        }
        SEQ_TWO_KEYS(KC_K, KC_C) { // kebab-case
            enable_xcase_with(KC_MINS);
        }
        SEQ_TWO_KEYS(KC_C, KC_W) { // CAPS word
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
        SEQ_ONE_KEY(KC_C) {
            register_code(KC_LCTL);
            tap_code16(KC_C);
            unregister_code(KC_LCTL);
        }
        SEQ_ONE_KEY(KC_R) {
            register_code(KC_LCTL);
            tap_code16(KC_R);
            unregister_code(KC_LCTL);
        }
    }
}
int osKey(int win, int mac) {
    if (user_config.osIsWindows == 1) {
        return win;
    }
    return mac;
}

td_state_t cur_dance(qk_tap_dance_state_t *state, bool interuptable) {
    if (state->count == 1) {
        if ((interuptable && !state->interrupted) || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else
            return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (interuptable && state->interrupted)
            return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return TD_DOUBLE_HOLD;
        else
            return TD_DOUBLE_TAP;
    }
    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    // if (state->count == 3) {
    //     if (state->interrupted || !state->pressed)
    //         return TD_TRIPLE_TAP;
    //     else
    //         return TD_TRIPLE_HOLD;
    // } else
    return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t tab_tap_state = {.is_press_action = true, .state = TD_NONE};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    tab_tap_state.state = cur_dance(state, false);
    switch (tab_tap_state.state) {
        case TD_SINGLE_TAP:
            register_code(KC_TAB);
            break;
        case TD_SINGLE_HOLD:
            register_code(osKey(KC_LCTL, KC_LGUI));
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_ESC);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(_LOWER);
            break;
        default:
            register_code(KC_TAB);
    }
}

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (tab_tap_state.state) {
        case TD_SINGLE_TAP:
            unregister_code(KC_TAB);
            break;
        case TD_SINGLE_HOLD:
            unregister_code(osKey(KC_LCTL, KC_LGUI));
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_ESC);
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_LOWER);
        default:
            unregister_code(KC_TAB);
    }
    tab_tap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {[TAB_LOWER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)};
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (biton32(layer_state)) {
        case _RAISE:
            // History scrubbing. For Adobe products, hold shift while moving
            // backward to go forward instead.
            if (clockwise) {
                tap_code16(undoKey());
            } else {
                tap_code16(redoKey());
            }
            break;
        default:
            if ((get_mods() & MOD_BIT(KC_LCTL)) || ((get_mods() & MOD_MASK_GUI) == MOD_MASK_GUI)) {
                if (clockwise) {
                    tap_code_delay(KC_MNXT, 300);
                } else {
                    tap_code_delay(KC_MPRV, 300);
                }
            } else {
                if (clockwise) {
                    tap_code_delay(KC_VOLU, 10);
                } else {
                    tap_code_delay(KC_VOLD, 10);
                }
            }
            break;
    }

    return false;
}
#endif