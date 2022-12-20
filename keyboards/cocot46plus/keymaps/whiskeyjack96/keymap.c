/*
Copyright 2022 aki27

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef __INTELLISENSE__
#    include "../../config.h"
#    include "../../cocot46plus.h"
#    define QMK_KEYBOARD_H "cocot46plus.h"
#endif

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "./features/semantic_actions.h"
#include "./features/casemodes.h"

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

// Defines names for use in layer keycodes and the keymap
enum layer_number { _BASE = 0, _LOWER = 1, _RAISE = 2, _TRACKBALL = 3 };

#define BSPC_RAISE LT(_RAISE, KC_BSPC)
#define BSPC_TRACKBALL LT(_TRACKBALL, KC_BSPC)
#define SPC_SFT MT(MOD_LSFT, KC_SPC)
#define PLAY_CTL MT(MOD_LCTL, KC_MPLY)
#define GRV_GUI MT(MOD_LGUI, KC_GRV)
#define DEL_ALT ALT_T(KC_DEL)

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
    TAB_LOWER,
    ENT_CTL
    // SOME_OTHER_DANCE
};
td_state_t cur_dance(qk_tap_dance_state_t *state, bool interuptable);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);
const key_override_t volup_override = ko_make_basic(MOD_MASK_CTRL | MOD_MASK_GUI, KC_VOLU, KC_MNXT);
const key_override_t voldown_override = ko_make_basic(MOD_MASK_CTRL| MOD_MASK_GUI, KC_VOLD, KC_MPRV);

const key_override_t volup_sft_override = ko_make_basic(MOD_MASK_SHIFT, KC_VOLU, KC_MS_BTN5);
const key_override_t voldown_sft_override = ko_make_basic(MOD_MASK_SHIFT, KC_VOLD, KC_MS_BTN4);
// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &volup_override,
    &voldown_override,
    &volup_sft_override,
    &voldown_sft_override,
    NULL // Null terminate the array of overrides!
};
// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      PLAY_CTL,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LEAD,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      GRV_GUI,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LALT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_MINS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                    G(C(KC_LEFT)), KC_LALT, TD(TAB_LOWER), TD(ENT_CTL), KC_MS_BTN1,             KC_MS_BTN2,  SPC_SFT, BSPC_RAISE, KC_ESC,  G(C(KC_RIGHT)),
                                                                 KC_VOLU, KC_MS_BTN3,  KC_VOLD, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC, XXXXXXX,   XXXXXXX, XXXXXXX,  XXXXXXX,  C(KC_T),                                       KC_HOME, XXXXXXX, KC_UP, XXXXXXX, KC_END, KC_DEL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  C(KC_G),                                     XXXXXXX, KC_LEFT,    KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LALT,  C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V), XXXXXXX,                                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ENT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______,  _______,   KC_MS_BTN4,             KC_MS_BTN5,  _______,   BSPC_TRACKBALL,  KC_LALT,  _______,
                                                                 KC_MNXT, KC_MS_BTN3,  KC_MPRV, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),  
  [_RAISE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,   KC_0,  KC_DEL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,  KC_MINS,  KC_EQL,   KC_LBRC,  KC_RBRC,  KC_BSLS,                                        _______,   _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   _______,   _______,   KC_MINS,  KC_EQL,  _______,                                       _______, _______, _______,  _______, _______, KC_ENT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______,  _______,     TT(3), KC_LSFT,   KC_MS_BTN4,             KC_MS_BTN5,  _______, _______, _______,  _______,
                                                                 KC_PGUP, KC_MS_BTN3,  KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_TRACKBALL] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,                                       SCRL_TO,  CPI_SW, SCRL_SW, ROT_L15, ROT_R15, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD,                                       SCRL_MO, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, RGB_VAD, RGB_SAD, RGB_HUD,RGB_RMOD,                                       SCRL_IN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______,  _______,   KC_MS_BTN1,             KC_MS_BTN2,  _______, _______, _______,  _______,
                                                                 KC_PGUP, KC_MS_BTN3,  KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    )
};
// clang-format on

keyevent_t encoder1_ccw = {.key = (keypos_t){.row = 4, .col = 2}, .pressed = false};

keyevent_t encoder1_cw = {.key = (keypos_t){.row = 4, .col = 5}, .pressed = false};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time    = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time    = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    }

    return true;
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
        SEQ_THREE_KEYS(KC_S, KC_S, KC_C) { // SNAKE_CASE
            enable_caps_word();
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
    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time    = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time    = (timer_read() | 1);
        action_exec(encoder1_cw);
    }
}


report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
//set mouse mode true
    // int8_t degree = 45;
    // int8_t x_rev, y_rev;
    // x_rev =  + mouse_report.x * cos(degree) + mouse_report.y * sin(degree);
    // y_rev =  - mouse_report.x * sin(degree) + mouse_report.y * cos(degree);
    // if (isScrollMode) {
    //     mouse_report.h = x_rev;
    //     #ifdef COCOT_DRAGSCROLL_INVERT
    //             // Invert vertical scroll direction
    //             mouse_report.v = -y_rev;
    //     #else
    //             mouse_report.v = y_rev;
    //     #endif
    //     // mouse_report.v = y_rev;
    //     mouse_report.x = 0;
    //     mouse_report.y = 0;
    // } else {
    //     mouse_report.x = x_rev;
    //     mouse_report.y = y_rev;
    // }
    return mouse_report;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process case modes
    if (!process_case_modes(keycode, record)) {
        return false;
    }

    // OS independent actions
    return process_semantic_actions(keycode, record);
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
            layer_on(_LOWER);
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_ESC);
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
            layer_off(_LOWER);

            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_ESC);
            break;
        default:
            unregister_code(KC_TAB);
    }
    tab_tap_state.state = TD_NONE;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t ent_tap_state = {.is_press_action = true, .state = TD_NONE};

void ent_finished(qk_tap_dance_state_t *state, void *user_data) {
    ent_tap_state.state = cur_dance(state, false);
    switch (ent_tap_state.state) {
        case TD_SINGLE_TAP:
            register_code(KC_ENT);
            break;
        case TD_SINGLE_HOLD:
            register_code(osKey(KC_LCTL, KC_LGUI));
            break;
        default:
            register_code(KC_ENT);
    }
}

void ent_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (ent_tap_state.state) {
        case TD_SINGLE_TAP:
            unregister_code(KC_ENT);
            break;
        case TD_SINGLE_HOLD:
            unregister_code(osKey(KC_LCTL, KC_LGUI));
            break;
        default:
            unregister_code(KC_ENT);
    }
    ent_tap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {[TAB_LOWER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset), [ENT_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ent_finished, ent_reset)};

int hue_fst = -1;
int sat_fst = -1;
int val_fst = -1;

layer_state_t layer_state_set_user(layer_state_t state) {
    hue_fst = rgblight_get_hue();
    sat_fst = rgblight_get_sat();
    val_fst = rgblight_get_val();

    switch (get_highest_layer(state)) {
        case _LOWER:
            rgblight_sethsv_range(HSV_BLUE, 0, 2);
            cocot_set_scroll_mode(true);
            break;
        case _RAISE:
            rgblight_sethsv_range(HSV_RED, 0, 2);
            cocot_set_scroll_mode(true);
            break;
        case _TRACKBALL:
            rgblight_sethsv_range(HSV_GREEN, 0, 2);
            cocot_set_scroll_mode(false);
            break;
        default:
            // rgblight_sethsv_range( 0, 0, 0, 0, 2);
            rgblight_sethsv_range(HSV_TURQUOISE, 0, 2);
            cocot_set_scroll_mode(false);
            break;
    }
    rgblight_set_effect_range(2, 10);
    return state;
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_logo();
    oled_write_layer_state();
    return false;
}
#endif
