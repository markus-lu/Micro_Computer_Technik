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
#define SCREEN_HOME 0
#define SCREEN_EVENTS 1
#define SCREEN_EVENT_DETAILS 2
#include "FrontIO.h"

struct MenuState{
	uint8_t screen;
	uint8_t sel_event;
	uint8_t sel_event_detail;
};

struct menu {
    void (*init)(void);
    void (*loop_once)(struct MenuState* state);
};

extern const struct menu Menu;

#endif
