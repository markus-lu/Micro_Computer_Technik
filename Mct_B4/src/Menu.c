#include "Menu.h"

void init() {
    FrontIO.init();
}

bool handle_keypress() {
    // TODO: FrontIO.get_keys();
    return 0;
}

void draw_menu() {
    // TODO: Figure out how to draw a menu
}

void loop_once(struct MenuState *state) {
    bool menu_changed = handle_keypress();
    if (menu_changed) {
        draw_menu();
    }
}

const struct menu Menu = {
        .init = init,
        .loop_once = loop_once,
};
