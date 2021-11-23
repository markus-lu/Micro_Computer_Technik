#include "Menu.h"

static void init() {
    FrontIO.init();
    LCD.init();
    LCD.backlight(0xFF);
}

static void check_keypress(struct State *state) {
    uint8_t buttons = FrontIO.get_buttons();
    FrontIO.set_leds(buttons);
    if (buttons != state->menu_last_buttons) {
        state->menu_should_redraw = true;
        switch (buttons) {
            case BUTTON_BACK:
                state->screen->handle_back(state);
                break;
            case BUTTON_UP:
                state->screen->handle_up(state);
                break;
            case BUTTON_DOWN:
                state->screen->handle_down(state);
                break;
            case BUTTON_OK:
                state->screen->handle_ok(state);
                break;
            default:
                state->menu_should_redraw = false;
                break;
        }
        state->menu_last_buttons = buttons;
    }
}

static void loop_once(struct State *state) {
    check_keypress(state);
    if (state->menu_should_redraw) {
        state->screen->draw_menu(state);
    }
}

const struct menu Menu = {
        .init = init,
        .loop_once = loop_once,
};
