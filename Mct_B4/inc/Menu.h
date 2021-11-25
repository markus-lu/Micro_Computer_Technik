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

#include "State.h"
#include "types.h"
#include "FrontIO.h"
#include "Events.h"
#include "LCD.h"

#define BUTTON_BACK (1 << 0)
#define BUTTON_DOWN (1 << 1)
#define BUTTON_UP (1 << 2)
#define BUTTON_OK (1 << 3)


struct menu {
    void (*init)();

    void (*loop_once)(struct State *state);
};

struct MenuScreen {
    void (*handle_back)(struct State *state);

    void (*handle_down)(struct State *state);

    void (*handle_up)(struct State *state);

    void (*handle_ok)(struct State *state);

    void (*draw_menu)(struct State *state);

    void (*update_menu)(struct State*state);
};

extern const struct menu Menu;
extern const struct MenuScreen MainMenu;
extern const struct MenuScreen EventsMenu;
extern const struct MenuScreen EventDetailsMenu;

#endif
