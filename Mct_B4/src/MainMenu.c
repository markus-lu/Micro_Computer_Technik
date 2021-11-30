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
	// Zälung der aktivierten Events
    uint8_t event_count = events_get_count(state->event_data);
    // Tempeartur in Vorkommastelle und Nachkommastelle zerlegen
    uint8_t temperature_whole = state->temperature >> 8;
    uint8_t temperature_fraction = state->temperature;

    // Bildschirm löschen (Tafeldienst)
    lcd_clrscr();

    // Temp in die erste Zeile Schreiben
    lcd_gotoxy(1, 1);
    lcd_write_string("Temperatur: ");
    lcd_write_char(temperature_whole / 10 + '0');
    lcd_write_char(temperature_whole % 10 + '0');
    lcd_write_char('.');
    // Ausgabe der Nachkommastellen
    // 0xDF -> Gradzeichen
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
    // Aktueller Status der RGB-Led wird in der zweiten Zeile ausgeben
    lcd_gotoxy(1, 2);
    lcd_write_string("Status:     ");
    lcd_write_string(state->rgb_state ? "An" : "Aus");

    // Eventanzahl wird in der dritten Zeile ausgegeben
    lcd_gotoxy(1, 3);
    lcd_write_string("Events:     ");
    lcd_write_char(event_count / 10 + '0');
    lcd_write_char(event_count % 10 + '0');
}
