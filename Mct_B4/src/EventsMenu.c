#include <stdio.h>
#include "Menu.h"

static void handle_back(struct State *state) {
    state->screen = &MainMenu;
}

static void goto_event(uint8_t event) {
    uint8_t x = (event / 4) * 5;
    uint8_t y = event % 4;
    LCD.go_to_xy(x + 1, y + 1);
}

static void handle_down(struct State *state) {
    goto_event(state->selected_event);
    LCD.write_char(' ');
    if (state->selected_event > 0) {
        state->selected_event--;
    }
    goto_event(state->selected_event);
    LCD.write_char('>');
    state->menu_should_redraw = false;
}

static void handle_up(struct State *state) {
    goto_event(state->selected_event);
    LCD.write_char(' ');
    if (state->selected_event < EVENT_COUNT) {
        state->selected_event++;
    }
    goto_event(state->selected_event);
    LCD.write_char('>');
    state->menu_should_redraw = false;
}

static void handle_ok(struct State *state) {
    state->screen = &EventDetailsMenu;
}

static void write_event(struct State *state, char_t *string, int event) {
    char_t selected = state->selected_event == event ? '>' : ' ';
    char_t enabled = state->event_data[event].enabled ? 'x' : 0xDB;

    sprintf(string, "%c%c %02d", selected, enabled, event);
}

static void draw_menu(struct State *state) {
    LCD.clear_screen();
    char_t event[5][5];
    event[4][0] = 0;
    char_t *line = (char_t *) event;

    for (int i = 0; i < EVENT_COUNT / 4; ++i) {
        LCD.go_to_xy(1, i + 1);
        write_event(state, event[0], i);
        write_event(state, event[1], i + 4);
        write_event(state, event[2], i + 8);
        write_event(state, event[3], i + 12);
        LCD.write_string(line);
    }
}

static void update_menu(struct State *state) {
    // do nothing
}

const struct MenuScreen EventsMenu = {
        .handle_back = handle_back,
        .handle_down = handle_down,
        .handle_up = handle_up,
        .handle_ok = handle_ok,
        .draw_menu = draw_menu,
        .update_menu = update_menu,
};
