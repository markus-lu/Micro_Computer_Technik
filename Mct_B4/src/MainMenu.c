#include "Menu.h"

static void handle_back(struct State *state) {
    state->menu_should_redraw = false;
}

static void handle_down(struct State *state) {
    state->screen = &EventsMenu;
}

static void handle_up(struct State *state) {
    state->screen = &EventsMenu;
}

static void handle_ok(struct State *state) {
    state->screen = &EventsMenu;
}

static void draw_menu(struct State *state) {
    uint16_t event_count = Events.get_count(state->event_data);
    uint8_t temperature_whole = state->temperature >> 8;
    uint8_t temperature_fraction = state->temperature;
    struct DateTime *time = &state->time;

    LCD.clear_screen();

    LCD.go_to_xy(1, 1);
    LCD.write_string("Temperatur:  ");
    LCD.write_char(temperature_whole / 10 + '0');
    LCD.write_char(temperature_whole % 10 + '0');
    LCD.write_char('.');
    switch (temperature_fraction) {
        case 0:
            LCD.write_string("00" "\xDF" "C");
            break;
        case 1:
            LCD.write_string("25" "\xDF" "C");
            break;
        case 2:
            LCD.write_string("50" "\xDF" "C");
            break;
        case 3:
            LCD.write_string("75" "\xDF" "C");
            break;
        default:
        	break;
    }

    LCD.go_to_xy(1, 2);
    LCD.write_string("Status:      ");
    LCD.write_string(state->rgb_state ? "An" : "Aus");

    LCD.go_to_xy(1, 3);
    LCD.write_string("Events:      ");
    LCD.write_char(event_count / 10 + '0');
    LCD.write_char(event_count % 10 + '0');

//    LCD.go_to_xy(1, 4);
//    LCD.write_char(time->hours / 10 + '0');
//    LCD.write_char(time->hours % 10 + '0');
//    LCD.write_char(':');
//    LCD.write_char(time->minutes / 10 + '0');
//    LCD.write_char(time->minutes % 10 + '0');
//    LCD.write_char(':');
//    LCD.write_char(time->seconds / 10 + '0');
//    LCD.write_char(time->seconds % 10 + '0');
//    LCD.write_char(' ');
//    LCD.write_char(time->day / 10 + '0');
//    LCD.write_char(time->day % 10 + '0');
//    LCD.write_char(':');
//    LCD.write_char(time->month / 10 + '0');
//    LCD.write_char(time->month % 10 + '0');
//    LCD.write_char(':');
//    LCD.write_string(time->century ? "20" : "19");
//    LCD.write_char(time->year / 10 + '0');
//    LCD.write_char(time->year % 10 + '0');
}

const struct MenuScreen MainMenu = {
        .handle_back = handle_back,
        .handle_down = handle_down,
        .handle_up = handle_up,
        .handle_ok = handle_ok,
        .draw_menu = draw_menu,
};
