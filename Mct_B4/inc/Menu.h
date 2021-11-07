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
#include "FrontIO.h"

struct MenuState{

};

struct menu {
    void (*init)(void);
    void (*loop_once)(struct MenuState* state);
};

extern const struct menu Menu;

#endif
