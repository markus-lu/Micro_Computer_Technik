#include "MainMenu.h"
#include "Menu.h"
#include "Events.h"
#include <stdio.h>
#include "lcdlib_1769.h"

static void handle_back(struct State *state);

static void handle_up(struct State *state);

static void handle_down(struct State *state);

static void handle_ok(struct State *state);

void main_menu_handle_keypress(struct State *state, uint8_t buttons) {
    bool should_redraw = state->menu_should_redraw;
    state->menu_should_redraw = true;
    switch (buttons) {
        case BUTTON_BACK:
            handle_back(state);
            break;
        case BUTTON_UP:
            handle_up(state);
            break;
        case BUTTON_DOWN:
            handle_down(state);
            break;
        case BUTTON_OK:
            handle_ok(state);
            break;
        default:
            state->menu_should_redraw = should_redraw;
            break;
    }
}

static void handle_back(struct State *state) {
    state->menu_should_redraw = false;
}

static void handle_down(struct State *state) {
    state->menu_screen = SCREEN_EVENTS_MENU;
}

static void handle_up(struct State *state) {
    state->menu_screen = SCREEN_EVENTS_MENU;
}

static void handle_ok(struct State *state) {
    state->menu_screen = SCREEN_EVENTS_MENU;
}

void main_menu_draw_menu(struct State *state) {
    uint16_t event_count = events_get_count(state->event_data);
    uint8_t temperature_whole = state->temperature >> 8;
    uint8_t temperature_fraction = state->temperature;
    struct DateTime *time = &state->time;

    lcd_clrscr();

    lcd_gotoxy(1, 1);
    lcd_write_string("Temperatur: ");
    lcd_write_char(temperature_whole / 10 + '0');
    lcd_write_char(temperature_whole % 10 + '0');
    lcd_write_char('.');
    switch (temperature_fraction) {
        case 0:
            lcd_write_string("00" "\xDF" "C");
            break;
        case 1:
            lcd_write_string("25" "\xDF" "C");
            break;
        case 2:
            lcd_write_string("50" "\xDF" "C");
            break;
        case 3:
            lcd_write_string("75" "\xDF" "C");
            break;
        default:
            break;
    }

    lcd_gotoxy(1, 2);
    lcd_write_string("Status:     ");
    lcd_write_string(state->rgb_state ? "An" : "Aus");

    lcd_gotoxy(1, 3);
    lcd_write_string("Events:     ");
    lcd_write_char(event_count / 10 + '0');
    lcd_write_char(event_count % 10 + '0');

}
