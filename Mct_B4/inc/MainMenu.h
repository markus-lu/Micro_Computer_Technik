/*
 ===============================================================================
 Name        : MainMenu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des Menüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#include "types.h"
#include "State.h"

void main_menu_handle_keypress(struct State* state, uint8_t buttons);

void main_menu_draw_menu(struct State *state);

void main_menu_update_menu(struct State *state);

#endif // _MAIN_MENU_H_
