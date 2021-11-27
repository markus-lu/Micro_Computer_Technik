/*
 ===============================================================================
 Name        : Menu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des Menüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _MENU_H_
#define _MENU_H_

#include "types.h"
#include "State.h"

#define BUTTON_BACK (1 << 0)
#define BUTTON_DOWN (1 << 1)
#define BUTTON_UP (1 << 2)
#define BUTTON_OK (1 << 3)

#define SCREEN_MAIN_MENU 0
#define SCREEN_EVENTS_MENU 1
#define SCREEN_EVENT_DETAILS_MENU 2

void menu_init();

void menu_loop_once(struct State *state);

#endif
