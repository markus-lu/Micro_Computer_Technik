/*
 ===============================================================================
 Name        : EventsMenu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des Menüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _EVENTS_MENU_H_
#define _EVENTS_MENU_H_

#include "types.h"
#include "State.h"

void events_menu_handle_keypress(struct State* state, uint8_t buttons);

void events_menu_draw_menu(struct State *state);

#endif // _EVENTS_MENU_H_
