#include QMK_KEYBOARD_H
#include "print.h"

enum custom_keycodes {
  ESC = SAFE_RANGE,
  LSFT,
};

bool caps_lock = false;
uint16_t caps_timer;
bool lctl_down = false;
bool lshift_down = false;
bool rshift_down = false;

void keyboard_post_init_user(void) {
  // debug_enable=true;
  // debug_matrix=true;
  // debug_keyboard=true;
  // debug_mouse=true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {

    // caps lock hold triggers layer 1
    case KC_LCAP:
      if(record->event.pressed){
        caps_timer = timer_read();
        if (lshift_down) unregister_code(KC_LSFT);
        layer_on(1);
      } else {
        if (timer_elapsed(caps_timer) < TAPPING_TERM) {
          caps_lock = !caps_lock;
          if (caps_lock) {
            rgblight_enable();
            register_code(KC_LCAP);
          } else {
            rgblight_disable();
            unregister_code(KC_LCAP);
          }
        }
        if (lshift_down) register_code(KC_LSFT);
        layer_off(1);
      }
      return false;
      break;

    case ESC:
      if (record->event.pressed) {
        if(lctl_down){
          SEND_STRING(SS_LCTL("`")); // for vscode workbench.action.togglePanel command
        } else {
          if(lshift_down || rshift_down){
            register_code(KC_GRV);
          } else {
            register_code(KC_ESC);
          }
        }
      } else {
        if(lshift_down || rshift_down){
          unregister_code(KC_GRV);
        } else {
          unregister_code(KC_ESC);
        }
      }
      return false;
      break;

    case KC_LCTL:
      if (record->event.pressed) {
        lctl_down = true;
      } else {
        lctl_down = false;
      }
      return true;
      break;

    case LSFT:
      // left shift is disabled on layer1 but status is kept. for vim like gg/G tap dance
      if (record->event.pressed) {
        if (!layer_state_is(1)) register_code(KC_LSFT);
        lshift_down = true;
      } else {
        if (!layer_state_is(1)) unregister_code(KC_LSFT);
        lshift_down = false;
      }
      return true;
      break;

    case KC_RSFT:
      if (record->event.pressed) {
        rshift_down = true;
      } else {
        rshift_down = false;
      }
      return true;
      break;

    default:
      return true;
  }
}

// support vim like gg (top of document) and G (end of document) commands on layer1
void dance_g(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1 && lshift_down) {
    tap_code16(LCTL(KC_PGDN));
    reset_tap_dance(state);
  } else if (state->count == 2 && !lshift_down) {
    tap_code16(LCTL(KC_PGUP));
    reset_tap_dance(state);
  } else if(state->count >= 3) {
    reset_tap_dance(state);
  }
}

enum {
  TD_G = 0
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_G] = ACTION_TAP_DANCE_FN(dance_g)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_64_ansi(
    ESC,      KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,             KC_BSPC,
    KC_TAB,             KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
    KC_LCAP,            KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
    LSFT,               KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_DEL,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_HAEN,  MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  [1] = LAYOUT_64_ansi(
    KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_DEL,
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,            _______,  _______,  KC_PGDN,  _______,  TD(TD_G), KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  KC_GRV,             _______,
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  _______,
    _______,  _______,  _______,                                _______,                                MO(2),    _______,  KC_HOME,  KC_PGDN,  KC_END
  ),
  [2] = LAYOUT_64_ansi(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,            RGB_TOG,  _______,  RGB_HUI,  RGB_HUD,  _______,  _______,  KC_PSCR,  KC_SLCK,  KC_PAUS,  KC_BRID,  KC_BRIU,  _______,  _______,
    _______,            RGB_MOD,  RGB_RMOD, RGB_SAI,  RGB_SAD,  _______,  _______,  KC_INS,   KC_HOME,  KC_PGUP,  _______,  _______,            _______,
    _______,            RGB_SPI,  RGB_SPD,  RGB_VAI,  RGB_VAD,  _______,  _______,  _______,  KC_END,   KC_PGDN,  _______,  _______,  _______,  RESET,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  DEBUG
  ),
};
