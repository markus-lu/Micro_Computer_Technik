#include "Events.h"
#include "Clock.h"
#include "LEDKey.h"

void init(struct State *state) {
    LEDKey.init();
    RTC.init();
    LEDKey.set_brightness(state->clock_brightness, true);
}


#define DEC(value, max) if (state->time.value > 1) { state->time.value--; } else { state->time.value = (max); } break;
#define INC(value, max) if (state->time.value < (max)) { state->time.value++; }else{ state->time.value=(0); } break;

static void check_keypress(struct State *state) {
    bool should_redraw = state->clock_should_redraw;
    uint8_t buttons = LEDKey.get_buttons();
    if (buttons != state->clock_last_buttons) {
        state->clock_should_redraw = true;
        switch (buttons) {
            case BUTTON_TOGGLE_TIME_DATE:
                state->clock_show_time = !state->clock_show_time;
                break;
            case BUTTON_EDIT:
                if (!state->clock_edit_mode) {
                    state->clock_edit_mode = true;
                    state->clock_selected_field = SELECTED_HOUR;
                    state->blink = true;
                } else {
                    state->clock_edit_mode = false;
                    state->blink = false;
                    RTC.write_time(&state->time);
                }
                break;
            case BUTTON_PREVIOUS:
                if (state->clock_selected_field != SELECTED_HOUR) {
                    state->clock_selected_field >>= 1;
                } else {
                    state->clock_selected_field = SELECTED_WEEKDAY;
                }
                if (state->clock_selected_field & SELECTED_TIME) {
                    state->clock_show_time = true;
                } else {
                    state->clock_show_time = false;
                }
                break;
            case BUTTON_NEXT:
                if (state->clock_selected_field != SELECTED_WEEKDAY) {
                    state->clock_selected_field <<= 1;
                } else {
                    state->clock_selected_field = SELECTED_HOUR;
                }

                if (state->clock_selected_field & SELECTED_TIME) {
                    state->clock_show_time = true;
                } else {
                    state->clock_show_time = false;
                }
                break;
            case BUTTON_DOWN:
                switch (state->clock_selected_field) {
                    case SELECTED_HOUR:
                    DEC(hours, 23)
                    case SELECTED_MINUTE:
                    DEC(minutes, 59)
                    case SELECTED_SECOND:
                    DEC(seconds, 59)
                    case SELECTED_DAY:
                    DEC(day, 31)
                    case SELECTED_MONTH:
                    DEC(month, 12)
                    case SELECTED_YEAR:
                        if (state->time.year > 0) {
                            state->time.year--;
                        } else {
                            state->time.year = 99;
                            state->time.century = !state->time.century;
                        }
                        break;
                    case SELECTED_WEEKDAY:
                        if (state->time.weekday != Monday) {
                            state->time.weekday <<= 1;
                        } else {
                            state->time.weekday = Sunday;
                        }
                        break;
                }
                break;
            case BUTTON_UP:
                switch (state->clock_selected_field) {
                    case SELECTED_HOUR:
                    INC(hours, 24)
                    case SELECTED_MINUTE:
                    INC(minutes, 60)
                    case SELECTED_SECOND:
                    INC(seconds, 60)
                    case SELECTED_DAY:
                    INC(day, 31)
                    case SELECTED_MONTH:
                    INC(month, 12)
                    case SELECTED_YEAR:
                        if (state->time.year < 99) {
                            state->time.year++;
                        } else {
                            state->time.year = 0;
                            state->time.century = !state->time.century;
                        }
                        break;
                    case SELECTED_WEEKDAY:
                        if (state->time.weekday != Sunday) {
                            state->time.weekday >>= 1;
                        } else {
                            state->time.weekday = Monday;
                        }
                        break;
                }
                break;
            case BUTTON_BRIGHTNESS_DOWN:
                state->clock_should_redraw = false;
                if (state->clock_brightness > -1) {
                    state->clock_brightness--;
                }

                if (state->clock_brightness > -1) {
                    LEDKey.set_brightness(state->clock_brightness, true);
                } else {
                    LEDKey.set_brightness(0, false);
                }
                break;
            case BUTTON_BRIGHTNESS_UP:
                state->clock_should_redraw = false;
                if (state->clock_brightness < MAX_BRIGHTNESS) {
                    state->clock_brightness++;
                }
                LEDKey.set_brightness(state->clock_brightness, true);
                break;
            default:
                state->clock_should_redraw = should_redraw;
                break;
        }
        state->clock_last_buttons = buttons;
    }
}


static const uint8_t SEGMENT_DIGITS[] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7, SEG_8, SEG_9, SEG_DOT};

static void draw_time(struct State *state) {
    uint8_t segments[8];
    if (state->clock_show_time) {
        segments[0] = 0;
        segments[1] = 0;
        segments[2] = SEGMENT_DIGITS[state->time.hours / 10];
        segments[3] = SEGMENT_DIGITS[state->time.hours % 10] | SEG_DOT;
        segments[4] = SEGMENT_DIGITS[state->time.minutes / 10];
        segments[5] = SEGMENT_DIGITS[state->time.minutes % 10] | SEG_DOT;
        segments[6] = SEGMENT_DIGITS[state->time.seconds / 10];
        segments[7] = SEGMENT_DIGITS[state->time.seconds % 10];
        if (state->blink) {
            switch (state->clock_selected_field) {
                case SELECTED_HOUR:
                    segments[2] = 0;
                    segments[3] = 0 | SEG_DOT;
                    break;
                case SELECTED_MINUTE:
                    segments[4] = 0;
                    segments[5] = 0 | SEG_DOT;
                    break;
                case SELECTED_SECOND:
                    segments[6] = 0;
                    segments[7] = 0;
                    break;
            }
        }
    } else {
        segments[0] = SEGMENT_DIGITS[state->time.day / 10];
        segments[1] = SEGMENT_DIGITS[state->time.day % 10] | SEG_DOT;
        segments[2] = SEGMENT_DIGITS[state->time.month / 10];
        segments[3] = SEGMENT_DIGITS[state->time.month % 10] | SEG_DOT;
        if (state->time.century) {
            segments[4] = SEGMENT_DIGITS[2];
            segments[5] = SEGMENT_DIGITS[0];
        } else {
            segments[4] = SEGMENT_DIGITS[1];
            segments[5] = SEGMENT_DIGITS[9];
        }
        segments[6] = SEGMENT_DIGITS[(state->time.year / 10) % 10];
        segments[7] = SEGMENT_DIGITS[state->time.year % 10];
        if (state->blink) {
            switch (state->clock_selected_field) {
                case SELECTED_DAY:
                    segments[0] = 0;
                    segments[1] = 0 | SEG_DOT;
                    break;
                case SELECTED_MONTH:
                    segments[2] = 0;
                    segments[3] = 0 | SEG_DOT;
                    break;
                case SELECTED_YEAR:
                    segments[4] = 0;
                    segments[5] = 0;
                    segments[6] = 0;
                    segments[7] = 0;
                    break;
            }
        }
    }
    uint8_t weekday = state->time.weekday;
    if (state->blink && state->clock_selected_field == SELECTED_WEEKDAY) {
        weekday = 0;
    }

    LEDKey.set_display_data(weekday, segments);
}

void loop_once(struct State *state) {
    check_keypress(state);
    if (state->clock_should_redraw) {
        draw_time(state);
        state->clock_should_redraw = false;
    }
}

const struct clock Clock = {
        .init = init,
        .loop_once = loop_once,
};
