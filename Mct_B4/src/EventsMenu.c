#include "Menu.h"

static void handle_back(struct State *state) {
    state->screen = &MainMenu;
}

static void handle_down(struct State *state) {
    if (state->selected_event < EVENT_COUNT) {
        state->selected_event++;
    }
}

static void handle_up(struct State *state) {
    if (state->selected_event > 0) {
        state->selected_event--;
    }
}

static void handle_ok(struct State *state) {
    state->screen = &EventDetailsMenu;
}

static void write_event_line(struct State *state, int index) {
    struct Event *event = &state->event_data[index];

    LCD.write_char(state->selected_event == index ? '>' : ' ');
    LCD.write_char(event->enabled ? 'x' : 0xBD);
    LCD.write_char(' ');
    LCD.write_char(event->hour / 10 + '0');
    LCD.write_char(event->hour % 10 + '0');
    LCD.write_char(':');
    LCD.write_char(event->minute / 10 + '0');
    LCD.write_char(event->minute % 10 + '0');
    LCD.write_string(event->on_or_off ? "  An" : " Aus");
    LCD.write_char(((event->weekdays & Monday) != 0) ? 'M' : 'm');
    LCD.write_char(((event->weekdays & Tuesday) != 0) ? 'D' : 'd');
    LCD.write_char(((event->weekdays & Wednesday) != 0) ? 'M' : 'm');
    LCD.write_char(((event->weekdays & Thursday) != 0) ? 'D' : 'd');
    LCD.write_char(((event->weekdays & Friday) != 0) ? 'F' : 'f');
    LCD.write_char(((event->weekdays & Saturday) != 0) ? 'S' : 's');
    LCD.write_char(((event->weekdays & Sunday) != 0) ? 'S' : 's');
}

static void draw_menu(struct State *state) {
    // limit downwards scroll
    int start_index = state->selected_event + 4 < EVENT_COUNT ? state->selected_event : EVENT_COUNT - 4;

    LCD.clear_screen();

    for (int i = 0; i < 4; ++i) {
        LCD.go_to_xy(1, i + 1);
        write_event_line(state, start_index + i);
    }
}

const struct MenuScreen EventsMenu = {
        .handle_back = handle_back,
        .handle_down = handle_down,
        .handle_up = handle_up,
        .handle_ok = handle_ok,
        .draw_menu = draw_menu,
};
