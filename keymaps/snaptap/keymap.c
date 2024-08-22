/* Copyright 2021 @ Keychron (https://www.keychron.com)
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
 * 
 * 
 * SnapTap Modifications By TORE
 */

#include QMK_KEYBOARD_H

bool snaptap_activated = false; // Estado del SnapTap

// Variables globales para manejar el estado de las teclas A y D
bool a_held = false;    // Estado real de la tecla 'A'
bool d_held = false;    // Estado real de la tecla 'D'
bool a_scrip = false;   // Estado simulado de la tecla 'A'
bool d_scrip = false;   // Estado simulado de la tecla 'D'

bool w_held = false;    // Estado real de la tecla 'W'
bool s_held = false;    // Estado real de la tecla 'S'
bool w_scrip = false;   // Estado simulado de la tecla 'W'
bool s_scrip = false;   // Estado simulado de la tecla 'S'

enum layers {
    MAC_BASE,
    WIN_BASE,
    _FN1,
    _FN2,
    _FN3
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_60_iso(
        KC_ESC,  KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,  KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,
        KC_CAPS, KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,
        KC_LSFT, KC_NUBS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            KC_RSFT,
        KC_LCTL, KC_LOPT,  KC_LCMD,                            KC_SPC,                             KC_RCMD,  MO(_FN1), MO(_FN3), KC_RCTL),

    [WIN_BASE] = LAYOUT_60_iso(
        KC_ESC,  KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,  KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,
        KC_CAPS, KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,
        KC_LSFT, KC_NUBS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            KC_RSFT,
        KC_LCTL, KC_LWIN,  KC_LALT,                            KC_SPC,                             KC_RALT,  MO(_FN2), MO(_FN3), KC_RCTL),

    [_FN1] = LAYOUT_60_iso(
        KC_GRV,  KC_BRID,  KC_BRIU, KC_NO,   KC_NO,   RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  RGB_MOD,
        RGB_TOG, RGB_MOD,  RGB_VAI, RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, _______, _______, KC_INS,   KC_PGUP,  KC_HOME,
        _______, RGB_RMOD, RGB_VAD, RGB_HUD, RGB_SAD, RGB_SPD, _______, _______, KC_UP,   _______, KC_PGDN,  KC_END,   _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, NK_TOGG, KC_LEFT, KC_DOWN, KC_RIGHT, KC_DEL,   _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  _______),

    [_FN2] = LAYOUT_60_iso(
        KC_GRV,  KC_BRID,  KC_BRIU, KC_TASK, KC_FLXP, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD,  KC_VOLU,  RGB_MOD,
        RGB_TOG, RGB_MOD,  RGB_VAI, RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, KC_APP,  KC_SCRL, KC_INS,   KC_PGUP,  KC_HOME,
        _______, RGB_RMOD, RGB_VAD, RGB_HUD, RGB_SAD, RGB_SPD, _______, _______, KC_UP,   KC_PSCR, KC_PGDN,  KC_END,   _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, NK_TOGG, KC_LEFT, KC_DOWN, KC_RIGHT, KC_DEL,             _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  _______),

    [_FN3] = LAYOUT_60_iso(
        KC_TILD, KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_F13,
        RGB_TOG, RGB_MOD,  RGB_VAI, RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, _______, _______, _______,  _______,  _______,
        _______, RGB_RMOD, RGB_VAD, RGB_HUD, RGB_SAD, RGB_SPD, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  _______)
};

// Lógica para manejar el comportamiento de las teclas 'A'/'D' y 'W'/'S'
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (snaptap_activated) {
		switch (keycode) {
			case KC_A:
				if (record->event.pressed) {
					a_held = true;
					if (d_scrip) {
						d_scrip = false;
						unregister_code(KC_D);  // Simula la liberación de la tecla 'D'
					}
					if (!a_scrip) {
						a_scrip = true;
						register_code(KC_A);   // Simula la pulsación de la tecla 'A'
					}
				} else {
					a_held = false;
					if (a_scrip) {
						a_scrip = false;
						unregister_code(KC_A); // Simula la liberación de la tecla 'A'
					}
					if (d_held && !d_scrip) {
						d_scrip = true;
						register_code(KC_D);   // Simula la pulsación de la tecla 'D'
					}
				}
				return false; // Evita la acción predeterminada de 'A'
			
			case KC_D:
				if (record->event.pressed) {
					d_held = true;
					if (a_scrip) {
						a_scrip = false;
						unregister_code(KC_A); // Simula la liberación de la tecla 'A'
					}
					if (!d_scrip) {
						d_scrip = true;
						register_code(KC_D);   // Simula la pulsación de la tecla 'D'
					}
				} else {
					d_held = false;
					if (d_scrip) {
						d_scrip = false;
						unregister_code(KC_D); // Simula la liberación de la tecla 'D'
					}
					if (a_held && !a_scrip) {
						a_scrip = true;
						register_code(KC_A);   // Simula la pulsación de la tecla 'A'
					}
				}
				return false; // Evita la acción predeterminada de 'D'
				
			case KC_W:
				if (record->event.pressed) {
					w_held = true;
					if (s_scrip) {
						s_scrip = false;
						unregister_code(KC_S);  // Simula la liberación de la tecla 'S'
					}
					if (!w_scrip) {
						w_scrip = true;
						register_code(KC_W);   // Simula la pulsación de la tecla 'W'
					}
				} else {
					w_held = false;
					if (w_scrip) {
						w_scrip = false;
						unregister_code(KC_W); // Simula la liberación de la tecla 'W'
					}
					if (s_held && !s_scrip) {
						s_scrip = true;
						register_code(KC_S);   // Simula la pulsación de la tecla 'S'
					}
				}
				return false; // Evita la acción predeterminada de 'W'

			case KC_S:
				if (record->event.pressed) {
					s_held = true;
					if (w_scrip) {
						w_scrip = false;
						unregister_code(KC_W); // Simula la liberación de la tecla 'W'
					}
					if (!s_scrip) {
						s_scrip = true;
						register_code(KC_S);   // Simula la pulsación de la tecla 'S'
					}
				} else {
					s_held = false;
					if (s_scrip) {
						s_scrip = false;
						unregister_code(KC_S); // Simula la liberación de la tecla 'S'
					}
					if (w_held && !w_scrip) {
						w_scrip = true;
						register_code(KC_W);   // Simula la pulsación de la tecla 'W'
					}
				}
				return false; // Evita la acción predeterminada de 'S'
				
			case KC_F13:
				if (record->event.pressed) {
					snaptap_activated = false; // Modifica el estado del SnapTap
				}
				return false;
				
			default:
				return true;  // Procesa todas las otras teclas normalmente
		}
	} else {
		switch (keycode) {
			case KC_F13:
				if (record->event.pressed) {
					snaptap_activated = true; // Modifica el estado del SnapTap
				}
				return false;
			default:
				return true;  // Procesa todas las otras teclas normalmente
		}
	}
}