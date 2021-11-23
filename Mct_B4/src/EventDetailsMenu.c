#include "Menu.h"

#define SELECTED1(index, normal) (state->selected_event_detail == index && state->blink) ? '\xFF' : (normal)
#define SELECTED2(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF") : (normal)
#define SELECTED3(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF") : (normal)
#define SELECTED4(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF" "\xFF") : (normal)

static void handle_back(struct State *state) {
    state->screen = &EventsMenu;
}

static void handle_down(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (state->menu_edit_mode) {
        switch (state->selected_event_detail) {
            case 0:
                event->weekdays &= ~Monday;
                break;
            case 1:
                event->weekdays &= ~Tuesday;
                break;
            case 2:
                event->weekdays &= ~Wednesday;
                break;
            case 3:
                event->weekdays &= ~Thursday;
                break;
            case 4:
                event->weekdays &= ~Friday;
                break;
            case 5:
                event->weekdays &= ~Saturday;
                break;
            case 6:
                event->weekdays &= ~Sunday;
                break;
            case 7:
                if (event->hour > 0) {
                    event->hour--;
                } else {
                    event->hour = 23;
                }
                break;
            case 8:
                if (event->minute > 0) {
                    event->minute--;
                } else {
                    event->minute = 59;
                }
                break;
            case 9:
                event->on_or_off = false;
                break;
            case 10:
                event->enabled = false;
                break;
        }
    } else if (state->selected_event_detail < 10) {
        state->selected_event_detail++;
    }
}

static void handle_up(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (state->menu_edit_mode) {
        switch (state->selected_event_detail) {
            case 0:
                event->weekdays &= ~Monday;
                break;
            case 1:
                event->weekdays &= ~Tuesday;
                break;
            case 2:
                event->weekdays &= ~Wednesday;
                break;
            case 3:
                event->weekdays &= ~Thursday;
                break;
            case 4:
                event->weekdays &= ~Friday;
                break;
            case 5:
                event->weekdays &= ~Saturday;
                break;
            case 6:
                event->weekdays &= ~Sunday;
                break;
            case 7:
                if (event->hour < 23) {
                    event->hour++;
                } else {
                    event->hour = 0;
                }
                break;
            case 8:
                if (event->minute < 59) {
                    event->minute++;
                } else {
                    event->minute = 0;
                }
                break;
            case 9:
                event->on_or_off = true;
                break;
            case 10:
                event->enabled = true;
                break;
        }
    } else if (state->selected_event_detail > 0) {
        state->selected_event_detail--;
    }
}

static void handle_ok(struct State *state) {
    state->menu_edit_mode = !state->menu_edit_mode;
}

static void draw_menu(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    LCD.clear_screen();

    LCD.go_to_xy(1, 1);
    LCD.write_string("Tag: ");
    LCD.write_string(SELECTED2(0, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
    LCD.write_string(SELECTED2(1, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
    LCD.write_string(SELECTED2(2, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
    LCD.write_string(SELECTED2(3, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
    LCD.write_string(SELECTED2(4, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
    LCD.write_string(SELECTED2(5, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
    LCD.write_string(SELECTED2(6, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));

    LCD.go_to_xy(1, 2);
    LCD.write_string("Zeit: ");
    LCD.write_char(SELECTED1(7, event->hour / 10 + '0'));
    LCD.write_char(SELECTED1(7, event->hour % 10 + '0'));
    LCD.write_char(':');
    LCD.write_char(SELECTED1(8, event->minute / 10 + '0'));
    LCD.write_char(SELECTED1(8, event->minute % 10 + '0'));

    LCD.go_to_xy(1, 3);
    LCD.write_string("Schaltzustand: ");
    LCD.write_string(SELECTED3(9, event->on_or_off ? " an" : "aus"));

    LCD.go_to_xy(1, 4);
    LCD.write_string("Aktiv: ");
    LCD.write_string(SELECTED4(10, event->enabled ? "  ja" : "nein"));
}

const struct MenuScreen EventDetailsMenu = {
        .handle_back = handle_back,
        .handle_down = handle_down,
        .handle_up = handle_up,
        .handle_ok = handle_ok,
        .draw_menu = draw_menu,
};
