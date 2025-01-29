#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┐
     * │Tab│ Q │ W │ E │ R │ T │
     * ├───┼───┼───┼───┼───┼───┤
     * │Bsp│ A │ S │ D │ F │ G │
     * ├───┼───┼───┼───┼───┼───┤
     * │Sft│ Z │ X │ C │ V │ B │
     * ├───┼───┼───┼───┼───┼───┤
     * │Ctl│App│GUI│Alt│Spc│Spc│
     * └───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(
                 KC_Q,    KC_W,    KC_E,
        KC_NUM,    KC_PSLS,    KC_PAST, KC_PMNS,
        KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_P4, KC_P5,    KC_P6,    KC_PPLS,
        KC_P1,    KC_P2,    KC_P3, KC_PENT,
        KC_P0, KC_PDOT, KC_P0,  KC_P0
    )
};
