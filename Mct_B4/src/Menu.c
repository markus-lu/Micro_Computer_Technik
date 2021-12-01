#include "Menu.h"
#include "FrontIO.h"
#include "lcdlib_1769.h"
#include "MainMenu.h"
#include "EventsMenu.h"
#include "EventDetailsMenu.h"

/**
 *  \file     Menu.c
*/


/*********************************************************************/
 /**
  \brief Diese Funktion initialisiert alle für das Menü benötigte Hardware.
 Dabei handelt es sich um das LC-Display und die Frontplatte.
 Zusätzlich werden noch der Cursor und die Hintergrundbeleuchtung des
 LC-Displays gesetzt.

 \param   -

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
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

/*********************************************************************/
 /**
  \brief Diese Funktion speichert sich gedrückte Taster ab und schaltet die dazugehörige
 LED ein. Danach werden je nach aktuell angezeigtem Menü die Taster anders
 verarbeitet. Die Verarbeitung ist in zusätzliche Methoden ausgelagert worden.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
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

/*********************************************************************/
 /**
  \brief Diese Funktion fragt ab ob eine Taste gedrückt wurde. Wenn der Wert
 von Neuzeichen auf wahr steht, wird je nach aktuell angezeigtem Menü das
 Neuzeichnen anders verarbeitet. Die Verarbeitung ist in zusätzliche Methoden
 ausgelagert worden.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void menu_loop_once (struct State *state) {
	// Prüfen ob eine Taste Gedrückt wurde
    menu_check_keypress(state);
    // Wenn Bildschirm neu gezeichnet werden muss
    if (state->menu_should_redraw) {
    	// Abhängig vom aktuellen angezeigten Menü
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
        // Neu zeichnen auf Falsch setzten
        state->menu_should_redraw = false;
    }
}
