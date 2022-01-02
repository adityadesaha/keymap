#pragma once
#include QMK_KEYBOARD_H

enum layer_names {
  COLEMAK_DH, 
  SYM_NUM, 
  FUNCTION, 
  SURROUND, 
  NAVIGATION, 
  ADJUST, 
  MOUSE, 
};

// custom key symbols definition.
enum custom_keys {
  REPEAT = SAFE_RANGE,
};

// uint8_t oneshot_mod_state;
uint16_t last_keycode;

// helper function
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF

// thumb keys
#define THMB_SPC LT(SYM_NUM, KC_SPC)
#define THMB_ESC LT(MOUSE, KC_ESC)
#define THMB_RET LT(ADJUST, KC_ENT)

// navigation using the f key
#define NAV_F LT(NAVIGATION, KC_F)

// homerow mod keys definitions
#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LCTL_T(KC_S)
#define HOME_T LSFT_T(KC_T)
#define HOME_G LT(SURROUND, KC_G)

#define HOME_M LT(FUNCTION, KC_M)
#define HOME_N RSFT_T(KC_N)
#define HOME_E RCTL_T(KC_E)
#define HOME_I RALT_T(KC_I)
#define HOME_O RGUI_T(KC_O)

// there seems to be some issues with the wiring of my sweep, so the right hand is flipped
#undef LAYOUT
#define LAYOUT( \
        L01, L02, L03, L04, L05, R05, R04, R03, R02, R01, \
        L06, L07, L08, L09, L10, R10, R09, R08, R07, R06, \
        L11, L12, L13, L14, L15, R15, R14, R13, R12, R11, \
                  L16, L17, R17, R16     \
    ) \
    { \
        { L01, L02, L03, L04, L05 }, \
        { L06, L07, L08, L09, L10 }, \
        { L11, L12, L13, L14, L15 }, \
        { L16, L17, ___, ___ , ___}, \
        { R01, R02, R03, R04, R05 }, \
        { R06, R07, R08, R09, R10 }, \
        { R11, R12, R13, R14, R15 }, \
        { R16, R17, ___, ___, ___ }  \
    }

