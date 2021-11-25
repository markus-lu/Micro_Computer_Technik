#include "Events.h"
#include "Clock.h"
#include "LEDKey.h"

// TODO: Find correct values for these!
#define BUTTON_EDIT 0
#define BUTTON_DOWN 1
#define BUTTON_UP 2
#define BUTTON_BRIGHTNESS_DOWN 3
#define BUTTON_BRIGHTNESS_UP 4

void init() {
    LEDKey.init();
    RTC.init();
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
            segments[state->clock_selected_field * 2 + 2] = 0;
            segments[state->clock_selected_field * 2 + 3] = 0 | SEG_DOT;
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
        segments[6] = SEGMENT_DIGITS[state->time.year / 10];
        segments[7] = SEGMENT_DIGITS[state->time.year % 10];
        if (state->blink) {
            switch (state->clock_selected_field) {
                case 3:
                    segments[0] = 0;
                    segments[1] = 0 | SEG_DOT;
                    break;
                case 4:
                    segments[2] = 0;
                    segments[3] = 0 | SEG_DOT;
                    break;
                case 5:
                    segments[4] = 0;
                    segments[5] = 0;
                    segments[6] = 0;
                    segments[7] = 0;
                    break;
            }
        }
    }

    LEDKey.set_display_data((state->blink && state->clock_selected_field == 6) ? 0 : state->time.weekday, segments);
}

#define DEC(value, max) if (state->time.value > 0) { state->time.value--; } else { state->time.value = (max); } break;
#define INC(value, max) if (state->time.value < (max)) { state->time.value++; }else{state->time.value=(0);} break;

static void check_keypress(struct State *state) {
    bool should_redraw = state->clock_should_redraw;
    uint8_t buttons = LEDKey.get_buttons();
    if (buttons != state->clock_last_buttons) {
        state->clock_should_redraw = true;
        switch (buttons) {
            case BUTTON_EDIT:
                state->clock_edit_mode = !state->clock_edit_mode;
                state->clock_selected_field = 0;
                state->blink = true;
                break;
            case BUTTON_DOWN:
                switch (state->clock_selected_field) {
                    case 0: DEC(hours, 23)
                    case 1: DEC(minutes, 59)
                    case 2: DEC(seconds, 59)
                    case 3: DEC(day, 31)
                    case 4: DEC(month,12)
                    case 5:
                        if (state->time.year > 0) {
                            state->time.year--;
                        } else {
                            state->time.year = 99;
                            state->time.century = !state->time.century;
                        }
                        break;
                    case 6:
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
                    case 0: INC(hours, 24)
                    case 1: INC(minutes, 60)
                    case 2: INC(seconds, 60)
                    case 3: INC(day, 31)
                    case 4: INC(month,12)
                    case 5:
                        if (state->time.year < 99) {
                            state->time.year++;
                        } else {
                            state->time.year = 0;
                            state->time.century = !state->time.century;
                        }
                    case 6:
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
                if (state->clock_brightness > 0) {
                    state->clock_brightness--;
                }
                LEDKey.set_brightness(state->clock_brightness, true);
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
