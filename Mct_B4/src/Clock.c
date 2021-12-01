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
	// Abfrage der gedrückten Taster
	uint8_t buttons = ledkey_get_buttons();
	// Wenn Taster sich verändert haben
	if (buttons != state->clock_last_buttons) {
		// 7Seg neuzeichen auf Wahr setzen
		state->clock_should_redraw = true;
		// Abhängig vom gedrückten Taster
		switch (buttons) {
		// Wenn Taster für Datum und Uhrzeit gedrückt wurde
		case BUTTON_TOGGLE_TIME_DATE:
			// Wenn im nicht im Bearbeitungsmodus
			if (!state->clock_edit_mode) {
				// Wechsel zwischen Datum und Uhrzeit
				state->clock_show_time = !state->clock_show_time;
			}
			break;
		// Wenn Taster für Bearbeiten gedrückt wurde
		case BUTTON_EDIT:
			// Wenn nicht im Bearbeitungsmodus
			if (!state->clock_edit_mode) {
				// Bearbeitungsmodus auf wahr setzten
				state->clock_edit_mode = true;
				// Stundenfeld auswälen
				state->clock_selected_field = SELECTED_HOUR;
				// Auf Uhrzeit anzeigen setzen
				state->clock_show_time = true;
				// Blinken einschalten
				state->blink = true;
			// Wenn im Bearbeitungsmodus
			} else {
				// Bearbeitungsmodus auschalten
				state->clock_edit_mode = false;
				// Blinken auschalten
				state->blink = false;
				// Neu Zeit auf den RTC schreiben
				rtc_write_time(&state->time);
			}
			break;
		// Wenn der Taster Zurück gedrückt wurde
		case BUTTON_PREVIOUS:
			// Wenn nicht die Stunden ausgewält sind
			if (state->clock_selected_field != SELECTED_HOUR) {
				// Um einen nach rechts schieben
				// Auf der 7Seg wird ein feld weiter Links ausgewählt
				state->clock_selected_field >>= 1;
			// Wenn die Stunden ausgewählt sind
			} else {
				// Zurück zum Wochentag springen
				state->clock_selected_field = SELECTED_WEEKDAY;
			}
			// Wenn eins der Uhrzeit Felder ausgewält ist
			if (state->clock_selected_field & SELECTED_TIME) {
				// Auf Uhrzeit anzeigen wechseln
				state->clock_show_time = true;
			// Wenn ein anderes Feld als Uhrzeit ausgewält ist
			} else {
				// Auf Datum anzeigen wechseln
				state->clock_show_time = false;
			}
			break;
		// Wenn der Taster Nächste gerückt wurde
		case BUTTON_NEXT:
			// Wenn nicht die Wochentag ausgewält sind
			if (state->clock_selected_field != SELECTED_WEEKDAY) {
				// Um einen nach links schieben
				// Auf der 7Seg wird ein feld weiter rechts ausgewählt
				state->clock_selected_field <<= 1;
			// Wenn der Wochentag ausgewält ist
			} else {
				// Stundenfeld auswählen
				state->clock_selected_field = SELECTED_HOUR;
			}

			// Wenn eins der Uhrzeit Felder ausgewält ist
			if (state->clock_selected_field & SELECTED_TIME) {
				// Auf uhrzeitanzeigen wechseln
				state->clock_show_time = true;
			} else {
				// Auf Datum anzeigen wechseln
				state->clock_show_time = false;
			}
			break;
		// Wenn Taster Runter gedrückt wurde
		case BUTTON_DEC:
			// Abhängig vom Augewählten Feld
			switch (state->clock_selected_field) {
			// Wenn Stunden ausgewählt sind
			case SELECTED_HOUR:
				DEC0(hours, 23)
			// Wenn Minuten ausgewählt sind
			case SELECTED_MINUTE:
				DEC0(minutes, 59)
			// Wenn Sekunden ausgewählt sind
			case SELECTED_SECOND:
				DEC0(seconds, 59)
			// Wenn Tag ausgewählt ist
			case SELECTED_DAY:
				DEC1(day, 31)
			// Wenn Monate ausgewählt sind
			case SELECTED_MONTH:
				DEC1(month, 12)
			// Wenn das Jahr ausgewält ist
			case SELECTED_YEAR:
				// Wenn das Jahr größer als 0 ist
				if (state->time.year > 0) {
					// Jahr minus 1 nehmen
					state->time.year--;
				// Wenn das Jahr gleich 0 ist
				} else {
					// Jahr auf 99 Setzen
					state->time.year = 99;
					// Jahundertbit wechseln
					state->time.century = !state->time.century;
				}
				break;
			// Wenn der Wochentag ausgewält ist
			case SELECTED_WEEKDAY:
				// Wenn der Wochentag kleiner als Sonnatg ist
				if (state->time.weekday < SUNDAY) {
					// Wochentag um Einen erhöhen
					state->time.weekday++;
				// Wen der Wochentag Sontag ist
				} else {
					// Wochentag auf Montag setzen
					state->time.weekday = MONDAY;
				}
				break;
			}
			break;
		// WEn der Taster Hoch gedrückt wurde
		case BUTTON_INC:
			// Je nach ausgewählten Feld
			switch (state->clock_selected_field) {
			// Wenn Stunden ausgewählt sind
			case SELECTED_HOUR:
				INC0(hours, 23)
			// Wenn Minuten ausgewählt sind
			case SELECTED_MINUTE:
				INC0(minutes, 59)
			// Wenn Sekunden ausgewählt sind
			case SELECTED_SECOND:
				INC0(seconds, 59)
			// Wenn Tage ausgewählt sind
			case SELECTED_DAY:
				INC1(day, 31)
			// Wenn Monate ausgewählt sind
			case SELECTED_MONTH:
				INC1(month, 12)
			// Wenn Jahre ausgewählt sind
			case SELECTED_YEAR:
				// Wenn Jahr kleiner als 99 ist
				if (state->time.year < 99) {
					// Jahr um eins erhöhen
					state->time.year++;
				// Wenn Jahr 99 ist
				} else {
					// Jahr auf null stezen
					state->time.year = 0;
					// Jahrundertbit wechseln
					state->time.century = !state->time.century;
				}
				break;
			// Wenn der Wochentag ausgewält ist
			case SELECTED_WEEKDAY:
				// Wenn Wochentag größer als Montag
				if (state->time.weekday > MONDAY) {
					// Einen Wochentag abziehen
					state->time.weekday--;
				// Wenn Wochentag Montag ist
				} else {
					// Auf Sontag setzen
					state->time.weekday = SUNDAY;
				}
				break;
			}
			break;
		// Wenn der Taster für Helligkeit Runter gedrückt wurde
		case BUTTON_BRIGHTNESS_DOWN:
			// Wenn die Helligkeit größer als -1 ist
			if (state->clock_brightness > -1) {
				// Helligkeit runterregeln
				state->clock_brightness--;
			}

			// Wenn Helligkeit immer noch größer als -1 ist
			if (state->clock_brightness > -1) {
				// Helligkeit setzen und 7Seg anschalten
				ledkey_set_brightness(state->clock_brightness, true);
			// Wenn Helligkeit gleich -1 ist
			} else {
				// 7seg Ausschalten
				ledkey_set_brightness(0, false);
			}
			break;
		// Wenn der Taster für Helligkeit hochregeln gedrückt wurde
		case BUTTON_BRIGHTNESS_UP:
			// Wenn Helligkeit kleiner als das Maximum ist
			if (state->clock_brightness < MAX_BRIGHTNESS) {
				// Helligkeit hochregeln
				state->clock_brightness++;
			}
			// Helligkeit setzen und 7Seg anschalten
			ledkey_set_brightness(state->clock_brightness, true);
			break;
		}
		// Neue Taster in den Status schreiben
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
	// Array constante mit den 7Seg Ziffern erstellen
	static const uint8_t SEGMENT_DIGITS[] = { SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
			SEG_5, SEG_6, SEG_7, SEG_8, SEG_9, SEG_DOT };

	//Speicher reservieren
	uint8_t segments[8];
	// Wenn Uhrzeit angezeigt wird
	if (state->clock_show_time) {
		// Ersten Beiden Segmente aus
		segments[0] = 0;
		segments[1] = 0;
		// Zehner Stunden schreiben
		segments[2] = SEGMENT_DIGITS[state->time.hours / 10];
		// Einer Stunden schreiben
		segments[3] = SEGMENT_DIGITS[state->time.hours % 10] | SEG_DOT;
		// Zehner Minuten schreiben
		segments[4] = SEGMENT_DIGITS[state->time.minutes / 10];
		// Einer Minuten schreiben
		segments[5] = SEGMENT_DIGITS[state->time.minutes % 10] | SEG_DOT;
		// Zehner Sekunden schreiben
		segments[6] = SEGMENT_DIGITS[state->time.seconds / 10];
		// Einer Sekunden schreiben
		segments[7] = SEGMENT_DIGITS[state->time.seconds % 10];
	// Wenn das Datum angezeit wird
	} else {
		// Zehner Tage schreiben
		segments[0] = SEGMENT_DIGITS[state->time.day / 10];
		// Einer Tage schreiben
		segments[1] = SEGMENT_DIGITS[state->time.day % 10] | SEG_DOT;
		// Zehener Monate schreiben
		segments[2] = SEGMENT_DIGITS[state->time.month / 10];
		// Einer Monate schreiben
		segments[3] = SEGMENT_DIGITS[state->time.month % 10] | SEG_DOT;
		// Wenn Jahrundertbit Wahr ist
		if (state->time.century) {
			// Schreibe 20
			segments[4] = SEGMENT_DIGITS[2];
			segments[5] = SEGMENT_DIGITS[0];
		// Wenn das Jahrhundertbit false ist
		} else {
			// Schreibe 19
			segments[4] = SEGMENT_DIGITS[1];
			segments[5] = SEGMENT_DIGITS[9];
		}
		// Zehner Jahr schreiben
		segments[6] = SEGMENT_DIGITS[(state->time.year / 10) % 10];
		// Einer Jahr schreiben
		segments[7] = SEGMENT_DIGITS[state->time.year % 10];
	}
	// Bit für Wochentag setzen und Schiften
	uint8_t weekday = (1 << state->time.weekday);

	// Wenn Bearbeitungsmodus an ist und Blinken an ist
	if (state->clock_edit_mode && state->blink) {
		// Je nach ausgewählten Feld blinkt es
		// Punkt blinkt nicht
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

	// Übersetzen und aufs 7seg schreiben
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
	// Wenn 7Seg neu gezeichnet werden muss
	if (state->clock_should_redraw) {
		// 7Seg neu Zeichnen
		clock_draw_time(state);
		// 7Seg neuzeichen auf falsch setzen
		state->clock_should_redraw = false;
	}
}
