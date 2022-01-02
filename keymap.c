#include QMK_KEYBOARD_H

// definitions
#include "keymap.h"

// combos
#include "g/keymap_combo.h"


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // base LAYOUT
  [COLEMAK_DH] = LAYOUT(
   KC_Q,   KC_W,   NAV_F,  KC_P,   KC_B,          KC_J,   KC_L,   KC_U,    KC_Y,   KC_QUOT,
   HOME_A, HOME_R, HOME_S, HOME_T, HOME_G,        HOME_M, HOME_N, HOME_E,  HOME_I, HOME_O,
   KC_Z,   KC_X,   KC_C,   KC_D,   KC_V,          KC_K,   KC_H,   KC_COMM, KC_DOT, KC_SLSH,

   THMB_SPC, THMB_ESC,    THMB_RET, REPEAT
  ),

  // numbers and symbols
  [SYM_NUM] = LAYOUT(
    ___, KC_AMPR, KC_ASTR, ___,     ___,        KC_MINS, KC_7, KC_8, KC_9, KC_SLSH,
    ___, KC_DLR,  KC_PERC, KC_CIRC, ___,        KC_PLUS, KC_4, KC_5, KC_6, KC_PIPE,
    ___, KC_TILD, KC_AT,   KC_HASH, ___,        ___,     KC_1, KC_2, KC_3, KC_BSLS,

    ___, ___,    KC_SPC, KC_0
  ),
  // function keys
  [FUNCTION] = LAYOUT(
    KC_F12, KC_F7, KC_F8, KC_F9, ___,     ___, ___, ___, ___, ___, 
    KC_F11, KC_F4, KC_F5, KC_F6, ___,     ___, ___, ___, ___, ___, 
    KC_F10, KC_F1, KC_F2, KC_F3, ___,     ___, ___, ___, ___, ___, 

    ___, ___,    ___, ___
  ),
  // surrounding stuff parentheses, brackets etc
  [SURROUND] = LAYOUT(
    ___, ___, ___, ___, ___,     ___, KC_LBRC, KC_RBRC, ___, ___, 
    ___, ___, ___, ___, ___,     ___, KC_LPRN, KC_RPRN, ___, ___, 
    ___, ___, ___, ___, ___,     ___, KC_LCBR, KC_RCBR, ___, ___, 

    ___, ___,    ___, ___
  ),
  // mouse keys
  [MOUSE] = LAYOUT(
    ___, ___, ___, ___, ___,        ___,     ___,     ___,     ___,     ___,
    ___, ___, ___, ___, ___,        KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_BTN3,
    ___, ___, ___, ___, ___,        KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, ___,

    ___, ___,    KC_BTN2, KC_BTN1
  ),
  // navigation (arrow keys etc)
  [NAVIGATION] = LAYOUT(
    ___, ___, ___, ___, ___,     ___,     KC_VOLD, KC_VOLU, ___,     ___,
    ___, ___, ___, ___, ___,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_CAPS,
    ___, ___, ___, ___, ___,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  ___,

    ___, ___, ___, KC_DEL
  ),
  // adjust layer, nothing as of now. will add later.
  [ADJUST] = LAYOUT(
    ___, ___, ___, ___, ___,     ___, ___, ___, ___, ___, 
    ___, ___, DT_DOWN, DT_UP, ___,     ___, ___, ___, ___, ___, 
    ___, ___, ___, ___, ___,     ___, ___, ___, ___, ___, 

    ___, DT_PRNT,    ___, ___
  ),
};


/** slight modification to precondition's code, so that
 * it works a bit better on my use case */
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;
bool processed_repeat_key = false;
bool pressed_repeat_key = false;
void process_repeat_key(uint16_t keycode, const keyrecord_t *record) {
    if (keycode != REPEAT) {
        // Early return when holding down a pure layer key
        // to retain modifiers
        switch (keycode) {
            case QK_DEF_LAYER ... QK_DEF_LAYER_MAX:
            case QK_MOMENTARY ... QK_MOMENTARY_MAX:
            case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
            case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
            case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
            case QK_TO ... QK_TO_MAX:
            case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
                return;
        }

        // last_modifier = oneshot_mod_state > mod_state ? oneshot_mod_state : mod_state;
        last_modifier = get_mods();

        switch (keycode) {
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
                if (record->event.pressed) {
                    last_keycode = GET_TAP_KC(keycode);
                }
                break;
            default:
                if (record->event.pressed) {
                    last_keycode = keycode;
                }
                break;
        }
    } else { // keycode == REPEAT
      if (record->event.pressed) {
        register_mods(last_modifier);
        register_code16(last_keycode);
        pressed_repeat_key = true;
      } else {
        unregister_code16(last_keycode);
        unregister_mods(last_modifier);
        pressed_repeat_key = false;
      }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE

    uprintf("keycode=%04X (%u, %u), pressed=%b, last_modifier=%02X, get_mods()=%02X\n",
        keycode,
        record->event.key.row,
        record->event.key.col,
        record->event.pressed,
        last_modifier,
        get_mods());

#endif

    switch (keycode) {

#ifdef SMART_REPEAT
      case REPEAT:
        if (get_mods() & MOD_MASK_CAG) {
          if (record->event.pressed) {
            register_code(KC_TAB);
          } else if (!pressed_repeat_key){
            unregister_code(KC_TAB);
          }
          if (pressed_repeat_key) break;
          return false;
        }
#endif // SMART REPEAT does some cool shit, but I think it's not worth it.

    }

    process_repeat_key(keycode, record);

    return true;
};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HOME_N:
    case HOME_T:
    // case HOME_E:
      return true;
  }
  return false;
}
