#include "Menu.h"
#include "FrontIO.h"
#include "lcdlib_1769.h"
#include "MainMenu.h"
#include "EventsMenu.h"
#include "EventDetailsMenu.h"

void menu_init () {
    frontio_init();
    lcd_init(4, I2C_FM);
    lcd_bglight(0xFF);
}

void menu_check_keypress (struct State *state) {
    uint8_t buttons = frontio_get_buttons();
    frontio_set_leds(buttons);
    frontio_get_buttons();
    if (buttons != state->menu_last_buttons) {
        switch (state->menu_screen) {
            case SCREEN_MAIN_MENU:
                main_menu_handle_keypress(state, buttons);
                break;
            case SCREEN_EVENTS_MENU:
                events_menu_handle_keypress(state, buttons);
                break;
            case SCREEN_EVENT_DETAILS_MENU:
                event_details_menu_handle_keypress(state, buttons);
                break;
        }
        state->menu_last_buttons = buttons;
    }
}

void menu_loop_once (struct State *state) {
    menu_check_keypress(state);
    if (state->menu_should_redraw) {
        switch (state->menu_screen) {
            case SCREEN_MAIN_MENU:
                main_menu_draw_menu(state);
                break;
            case SCREEN_EVENTS_MENU:
                events_menu_draw_menu(state);
                break;
            case SCREEN_EVENT_DETAILS_MENU:
                event_details_menu_draw_menu(state);
                break;
        }
        state->menu_should_redraw = false;
    }
}
