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


struct menu {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct menu Menu;

#endif
