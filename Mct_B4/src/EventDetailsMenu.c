#include "EventDetailsMenu.h"
#include "Menu.h"
#include "Events.h"
#include "lcdlib_1769.h"

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

static void handle_back(struct State *state);

static void handle_up(struct State *state);

static void handle_down(struct State *state);

static void handle_ok(struct State *state);

void event_details_menu_handle_keypress(struct State *state, uint8_t buttons) {
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
    if (state->menu_edit_mode) {
        state->menu_edit_mode = false;
    } else {
        state->menu_screen = SCREEN_EVENTS_MENU;
        state->selected_event_detail = SELECTED_MONDAY;
        state->menu_edit_mode = false;
    }
}

static void handle_up(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (!state->menu_edit_mode) {
        state->blink = false;
        event_details_menu_update_menu(state);
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
                lcd_gotoxy(7, 2);
                lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
                lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
                state->menu_should_redraw = false;
                break;
            case SELECTED_MINUTE:
                if (event->minute < 59) {
                    event->minute++;
                } else {
                    event->minute = 0;
                }
                lcd_gotoxy(10, 2);
                lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
                lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
                state->menu_should_redraw = false;
                break;
        }
    } else if (state->selected_event_detail < SELECTED_ENABLED) {
        state->selected_event_detail++;
    } else {
        state->selected_event_detail = SELECTED_MONDAY;
    }
}

static void handle_down(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];
    if (!state->menu_edit_mode) {
        state->blink = false;
        event_details_menu_update_menu(state);
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
                lcd_gotoxy(7, 2);
                lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
                lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
                state->menu_should_redraw = false;
                break;
            case SELECTED_MINUTE:
                if (event->minute > 0) {
                    event->minute--;
                } else {
                    event->minute = 59;
                }
                lcd_gotoxy(10, 2);
                lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
                lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
                state->menu_should_redraw = false;
                break;
        }
    } else if (state->selected_event_detail > SELECTED_MONDAY) {
        state->selected_event_detail--;
    } else {
        state->selected_event_detail = SELECTED_ENABLED;
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

    event_details_menu_update_menu(state);
    state->menu_should_redraw = false;
}

void event_details_menu_draw_menu(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    lcd_clrscr();

    lcd_gotoxy(1, 1);
    lcd_write_string("Tag: ");
    lcd_write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
    lcd_write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
    lcd_write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
    lcd_write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
    lcd_write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
    lcd_write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
    lcd_write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));

    lcd_gotoxy(1, 2);
    lcd_write_string("Zeit: ");
    lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
    lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
    lcd_write_char(':');
    lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
    lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));

    lcd_gotoxy(1, 3);
    lcd_write_string("Schaltzustand: ");
    lcd_write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));

    lcd_gotoxy(1, 4);
    lcd_write_string("Aktiv: ");
    lcd_write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
}

void event_details_menu_update_menu(struct State *state) {
    struct Event *event = &state->event_data[state->selected_event];

    switch (state->selected_event_detail) {
        case SELECTED_MONDAY:
            lcd_gotoxy(6, 1);
            lcd_write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
            break;
        case SELECTED_TUESDAY:
            lcd_gotoxy(8, 1);
            lcd_write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
            break;
        case SELECTED_WEDNESDAY:
            lcd_gotoxy(10, 1);
            lcd_write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
            break;
        case SELECTED_THURSDAY:
            lcd_gotoxy(12, 1);
            lcd_write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
            break;
        case SELECTED_FRIDAY:
            lcd_gotoxy(14, 1);
            lcd_write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
            break;
        case SELECTED_SATURDAY:
            lcd_gotoxy(16, 1);
            lcd_write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
            break;
        case SELECTED_SUNDAY:
            lcd_gotoxy(18, 1);
            lcd_write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));
            break;
        case SELECTED_HOUR:
            lcd_gotoxy(7, 2);
            lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour / 10 + '0'));
            lcd_write_char(SELECTED1(SELECTED_HOUR, event->hour % 10 + '0'));
            break;
        case SELECTED_MINUTE:
            lcd_gotoxy(10, 2);
            lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute / 10 + '0'));
            lcd_write_char(SELECTED1(SELECTED_MINUTE, event->minute % 10 + '0'));
            break;
        case SELECTED_ON_OR_OFF:
            lcd_gotoxy(16, 3);
            lcd_write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));
            break;
        case SELECTED_ENABLED:
            lcd_gotoxy(8, 4);
            lcd_write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
            break;
    }
}
