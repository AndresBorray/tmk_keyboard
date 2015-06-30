#include "keymap_common.h"

/*const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP( ESC,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    BSPC, \
            TAB,  A,    S,    D,    F,    G,    H,    J,    K,    L,    ENT, \
            LSFT, Z,    X,    C,    V,    B,    N,    M,    COMM, RSFT, CAPS, \
            LCTL, DEL,  LALT, LALT, LALT, SPC,  RALT, RALT, DOT,  SLSH ),
};*/

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = KEYMAP(
  FN29, FN30, FN31, 4,    5,    6,    7,    8,    9,    0,    MINS, EQL,
  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC,
  A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
  Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH ),
[1] = KEYMAP(
  FN29, FN30, FN31, 4,    5,    6,    7,    8,    9,    0,    MINS, EQL,
  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC,
  A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
  Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH ),
[2] = KEYMAP(
  FN29, FN30, FN31, 4,    5,    6,    7,    8,    9,    0,    MINS, EQL,
  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC,
  A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT,
  Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH ),
};



const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_MOMENTARY(1),  // LOWER
    [2] = ACTION_LAYER_MOMENTARY(2),  // RAISE

    [10] = ACTION_MODS_KEY(MOD_LSFT, KC_1), // !
    [11] = ACTION_MODS_KEY(MOD_LSFT, KC_2), // @
    [12] = ACTION_MODS_KEY(MOD_LSFT, KC_3), // #
    [13] = ACTION_MODS_KEY(MOD_LSFT, KC_4), // $
    [14] = ACTION_MODS_KEY(MOD_LSFT, KC_5), // %
    [15] = ACTION_MODS_KEY(MOD_LSFT, KC_6), // ^
    [16] = ACTION_MODS_KEY(MOD_LSFT, KC_7), // &
    [17] = ACTION_MODS_KEY(MOD_LSFT, KC_8), // *
    [18] = ACTION_MODS_KEY(MOD_LSFT, KC_9), // (
    [19] = ACTION_MODS_KEY(MOD_LSFT, KC_0), // )
    
    [20] = ACTION_MODS_KEY(MOD_LSFT, KC_MINS), // _
    [21] = ACTION_MODS_KEY(MOD_LSFT, KC_EQL), // +
    [22] = ACTION_MODS_KEY(MOD_LSFT, KC_GRV), // ~
    [23] = ACTION_MODS_KEY(MOD_LSFT, KC_LBRC), // {
    [24] = ACTION_MODS_KEY(MOD_LSFT, KC_RBRC), // }
    [25] = ACTION_MODS_KEY(MOD_LSFT, KC_BSLS), // |

    [26] = ACTION_MODS_KEY(MOD_LSFT | MOD_RSFT, KC_PAUSE),
    
    
    [29] = ACTION_BACKLIGHT_TOGGLE(),
    [30] = ACTION_BACKLIGHT_INCREASE(),
    [31] = ACTION_BACKLIGHT_DECREASE()
    
};