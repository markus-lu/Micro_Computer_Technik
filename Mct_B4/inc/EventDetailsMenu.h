/*
 ===============================================================================
 Name        : EventDetailsMenu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des EventDetailsMenüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _EVENT_DETAILS_MENU_H_
#define _EVENT_DETAILS_MENU_H_

#include "types.h"
#include "State.h"

#define SELECTED_MONDAY 0
#define SELECTED_TUESDAY 1
#define SELECTED_WEDNESDAY 2
#define SELECTED_THURSDAY 3
#define SELECTED_FRIDAY 4
#define SELECTED_SATURDAY 5
#define SELECTED_SUNDAY 6
#define SELECTED_HOUR 7
#define SELECTED_MINUTE 8
#define SELECTED_ON_OR_OFF 9
#define SELECTED_ENABLED 10

void event_details_menu_handle_keypress(struct State* state, uint8_t buttons);

void event_details_menu_draw_menu(struct State *state);

void event_details_menu_update_menu(struct State *state);

#endif // _EVENT_DETAILS_MENU_H_
