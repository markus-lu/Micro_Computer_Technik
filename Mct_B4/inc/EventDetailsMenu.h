/*
 ===============================================================================
 Name        : EventDetailsMenu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des Menüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _EVENT_DETAILS_MENU_H_
#define _EVENT_DETAILS_MENU_H_

#include "types.h"
#include "State.h"

void event_details_menu_handle_keypress(struct State* state, uint8_t buttons);

void event_details_menu_draw_menu(struct State *state);

void event_details_menu_update_menu(struct State *state);

#endif // _EVENT_DETAILS_MENU_H_
