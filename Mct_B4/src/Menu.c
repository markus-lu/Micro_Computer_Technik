#include "Menu.h"
#include "FrontIO.h"
#include "lcdlib_1769.h"
#include "MainMenu.h"
#include "EventsMenu.h"
#include "EventDetailsMenu.h"

void menu_init () {
	// Initialisierung Frontplatte
    frontio_init();
    // Initialisierung LCD
    lcd_init(LCD_LINES, I2C_SPEED);
    // LCD Hintergrundbeleuchtung einschalten
    lcd_bglight(0xFF);
    // Curser des LCD auschalten
    lcd_cursor(0);
}

void menu_check_keypress (struct State *state) {
	// Aktuell gedrückte Taster holen
    uint8_t buttons = frontio_get_buttons();
    // LEDs für gedrüten Taster einschalten
    frontio_set_leds(buttons);
    // Portexpander in Input modus setzen
    // dies geschiet durch eine Lesebefehl
    frontio_get_buttons();
    // wenn Taster sich verändert hat
    if (buttons != state->menu_last_buttons) {
    	// Je nach menübildschirm aktuell in benutzung
    	// Leite Tastendruck weiter
        switch (state->menu_screen) {
            case SCREEN_MAIN_MENU:
                main_menu_handle_keypress(state, buttons);
                break;
            case SCREEN_EVENTS_MENU:
                events_menu_handle_keypress(state, buttons);
                break;
            case SCREEN_EVENT_DETAILS_MENU:
                event_details_menu_handle_keypress(state, buttons);
                break;
        }
        // Speichern der der Gedrückten Tasten
        state->menu_last_buttons = buttons;
    }
}

void menu_loop_once (struct State *state) {
	// Prüfen ob eine Taste Gedrückt wurde
    menu_check_keypress(state);
    if (state->menu_should_redraw) {
        switch (state->menu_screen) {
            case SCREEN_MAIN_MENU:
                main_menu_draw_menu(state);
                break;
            case SCREEN_EVENTS_MENU:
                events_menu_draw_menu(state);
                break;
            case SCREEN_EVENT_DETAILS_MENU:
                event_details_menu_draw_menu(state);
                break;
        }
        state->menu_should_redraw = false;
    }
}
