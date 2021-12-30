#pragma once

#include "quantum.h"
int osKey(int win, int mac);
int nextWord(void) { return osKey(C(KC_RGHT), A(KC_RGHT)); }
int lastWord(void) { return osKey(C(KC_LEFT), A(KC_LEFT)); }
int pasteKey(void) { return osKey(C(KC_V), G(KC_V)); }
int copyKey(void) { return osKey(C(KC_C), G(KC_C)); }
int cutKey(void) { return osKey(C(KC_X), G(KC_X)); }
int undoKey(void) { return osKey(C(KC_Z), G(KC_Z)); }
int refactorKey(void) { return osKey(C(KC_T), G(KC_T)); }
int selectNextKey(void) { return osKey(C(KC_G), G(KC_G)); }
int leftWorkspaceKey(void) { return osKey(G(C(KC_LEFT)), G(KC_LEFT)); }
int rightWorkspaceKey(void) { return osKey(G(C(KC_RGHT)), G(KC_RGHT)); }
