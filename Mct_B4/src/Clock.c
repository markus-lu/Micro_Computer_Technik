#include "Clock.h"
#include "LEDKey.h"

/**
 *  \file    Clock.c
*/


/**
 * Programmcode zum Nullbasierten decrementiren
 */
#define DEC0(value, max) if (state->time.value > 0) { state->time.value--; } else { state->time.value = (max); } break;

/**
 * Programmcode zum Nullbasierten incrementiren
 */
#define INC0(value, max) if (state->time.value < (max)) { state->time.value++; } else { state->time.value = 0; } break;

/**
 * Programmcode zum Einsbasierten decrementiren
 */
#define DEC1(value, max) if (state->time.value > 1) { state->time.value--; } else { state->time.value = (max); } break;

/**
 * Programmcode zum Einsbasierten incrementiren
 */
#define INC1(value, max) if (state->time.value < (max)) { state->time.value++; } else { state->time.value = 1; } break;


/*********************************************************************/
 /**
 Diese Funktion initialisiert alle für die Clock nötigen Komponenten.
 Zusätzlich wird die Helligkeit der 7Seg Anzeige gesetzt.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void clock_init(struct State *state) {
	// Initialisierung LED & Key
	ledkey_init();
	// Initialisierung RTC
	rtc_init();
	// Helligkeit 7Seg
	ledkey_set_brightness(state->clock_brightness, true);
}



/*********************************************************************/
 /**
 Diese Funktion reagiert auf Taster und verarbeitet diese

 Taster von rechts
 1. Wechsel zwischen Uhrzeit und Datum<BR>
 2. Bearbeitungsmodus an/aus schalten<BR>
 3. Voherigesfeld auswälen<BR>
 4. Nächstes Feld auswälen<BR>
 5. Ausgewältes Feld Runterzählen<BR>
 6. Ausgewältes Feld Hochzählen<BR>
 7. Helligkeit Runterregeln<BR>
 8. Helligkeit Hochregeln<BR>

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
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


/*********************************************************************/
 /**
 Diese Funktion schreibt je nachdem was ausgewält ist, die Uhrzeit oder das
 Datum auf die 7Seg Anzeige


 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void clock_draw_time(struct State *state) {
	static const uint8_t SEGMENT_DIGITS[] = { SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
			SEG_5, SEG_6, SEG_7, SEG_8, SEG_9, SEG_DOT };

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

/*********************************************************************/
 /**
 Diese Funktion chekt ob ein Taster gedrückt wurde und lässt die 7Seg Anzeige
 neu zeichnen


 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void clock_loop_once(struct State *state) {
	// Schalter verarbeitung
	clock_check_keypress(state);
	if (state->clock_should_redraw) {
		clock_draw_time(state);
		state->clock_should_redraw = false;
	}
}
