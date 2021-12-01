#include "EventDetailsMenu.h"
#include "Menu.h"
#include "Events.h"
#include "lcdlib_1769.h"

/**
 *  \file     EventsDetailsMenu.c
*/


/**
 * Wenn Eventdetail am Index ausgewählt ist und Blink wahr ist, wird die ausgewälte Stelle durch einen Block ersetzt
 */
#define SELECTED1(index, normal) (state->selected_event_detail == index && state->blink) ? '\xFF' : (normal)

/**
 * Wenn Eventdetail am Index ausgewählt ist und Blink wahr ist, wird die ausgewälte Stelle durch zwei Blöcke ersetzt
 */
#define SELECTED2(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF") : (normal)

/**
 * Wenn Eventdetail am Index ausgewählt ist und Blink wahr ist, wird die ausgewälte Stelle durch drei Blöcke ersetzt
 */
#define SELECTED3(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF") : (normal)

/**
 * Wenn Eventdetail am Index ausgewählt ist und Blink wahr ist, wird die ausgewälte Stelle durch vier Blöcke ersetzt
 */
#define SELECTED4(index, normal) (state->selected_event_detail == index && state->blink) ? ("\xFF" "\xFF" "\xFF" "\xFF") : (normal)

static void handle_back(struct State *state);

static void handle_up(struct State *state);

static void handle_down(struct State *state);

static void handle_ok(struct State *state);



/*********************************************************************/
 /**
 Diese Funktion verarbeitet einen Tastendruck, wenn das Menü sich im
 Events-Details-Menü befindet. Die Verabeitung der einzelden Tasten sind in
 zusätzliche Methoden ausgelagert worden.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)

\param	buttons
		Ein Integer der eine Codierung für gedrückte Taster enthält
		(0 - 31)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void event_details_menu_handle_keypress(struct State *state, uint8_t buttons) {
	// Je nach gedrückten Button in die dazugehörige handle Methode weiterleiten
    switch (buttons) {
        case BUTTON_BACK:
            handle_back(state);
            break;
        case BUTTON_UP:
            handle_up(state);
            break;
        case BUTTON_DOWN:
            handle_down(state);
            break;
        case BUTTON_OK:
            handle_ok(state);
            break;
    }
}

/*********************************************************************/
 /**
 Diese Funktion verarbeitet den Tastendruck Zurück.

 Wenn mann im Bearbeitungsmodus ist, verlässt man diesen.

 Wenn mann nicht im Bearbeitungsmodus ist, wechselt mann in das Events-Menü

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
static void handle_back(struct State *state) {
	// Wenn im Bearbeitungsmodus für die Uhrzeit
    if (state->menu_event_time_edit_mode) {
    	// Verlasse den Bearbeitungsmodus
        state->menu_event_time_edit_mode = false;
    } else {
    	// Wenn nicht im Bearbeitungsmodus
    	// Menüstaus auf Events-Menü setzen
        state->menu_screen = SCREEN_EVENTS_MENU;
        // Montag in den Eventsdetails auswälen
        // Zum Rücksetzen des Cursors für das darauf folgende Event
        state->selected_event_detail = SELECTED_MONDAY;
		// Neuzeichnen auf Wahr setzen
        state->menu_should_redraw = true;
    }
}

/*********************************************************************/
 /**
Diese Funktion verarbeitet den Tastendruck Hoch.

Wenn man im Bearbeitungsmodus ist, werden je nachdem, was ausgewält ist, die
Stunden oder Minuten hochgezäht.

Wenn man nicht im Bearbeitungsmodus ist, wird das Blinken aus gemacht und man
springt zum nächsten Event-Detail.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
static void handle_up(struct State *state) {
	// Pointer zum aktuellen Event holen
    struct Event *event = &state->event_data[state->selected_event];

    // Wenn Eventzeit bearbeitet wird
    if (state->menu_event_time_edit_mode) {
    	// Wenn Stunde oder Minute ausgewählt ist
        switch (state->selected_event_detail) {
        	// Bei ausgewälter Stunde
            case SELECTED_HOURS:
            	// Wenn kleiner als 23 Uhr
                if (event->hour < 23) {
                	// Stunden hochzälen
                    event->hour++;
				// Wenn 23 Uhr
                } else {
                	// Stunden auf 0 setzen
                    event->hour = 0;
                }
                // Stunde neu Zeichnen
                event_details_menu_update_menu(state);
                break;
			// Bei ausgewählter Minute
            case SELECTED_MINUTES:
            	// Wenn Minute Kleiner als 59
                if (event->minute < 59) {
                	// Minuten hochzälen
                    event->minute++;
				// Wenn Minute 59 ist
                } else {
                	// Minuten auf 0 setzen
                    event->minute = 0;
                }
                // Minuten neu Zeichen
                event_details_menu_update_menu(state);
                break;
        }
	// Wenn nicht im Bearbeitungsmodus
    } else {
    	// Blinken ausschalten
		state->blink = false;
		// Blinken vom Bildschirm entfernen
		event_details_menu_update_menu(state);
		// Wenn der Cursor noch vor dem letzen Element ist
    	if (state->selected_event_detail < SELECTED_ENABLED) {
    		// Zum nächsten Detail springen
			state->selected_event_detail++;
		// Wenn der Cursor beim lezten Element ist
		} else {
			// Cursor Springt an den Anfang (Montag)
			state->selected_event_detail = SELECTED_MONDAY;
		}
    }
}

/*********************************************************************/
 /**
Diese Funktion verarbeitet den Tastendruck Runter.

Wenn man im Bearbeitungsmodus ist, werden je nachdem was ausgewält ist, die
Stunden oder Minuten runtergezäht.

Wenn man nicht im Bearbeitungsmodus ist, wird das Blinken aus gemacht und man
springt zum vorherigen Event-Detail.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
static void handle_down(struct State *state) {
	// Pointer zum aktuellen Event holen
    struct Event *event = &state->event_data[state->selected_event];

    // Wenn Stunde oder Minute ausgewählt ist
    if (state->menu_event_time_edit_mode) {
        switch (state->selected_event_detail) {
        	// Bei ausgewälter Stunde
            case SELECTED_HOURS:
            	// Wenn größer als 0 Uhr
                if (event->hour > 0) {
                	// Stunden runterzählen
                    event->hour--;
                // Wenn Stunden 0 Uhr
                } else {
                	// Stunden auf 23 setzen
                    event->hour = 23;
                }
                // Stunde neu Zeichnen
                event_details_menu_update_menu(state);
                break;
			// Bei ausgewälter Minute
            case SELECTED_MINUTES:
            	// Wenn minute größer als 0
                if (event->minute > 0) {
                	// Minute runterzälen
                    event->minute--;
				// Wenn Minute bei 0
                } else {
                	// Minuten auf 59 setzen
                    event->minute = 59;
                }
                // Minuten neu Zeichen
                event_details_menu_update_menu(state);
                break;
        }
	// Wenn nicht im Bearbeitungsmodus
    } else{
    	// Blinken ausschalten
    	state->blink = false;
    	// Blinken vom Bildschirm entfernen
		event_details_menu_update_menu(state);
		// Wenn der Cursor nicht auf dem ersten Element ist
    	if (state->selected_event_detail > SELECTED_MONDAY) {
    		// Zum vorherigen Detail springen
			state->selected_event_detail--;
		} else {
			// Cursor Springt an das Ende (Aktiv ja/nein)
			state->selected_event_detail = SELECTED_ENABLED;
		}
	}
}

/*********************************************************************/
 /**
Diese Funktion verarbeitet den Tastendruck OK.
Je nachdem welches Eventdetail ausgewält ist, wird der Tastendruck unterschiedlich verarbeitet.

Wenn ein Wochentag ausgewält ist wird dieser aktiviert (Großbuchstaben) oder deaktiviert (kleinbuchstaben)

Wenn die Uhrzeit (Stunden oder Minuten) ausgewählt ist, wird der Bearbeitungsmodus aktiviert.
Sollte Dieser bereits aktiviert sein, wird er deaktiviert.

Wenn der Schaltzustand ausgewält ist, wird dieser aktiviert oder, falls dies schon der Fall sein sollte, deaktiviert.

Wenn Aktiv ausgewählt ist, wird das gesammte Event aktiviert oder, falls dies schon der Fall sein sollte, deaktiviert.

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
static void handle_ok(struct State *state) {
	// Pointer zum aktuellen Event holen
    struct Event *event = &state->event_data[state->selected_event];

    // Welches Detail ist ausgewält
    switch (state->selected_event_detail) {
    	// Wenn Montag ausgewält ist
        case SELECTED_MONDAY:
        	// Montag Bit wird xor (flippen des Bits)
            event->weekdays ^= Monday;
            break;
        case SELECTED_TUESDAY:
            event->weekdays ^= Tuesday;
            break;
        case SELECTED_WEDNESDAY:
            event->weekdays ^= Wednesday;
            break;
        case SELECTED_THURSDAY:
            event->weekdays ^= Thursday;
            break;
        case SELECTED_FRIDAY:
            event->weekdays ^= Friday;
            break;
        case SELECTED_SATURDAY:
            event->weekdays ^= Saturday;
            break;
        case SELECTED_SUNDAY:
            event->weekdays ^= Sunday;
            break;
		// Bei ausgewälter Uhrzeit
        case SELECTED_HOURS:
        case SELECTED_MINUTES:
        	// Ein und auschalten des Bearbeitungsmodus
            state->menu_event_time_edit_mode = !state->menu_event_time_edit_mode;
            break;
		// Bei Schaltzustand (aus/an)
        case SELECTED_ON_OR_OFF:
        	// Negieren
            event->on_or_off = !event->on_or_off;
            break;
        case SELECTED_ENABLED:
            event->enabled = !event->enabled;
            break;
    }
    // Den Entprechenden Teil des LCDs neu Zeichnen
    event_details_menu_update_menu(state);
}


/*********************************************************************/
 /**
Diese Funktion schreibt das Events Detail Menü auf das LC-Display

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  Wenn bei Schaltzustand von aus auf an gewechselt wird, blinken drei Felder, obwohl nur zwei Felder benötigt werden.
 **********************************************************************/
void event_details_menu_draw_menu(struct State *state) {
	// Pointer zum aktuellen Event holen
    struct Event *event = &state->event_data[state->selected_event];

    //LCD löschen (Tafeldienst)
    lcd_clrscr();

    // LCD Cursor setzen
    lcd_gotoxy(1, 1);
    // Wochentage ausgeben
    // Wenn Wochentag aktiviert ist dann groß schreiben
    // Ansonsten klein
    lcd_write_string("Tag: ");
    lcd_write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
    lcd_write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
    lcd_write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
    lcd_write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
    lcd_write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
    lcd_write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
    lcd_write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));

    // Cousor setzen
    lcd_gotoxy(1, 2);
    // Uhrzeit ausgeben
    lcd_write_string("Zeit: ");
    // Zehner
    lcd_write_char(SELECTED1(SELECTED_HOURS, event->hour / 10 + '0'));
    // Einer
    lcd_write_char(SELECTED1(SELECTED_HOURS, event->hour % 10 + '0'));
    lcd_write_char(':');
    // Zehner
    lcd_write_char(SELECTED1(SELECTED_MINUTES, event->minute / 10 + '0'));
    // Einer
    lcd_write_char(SELECTED1(SELECTED_MINUTES, event->minute % 10 + '0'));

    // Cursor setzen
    lcd_gotoxy(1, 3);
    // Schaltzustend ausgeben
    lcd_write_string("Schaltzustand: ");
    lcd_write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));

    // Cursor setzen
    lcd_gotoxy(1, 4);
    // Aktivirung ausgeben
    lcd_write_string("Aktiv: ");
    lcd_write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
}


/*********************************************************************/
 /**
Diese Funktion aktualisiert das Events Details Menü.
Hierbei werden nur die Zellen neu beschrieben, die sich verändert haben

 \param state
 		Ein Pointer für das State Struct welches den Zustandspeicher für die Menüs
		und die Uhr enthält
		(Wertebereich in der Struct-Defenition)


 \return  -

 \version 30.11.2021

 \todo -
 \bug  Wenn bei Schaltzustand von aus auf an gewechselt wird, blinken drei Felder, obwohl nur zwei Felder benötigt werden.
 **********************************************************************/
void event_details_menu_update_menu(struct State *state) {
	// Pointer zum aktuell ausgewählten Event raussuchen
    struct Event *event = &state->event_data[state->selected_event];

    // Je nach ausgewälten Feld wird es zum blinken gebracht
    switch (state->selected_event_detail) {
        case SELECTED_MONDAY:
            lcd_gotoxy(6, 1);
            lcd_write_string(SELECTED2(SELECTED_MONDAY, ((event->weekdays & Monday) != 0) ? "MO" : "mo"));
            break;
        case SELECTED_TUESDAY:
            lcd_gotoxy(8, 1);
            lcd_write_string(SELECTED2(SELECTED_TUESDAY, ((event->weekdays & Tuesday) != 0) ? "DI" : "di"));
            break;
        case SELECTED_WEDNESDAY:
            lcd_gotoxy(10, 1);
            lcd_write_string(SELECTED2(SELECTED_WEDNESDAY, ((event->weekdays & Wednesday) != 0) ? "MI" : "mi"));
            break;
        case SELECTED_THURSDAY:
            lcd_gotoxy(12, 1);
            lcd_write_string(SELECTED2(SELECTED_THURSDAY, ((event->weekdays & Thursday) != 0) ? "DO" : "do"));
            break;
        case SELECTED_FRIDAY:
            lcd_gotoxy(14, 1);
            lcd_write_string(SELECTED2(SELECTED_FRIDAY, ((event->weekdays & Friday) != 0) ? "FR" : "fr"));
            break;
        case SELECTED_SATURDAY:
            lcd_gotoxy(16, 1);
            lcd_write_string(SELECTED2(SELECTED_SATURDAY, ((event->weekdays & Saturday) != 0) ? "SA" : "sa"));
            break;
        case SELECTED_SUNDAY:
            lcd_gotoxy(18, 1);
            lcd_write_string(SELECTED2(SELECTED_SUNDAY, ((event->weekdays & Sunday) != 0) ? "SO" : "so"));
            break;
        case SELECTED_HOURS:
            lcd_gotoxy(7, 2);
            lcd_write_char(SELECTED1(SELECTED_HOURS, event->hour / 10 + '0'));
            lcd_write_char(SELECTED1(SELECTED_HOURS, event->hour % 10 + '0'));
            break;
        case SELECTED_MINUTES:
            lcd_gotoxy(10, 2);
            lcd_write_char(SELECTED1(SELECTED_MINUTES, event->minute / 10 + '0'));
            lcd_write_char(SELECTED1(SELECTED_MINUTES, event->minute % 10 + '0'));
            break;
        case SELECTED_ON_OR_OFF:
            lcd_gotoxy(16, 3);
            lcd_write_string(SELECTED3(SELECTED_ON_OR_OFF, event->on_or_off ? " an" : "aus"));
            break;
        case SELECTED_ENABLED:
            lcd_gotoxy(8, 4);
            lcd_write_string(SELECTED4(SELECTED_ENABLED, event->enabled ? "  ja" : "nein"));
            break;
    }
}
