#pragma once

#include "quantum.h"

int osKey(int win, int mac);
int nextWord(void) {
    return osKey(C(KC_RGHT), A(KC_RGHT));
}
int lastWord(void) {
    return osKey(C(KC_LEFT), A(KC_LEFT));
}
int pasteKey(void) {
    return osKey(C(KC_V), G(KC_V));
}
int copyKey(void) {
    return osKey(C(KC_C), G(KC_C));
}
int cutKey(void) {
    return osKey(C(KC_X), G(KC_X));
}
int undoKey(void) {
    return osKey(C(KC_Z), G(KC_Z));
}
int redoKey(void) {
    return osKey(C(KC_Y), G(KC_Y));
}
int refactorKey(void) {
    return osKey(C(KC_T), G(KC_T));
}
int selectNextKey(void) {
    return osKey(C(KC_G), G(KC_G));
}
int leftWorkspaceKey(void) {
    return osKey(G(C(KC_LEFT)), G(KC_LEFT));
}
int rightWorkspaceKey(void) {
    return osKey(G(C(KC_RGHT)), G(KC_RGHT));
}
int nextWindow(void) {
    return osKey(A(KC_TAB), G(KC_TAB));
}
int lastWindow(void) {
    return osKey(S(A(KC_TAB)), S(G(KC_TAB)));
}

bool process_semantic_actions(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C(KC_LEFT):
            if (record->event.pressed) {
                tap_code16(lastWord());
                return false;
            }
            break;
        case C(KC_RIGHT):
            if (record->event.pressed) {
                tap_code16(nextWord());
                return false;
            }
            break;
        case C(KC_V):
            if (record->event.pressed) {
                tap_code16(pasteKey());
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
        case C(KC_Z):
            if (record->event.pressed) {
                tap_code16(undoKey());
                return false;
            }
            break;
        case C(KC_G):
            if (record->event.pressed) {
                tap_code16(selectNextKey());
                return false;
            }
            break;
        case C(KC_T):
            if (record->event.pressed) {
                tap_code16(refactorKey());
                return false;
            }
            break;
        case C(KC_Y):
            if (record->event.pressed) {
                tap_code16(redoKey());
                return false;
            }
            break;
        case G(C(KC_LEFT)):
            if (record->event.pressed) {
                tap_code16(leftWorkspaceKey());
                return false;
            }
            break;
        case G(C(KC_RGHT)):
            if (record->event.pressed) {
                tap_code16(rightWorkspaceKey());
                return false;
            }
            break;
    }
    return true;
}