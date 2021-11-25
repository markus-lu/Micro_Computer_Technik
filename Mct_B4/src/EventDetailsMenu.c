#include "Menu.h"

#define SELECTED_MONDAY 0
#define SELECTED_TUESDAY 1
#define SELECTED_WEDNESDAY 2
#define SELECTED_THURSDAY 3
#define SELECTED_FRIDAY 4
#define SELECTED_SATURDAY 5
#define SELECTED_SUNDAY 6
#define SELECTED_HOUR 7
#define SELECTED_MINUTE 8
#define SELECTED_ON_OR_OFF 9
#define SELECTED_ENABLED 10

#define SELECTED1(index, normal) (state->selected_event_detail == index && state->blink) ? '\xFF' : (normal)
#define SELECTED2(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF") : (normal)
#define SELECTED3(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF") : (normal)
#define SELECTED4(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF" "\xFF") : (normal)

static void handle_back(struct State *state) {
    state->screen = &EventsMenu;
    state->selected_event_detail = SELECTED_MONDAY;
    state->menu_edit_mode = false;
}

static void handle_down(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (!state->menu_edit_mode) {
        state->blink = false;
        EventDetailsMenu.update_menu(state);
        state->menu_should_redraw = false;
    }
    if (state->menu_edit_mode) {
        switch (state->selected_event_detail) {
            case SELECTED_HOUR:
                if (event->hour > 0) {
                    event->hour--;
                } else {
                    event->hour = 23;
                }
                LCD.go_to_xy(7, 2);
                LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
                LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
                state->menu_should_redraw = false;
                break;
            case SELECTED_MINUTE:
                if (event->minute > 0) {
                    event->minute--;
                } else {
                    event->minute = 59;
                }
                LCD.go_to_xy(10, 2);
                LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
                LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
                state->menu_should_redraw = false;
                break;
        }
    } else if (state->selected_event_detail > SELECTED_MONDAY) {
        state->selected_event_detail--;
    } else {
        state->selected_event_detail = SELECTED_ENABLED;
    }
}

static void handle_up(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (!state->menu_edit_mode) {
        state->blink = false;
        EventDetailsMenu.update_menu(state);
        state->menu_should_redraw = false;
    }
    if (state->menu_edit_mode) {
        switch (state->selected_event_detail) {
            case SELECTED_HOUR:
                if (event->hour < 23) {
                    event->hour++;
                } else {
                    event->hour = 0;
                }
                LCD.go_to_xy(7, 2);
                LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
                LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
                state->menu_should_redraw = false;
                break;
            case SELECTED_MINUTE:
                if (event->minute < 59) {
                    event->minute++;
                } else {
                    event->minute = 0;
                }
                LCD.go_to_xy(10, 2);
                LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
                LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
                state->menu_should_redraw = false;
                break;
        }
    } else if (state->selected_event_detail < SELECTED_ENABLED) {
        state->selected_event_detail++;
    } else {
        state->selected_event_detail = SELECTED_MONDAY;
    }
}

static void handle_ok(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    switch (state->selected_event_detail) {
        case SELECTED_MONDAY:
            event->weekdays ^= Monday;
            break;
        case SELECTED_TUESDAY:
            event->weekdays ^= Tuesday;
            break;
        case SELECTED_WEDNESDAY:
            event->weekdays ^= Wednesday;
            break;
        case SELECTED_THURSDAY:
            event->weekdays ^= Thursday;
            break;
        case SELECTED_FRIDAY:
            event->weekdays ^= Friday;
            break;
        case SELECTED_SATURDAY:
            event->weekdays ^= Saturday;
            break;
        case SELECTED_SUNDAY:
            event->weekdays ^= Sunday;
            break;
        case SELECTED_HOUR:
        case SELECTED_MINUTE:
            state->menu_edit_mode = !state->menu_edit_mode;
            break;
        case SELECTED_ON_OR_OFF:
            event->on_or_off = !event->on_or_off;
            break;
        case SELECTED_ENABLED:
            event->enabled = !event->enabled;
            break;
    }

    EventDetailsMenu.update_menu(state);
    state->menu_should_redraw = false;
}

static void draw_menu(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    LCD.clear_screen();

    LCD.go_to_xy(1, 1);
    LCD.write_string("Tag: ");
    LCD.write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
    LCD.write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
    LCD.write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
    LCD.write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
    LCD.write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
    LCD.write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
    LCD.write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));

    LCD.go_to_xy(1, 2);
    LCD.write_string("Zeit: ");
    LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
    LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
    LCD.write_char(':');
    LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
    LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));

    LCD.go_to_xy(1, 3);
    LCD.write_string("Schaltzustand: ");
    LCD.write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));

    LCD.go_to_xy(1, 4);
    LCD.write_string("Aktiv: ");
    LCD.write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
}

static void update_menu(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    switch (state->selected_event_detail) {
        case SELECTED_MONDAY:
            LCD.go_to_xy(6, 1);
            LCD.write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
            break;
        case SELECTED_TUESDAY:
            LCD.go_to_xy(8, 1);
            LCD.write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
            break;
        case SELECTED_WEDNESDAY:
            LCD.go_to_xy(10, 1);
            LCD.write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
            break;
        case SELECTED_THURSDAY:
            LCD.go_to_xy(12, 1);
            LCD.write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
            break;
        case SELECTED_FRIDAY:
            LCD.go_to_xy(14, 1);
            LCD.write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
            break;
        case SELECTED_SATURDAY:
            LCD.go_to_xy(16, 1);
            LCD.write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
            break;
        case SELECTED_SUNDAY:
            LCD.go_to_xy(18, 1);
            LCD.write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));
            break;
        case SELECTED_HOUR:
            LCD.go_to_xy(7, 2);
            LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
            LCD.write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
            break;
        case SELECTED_MINUTE:
            LCD.go_to_xy(10, 2);
            LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
            LCD.write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
            break;
        case SELECTED_ON_OR_OFF:
            LCD.go_to_xy(16, 3);
            LCD.write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));
            break;
        case SELECTED_ENABLED:
            LCD.go_to_xy(8, 4);
            LCD.write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
            break;
    }
}

const struct MenuScreen EventDetailsMenu = {
        .handle_back = handle_back,
        .handle_down = handle_down,
        .handle_up = handle_up,
        .handle_ok = handle_ok,
        .draw_menu = draw_menu,
        .update_menu = update_menu,
};
