/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{KC_NO}}};

uint8_t reset_state = 0;
uint8_t lock_state  = 0;

void keyboard_post_init_user(void) {
    reset_state = host_keyboard_led_state().caps_lock;
    lock_state  = host_keyboard_led_state().num_lock;
}

bool led_update_user(led_t led_state) {
    if (led_state.caps_lock != reset_state) {
        reset_keyboard();
    }
    if (led_state.num_lock != lock_state) {
        lock_state = led_state.num_lock;
    }

    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (lock_state) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}
