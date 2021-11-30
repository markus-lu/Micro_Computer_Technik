#include "EventsMenu.h"
#include "Menu.h"
#include "Events.h"
#include <stdio.h>
#include "lcdlib_1769.h"

static void goto_event(uint8_t event) {
	// LCD hat 4 Zeilen mit je 20 Zeichen
	// Aufteilung der Events auf 5 Zeichen pro Event
	// X-Position alle 4 Events eine Spalte (5 Zeichen) nach Rechts
	uint8_t x = (event / 4) * 5;
	// Pro Spalte 4 Events
	uint8_t y = event % 4;
	// Cursor an die Position des Events setzen
	lcd_gotoxy(x + 1, y + 1);
}

void events_menu_handle_keypress(struct State *state, uint8_t buttons) {
	// Je nach gedrückten Taster
	switch (buttons) {
	// Back Taster geht zurück ins Mainmenü
	case BUTTON_BACK:
		state->menu_screen = SCREEN_MAIN_MENU;
		// Neuzeichnen auf Wahr setzen
		state->menu_should_redraw = true;
		break;

	case BUTTON_UP:
		// Cursor auf das aktuell ausgewählten Event setzen
		goto_event(state->selected_event);
		// Cursor Wird mit Leerzeichen überschrieben
		lcd_write_char(' ');
		// Wenn das Letze Event noch nicht ausgewählt
		if (state->selected_event < EVENT_COUNT - 1) {
			// Ein event hochzählen
			state->selected_event++;
		}
		// Cursor auf das aktuell ausgewählten Event setzen
		goto_event(state->selected_event);
		// Cursor neu setzen
		lcd_write_char('>');
		break;

	case BUTTON_DOWN:
		// Cursor auf das aktuell ausgewählten Event setzen
		goto_event(state->selected_event);
		// Cursor Wird mit Leerzeichen überschrieben
		lcd_write_char(' ');
		// Wenn das erste Event noch nicht ausgewählt
		if (state->selected_event > 0) {
			// Ein event runterzählen
			state->selected_event--;
		}
		// Cursor auf das aktuell ausgewählten Event setzen
		goto_event(state->selected_event);
		// Cursor neu setzen
		lcd_write_char('>');
		break;

	case BUTTON_OK:
		// Menüstatus auf Event-Details-Menü setzen
		state->menu_screen = SCREEN_EVENT_DETAILS_MENU;
		// Neuzeichnen auf Wahr setzen
		state->menu_should_redraw = true;
		break;
	}
}



static void write_event(struct State *state, char_t *string, int event) {
	// Wenn Event ausgewählt ist dann enthält der Char '>'
	// Ansonsten ' '
	char_t selected = state->selected_event == event ? '>' : ' ';
	// Wenn Event aktiviert ist dann enthält der Char 'x'
	// Ansonsten eine Box (0xDB)
	char_t enabled = state->event_data[event].enabled ? 'x' : 0xDB;

	// Bsp: ">x 02"
	sprintf(string, "%c%c %02d", selected, enabled, event);
}

void events_menu_draw_menu(struct State *state) {
	// LCD Löschen (Tafeldienst)
	lcd_clrscr();
	// Zweidim Array erstellen
	char_t event[5][5];
	// Am Ende eine Pysikalische 0 schreiben um das ende des Strings zu signalisieren
	event[4][0] = 0;
	// Char-Pointer erstellen (String)
	char_t *line = (char_t*) event;

	// Schleife zum zschreiben einer Zeile des Menüs auf LCD
	// Erste Zeile sind Events  0,4,8,12
	// Zweite Zeile sind Event  1,5,9,13
	// Dritte Zeile sind Event  2,6,10,14
	// Vierte Zeile sind Events 3,7,11,15
	for (int i = 0; i < EVENT_COUNT / 4; ++i) {
		// Startpunkt festsetzen
		lcd_gotoxy(1, i + 1);
		write_event(state, event[0], i);
		write_event(state, event[1], i + 4);
		write_event(state, event[2], i + 8);
		write_event(state, event[3], i + 12);
		// Zeile auf das LCD schreiben
		lcd_write_string(line);
	}
}
