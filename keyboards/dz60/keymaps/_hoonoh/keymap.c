#include QMK_KEYBOARD_H

enum custom_keycodes {
  ESC = SAFE_RANGE
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t caps_timer;
  static bool lctl_down = false;
  static bool shift_down = false;

  switch (keycode) {
    // caps lock hold triggers layer 1
    case KC_CAPS:
      if(record->event.pressed){
        caps_timer = timer_read();
        layer_on(1);
      } else {
        if (timer_elapsed(caps_timer) < TAPPING_TERM) {
          tap_code(KC_CAPS);
        }
        layer_off(1);
      }
      return false;

    case ESC:
      if (record->event.pressed) {
        if(lctl_down){
          SEND_STRING(SS_LCTL("`")); // for vscode workbench.action.togglePanel command
        } else {
          if(shift_down){
            register_code(KC_GRV);
          } else {
            register_code(KC_ESC);
          }
        }
      } else {
        if(shift_down){
          unregister_code(KC_GRV);
        } else {
          unregister_code(KC_ESC);
        }
      }
      return false;

    case KC_LCTL:
      if (record->event.pressed) {
        lctl_down = true;
      } else {
        lctl_down = false;
      }
      return true;

    case KC_LSFT:
    case KC_RSFT:
      if (record->event.pressed) {
        shift_down = true;
      } else {
        shift_down = false;
      }
      return true;
    default:
      return true;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_64_ansi(
    ESC,      KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,             KC_BSPC,
    KC_TAB,             KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
    KC_CAPS,            KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
    KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_DEL,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_HAEN,  MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  [1] = LAYOUT_64_ansi(
    KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_DEL,
    KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
    KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
    MO(2),              KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_PGUP,  KC_TRNS,
    KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_HOME,  KC_PGDN,  KC_END
  ),
  [2] = LAYOUT_64_ansi(
    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
    KC_TRNS,            RGB_TOG,  KC_TRNS,  RGB_HUI,  RGB_HUD,  KC_TRNS,  KC_TRNS,  KC_PSCR,  KC_SLCK,  KC_PAUS,  KC_BRID,  KC_BRIU,  KC_TRNS,  KC_TRNS,
    KC_TRNS,            RGB_MOD,  RGB_RMOD, RGB_SAI,  RGB_SAD,  KC_TRNS,  KC_TRNS,  KC_INS,   KC_HOME,  KC_PGUP,  KC_TRNS,  KC_GRV,             KC_TRNS,
    KC_TRNS,            RGB_SPI,  RGB_SPD,  RGB_VAI,  RGB_VAD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_END,   KC_PGDN,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RESET,
    KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS
  )
};
