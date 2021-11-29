#include "MainMenu.h"
#include "Menu.h"
#include "Events.h"
#include <stdio.h>
#include "lcdlib_1769.h"

void main_menu_handle_keypress(struct State *state, uint8_t buttons) {
	// Je nach gedrückten Taster
    switch (buttons) {
    // Bei Zurück, Hoch und Runter passiert nichts
        case BUTTON_BACK:
        case BUTTON_UP:
        case BUTTON_DOWN:
            state->menu_should_redraw = false;
            break;
		// Bei OK wechseln wir ins Eventsmenü
        case BUTTON_OK:
        	// Status auf Eventsmenü ändern
            state->menu_screen = SCREEN_EVENTS_MENU;
            // Neuzeichnen auf Wahr setzen
            state->menu_should_redraw = true;
            break;
    }
}

void main_menu_draw_menu(struct State *state) {
    uint16_t event_count = events_get_count(state->event_data);
    uint8_t temperature_whole = state->temperature >> 8;
    uint8_t temperature_fraction = state->temperature;

    lcd_clrscr();

    lcd_gotoxy(1, 1);
    lcd_write_string("Temperatur: ");
    lcd_write_char(temperature_whole / 10 + '0');
    lcd_write_char(temperature_whole % 10 + '0');
    lcd_write_char('.');
    switch (temperature_fraction) {
        case 0:
            lcd_write_string("00" "\xDF" "C");
            break;
        case 1:
            lcd_write_string("25" "\xDF" "C");
            break;
        case 2:
            lcd_write_string("50" "\xDF" "C");
            break;
        case 3:
            lcd_write_string("75" "\xDF" "C");
            break;
        default:
            break;
    }

    lcd_gotoxy(1, 2);
    lcd_write_string("Status:     ");
    lcd_write_string(state->rgb_state ? "An" : "Aus");

    lcd_gotoxy(1, 3);
    lcd_write_string("Events:     ");
    lcd_write_char(event_count / 10 + '0');
    lcd_write_char(event_count % 10 + '0');
}
