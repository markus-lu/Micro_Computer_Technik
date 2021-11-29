#include "EventsMenu.h"
#include "Menu.h"
#include "Events.h"
#include <stdio.h>
#include "lcdlib_1769.h"

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
		lcd_write_char(' ');
		if (state->selected_event < EVENT_COUNT - 1) {
			state->selected_event++;
		}
		goto_event(state->selected_event);
		lcd_write_char('>');
		break;

	case BUTTON_DOWN:
		goto_event(state->selected_event);
		lcd_write_char(' ');
		if (state->selected_event > 0) {
			state->selected_event--;
		}
		goto_event(state->selected_event);
		lcd_write_char('>');
		break;

	case BUTTON_OK:
		state->menu_screen = SCREEN_EVENT_DETAILS_MENU;
		state->menu_should_redraw = true;
		break;
	}
}

static void goto_event(uint8_t event) {
	// tbd
	uint8_t x = (event / 4) * 5;
	uint8_t y = event % 4;
	lcd_gotoxy(x + 1, y + 1);
}

static void write_event(struct State *state, char_t *string, int event) {
	char_t selected = state->selected_event == event ? '>' : ' ';
	char_t enabled = state->event_data[event].enabled ? 'x' : 0xDB;

	sprintf(string, "%c%c %02d", selected, enabled, event);
}

void events_menu_draw_menu(struct State *state) {
	lcd_clrscr();
	char_t event[5][5];
	event[4][0] = 0;
	char_t *line = (char_t*) event;

	for (int i = 0; i < EVENT_COUNT / 4; ++i) {
		lcd_gotoxy(1, i + 1);
		write_event(state, event[0], i);
		write_event(state, event[1], i + 4);
		write_event(state, event[2], i + 8);
		write_event(state, event[3], i + 12);
		lcd_write_string(line);
	}
}
