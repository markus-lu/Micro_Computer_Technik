#include "Clock.h"
#include "LEDKey.h"

void clock_init(struct State *state) {
	// Initialisierung LED & Key
	ledkey_init();
	// Initialisierung RTC
	rtc_init();
	// Helligkeit 7Seg
	ledkey_set_brightness(state->clock_brightness, true);
}

#define DEC0(value, max) if (state->time.value > 0) { state->time.value--; } else { state->time.value = (max); } break;
#define INC0(value, max) if (state->time.value < (max)) { state->time.value++; } else { state->time.value = 0; } break;
#define DEC1(value, max) if (state->time.value > 1) { state->time.value--; } else { state->time.value = (max); } break;
#define INC1(value, max) if (state->time.value < (max)) { state->time.value++; } else { state->time.value = 1; } break;


void clock_check_keypress(struct State *state) {
	uint8_t buttons = ledkey_get_buttons();
	if (buttons != state->clock_last_buttons) {
		state->clock_should_redraw = true;
		switch (buttons) {
		case BUTTON_TOGGLE_TIME_DATE:
			if (!state->clock_edit_mode) {
				state->clock_show_time = !state->clock_show_time;
			}
			break;
		case BUTTON_EDIT:
			if (!state->clock_edit_mode) {
				state->clock_edit_mode = true;
				state->clock_selected_field = SELECTED_HOUR;
				state->clock_show_time = true;
				state->blink = true;
			} else {
				state->clock_edit_mode = false;
				state->blink = false;
				rtc_write_time(&state->time);
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
		case BUTTON_DEC:
			switch (state->clock_selected_field) {
			case SELECTED_HOUR:
				DEC0(hours, 23)
			case SELECTED_MINUTE:
				DEC0(minutes, 59)
			case SELECTED_SECOND:
				DEC0(seconds, 59)
			case SELECTED_DAY:
				DEC1(day, 31)
			case SELECTED_MONTH:
				DEC1(month, 12)
			case SELECTED_YEAR:
				if (state->time.year > 0) {
					state->time.year--;
				} else {
					state->time.year = 99;
					state->time.century = !state->time.century;
				}
				break;
			case SELECTED_WEEKDAY:
				if (state->time.weekday < SUNDAY) {
					state->time.weekday++;
				} else {
					state->time.weekday = MONDAY;
				}

				break;
			}
			break;
		case BUTTON_INC:
			switch (state->clock_selected_field) {
			case SELECTED_HOUR:
				INC0(hours, 23)
			case SELECTED_MINUTE:
				INC0(minutes, 59)
			case SELECTED_SECOND:
				INC0(seconds, 59)
			case SELECTED_DAY:
				INC1(day, 31)
			case SELECTED_MONTH:
				INC1(month, 12)
			case SELECTED_YEAR:
				if (state->time.year < 99) {
					state->time.year++;
				} else {
					state->time.year = 0;
					state->time.century = !state->time.century;
				}
				break;
			case SELECTED_WEEKDAY:
				if (state->time.weekday > MONDAY) {
					state->time.weekday--;
				} else {
					state->time.weekday = SUNDAY;
				}
				break;
			}
			break;
		case BUTTON_BRIGHTNESS_DOWN:
			if (state->clock_brightness > -1) {
				state->clock_brightness--;
			}

			if (state->clock_brightness > -1) {
				ledkey_set_brightness(state->clock_brightness, true);
			} else {
				ledkey_set_brightness(0, false);
			}
			break;
		case BUTTON_BRIGHTNESS_UP:
			if (state->clock_brightness < MAX_BRIGHTNESS) {
				state->clock_brightness++;
			}
			ledkey_set_brightness(state->clock_brightness, true);
			break;
		}
		state->clock_last_buttons = buttons;
	}
}

static const uint8_t SEGMENT_DIGITS[] = { SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
		SEG_5, SEG_6, SEG_7, SEG_8, SEG_9, SEG_DOT };

void clock_draw_time(struct State *state) {
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
	}
	uint8_t weekday = (1 << state->time.weekday);

	if (state->clock_edit_mode && state->blink) {
		switch (state->clock_selected_field) {
		case SELECTED_MINUTE:
			segments[4] = 0;
			segments[5] = 0 | SEG_DOT;
			break;
		case SELECTED_SECOND:
			segments[6] = 0;
			segments[7] = 0;
			break;
		case SELECTED_DAY:
			segments[0] = 0;
			segments[1] = 0 | SEG_DOT;
			break;
		case SELECTED_MONTH:
		case SELECTED_HOUR:
			segments[2] = 0;
			segments[3] = 0 | SEG_DOT;
			break;
		case SELECTED_YEAR:
			segments[4] = 0;
			segments[5] = 0;
			segments[6] = 0;
			segments[7] = 0;
			break;
		case SELECTED_WEEKDAY:
			weekday = 0;
			break;
		}
	}

	ledkey_set_display_data(weekday, segments);
}

void clock_loop_once(struct State *state) {
	// Schalter verarbeitung
	clock_check_keypress(state);
	if (state->clock_should_redraw) {
		clock_draw_time(state);
		state->clock_should_redraw = false;
	}
}
