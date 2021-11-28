/*
 ===============================================================================
 Name        : Mct_B4.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include "Mct_B4.h"
#include "types.h"
#include "Menu.h"
#include "Events.h"
#include "I2CLEDs.h"
#include "Clock.h"
#include "LEDKey.h"
#include "Timer.h"
#include "RGBLED.h"
#include "EventDetailsMenu.h"
#include <stdio.h>

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

/**
 *  \file     Mct_B4.c
*/

/**
 *  \mainpage B4: Rechenmaschine
 *
 *  Dies ist unsere Lösung zu Aufgabe B4: Schaltuhr und Thermometer.<BR><BR>
 *  Dieses Programm ist eine Schaltuhr und ein Thermometer. Dabei können Events abgespeichert werden,
 *  welche bei der eingestellten Uhrzeit Auslösen.
 *  Zusätzlich können Uhrzeit und Datum individuell eingestellt werden. <BR>
 *
 *  <HR>
 *
 *  \section sec1 Aufgabenstellung
 *  Diese Aufgabe besteht aus folgenden Teilaufgaben:
 *
 *  \subsection sec1_1 Teilaufgabe 1
 *  Programmieren Sie einen Treiber zum vollständigen Setzen und Auslesen des I²C-Uhrenbausteins
 *  DS3231. Es soll auf die Sekunde, Minute, Stunde, den Tag, den Monat und das Jahr sowie –
 *  separat - auf die vom Baustein zur Verfügung gestellte Temperaturinformation zugegriffen werden
 *  können. Implementieren Sie dazu ein geeignetes Time-Struct zur Kommunikation mit der Leseund der Schreibfunktion für die Echtzeituhr.
 *
 * \subsection sec1_1 Teilaufgabe 1
 * Programmieren Sie einen Treiber für den vollständigen Zugriff auf die TM1638-Platine, die sich
 * auf dem beiliegenden I/O-Board befindet. Es sollen alle 8 LEDs und die achtstellige
 * Siebensegmentanzeige (incl. Dezimalpunkten) ansteuerbar sein und alle 8 Taster ausgelesen
 * werden können.
 *
 *  \section sec2 Unterprogramme
 *
 *  \subsection sec2_1 Initialisierung
 *  Die Initialisierung des verwendeten GPIO-Ports wird in der
 *  Funktion "io_init" gemacht.
 *
 *  \subsection sec2_2 Ablaufsteuerung
 *  Dieses Programm multipliziert zwei Zahlen miteinander,
 *  die über Schalter Binär angegeben werden.<BR>
 *  Die main-Funktion kontrolliert den Programmablauf und<BR>
 *  ruft die folgenden Funktionen der Reihe nach auf.
 *
 *  \subsection sec2_3 Einlesen der Buttons und Schalter
 *  Das Einlesen der gedrückten  Schalter und Taster wird in der Funktion
 *  "getkey" gemacht.
 *
 *  \subsection sec2_4 Verarbeiten der Eingaben
 *  Funktion zum Speichern der Multiplikanten und Berechnen des Produkts wird
 *  in der Funktion "keyhandler" abgearbeitet.
 *
 *  \subsection sec2_5 Anzeigen des Produkts
 *  Die Ausgabe des Produkts auf die LEDs wird in der Funktion
 *  "show_result" gemacht.
 *
 *  \subsection sec2_6 Zeitverzögerung
 *  Zum Erzeugen einer variablen Verzögerungszeit wird die Funktion "delay"
 *  verwendet.<BR>
 *  Diese wird aber im Hauptprogramm nicht benutzt.
 *
 *
 *  <HR>
 *
 *   \author	Markus Luckau, Simon Struck
 *   \date		25.10.2021
 *   \version 	1.0             Fertige Version
 */


void init_state(struct State *state, struct Event *event_data) {
    state->blink = false;
    state->menu_screen = SCREEN_MAIN_MENU;
    state->menu_last_buttons = 0;
    state->selected_event = 0;
    state->selected_event_detail = 0;
    state->menu_edit_mode = false;
    state->event_data = event_data;
    state->menu_should_redraw = true;
    state->rgb_state = false;
    state->temperature = 0;
//    state->time;                        // wird an anderer Stelle initialisiert
    state->clock_selected_field = 0;
    state->clock_edit_mode = false;
    state->clock_should_redraw = true;
    state->clock_show_time = true;
    state->clock_last_buttons = 0;
    state->clock_brightness = 0;
}

void check_events(struct State *state){

    for (int i = 0; i < EVENT_COUNT; ++i) {
        struct Event *event = &state->event_data[i];
        if (event->enabled &&
            event->hour == state->time.hours &&
            event->minute == state->time.minutes &&
            ((event->weekdays & (1 << (state->time.weekday-1))) != 0)) {
            state->rgb_state = event->on_or_off;
            if (event->on_or_off) {
                rgbled_set_green();
            } else {
                rgbled_set_red();
            }
        }
    }

}

void check_time_change(struct State *state) {
    struct DateTime new_time;
    rtc_read_time(&new_time);
    if (compare_times(&state->time, &new_time)) {
        if (state->time.minutes != new_time.minutes) {
        	state->time = new_time;
        	check_events(state);
        }
        state->time = new_time;
        state->clock_should_redraw = true;
    }
}

void check_temperature_change(struct State *state) {
    uint16_t new_temperature = rtc_read_temp();
    if (new_temperature != state->temperature) {
        state->temperature = new_temperature;
        state->menu_should_redraw = true;
    }
}

/*********************************************************************/
/**
 *
 * \brief Dies ist das Hauptprogramm

Dieses Programm

\param   -

\return  wird nie erreicht

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
int main() {
    SystemCoreClockUpdate();
    printf("Hello World!!!\n");
    // Initialisierung
    rtc_init();
    rgbled_init();
    ledkey_init();
    menu_init();
    i2cleds_init();
    // Timer für Displayblinken
    timer_init_timer1();
    // 1 tick alle 1000 ms
    timer_set_prescaler(LPC_TIM1, SystemCoreClock / 1000);
    // Interupt und Reset alle 500 ms
    timer_enable_match_interrupt(LPC_TIM1, 1, 500);
    // timer Starten
    timer_start_timer(LPC_TIM1);
    //Speicher für Events reservieren
    struct Event event_data[EVENT_COUNT];
    //Events Initialieseiern
    events_init(event_data);
    // Speicher für Zustand reservieren
    struct State state;
    // Startwerte für Zustand Setzen
    init_state(&state, event_data);

    // Initialiesiernung Clock & Helligkeit von 7seg
    clock_init(&state);


    while (true) {
    	//wenn Timer1(500ms) ticked
        if (timer_has_timer1_ticked()) {
        	// blink an aus
            state.blink = !state.blink;
            //wenn menü im eventdetails angezeigt werden
            if (state.menu_screen == SCREEN_EVENT_DETAILS_MENU) {
            	//Aktuelles Feld Blinken lassen
                event_details_menu_update_menu(&state);
            }
            // wenn Uhr im Edit modus
            if (state.clock_edit_mode) {
            	// aktueller zustand blinken lassen
                state.clock_should_redraw = true;
            }
        }
        // Tempeatursennsor auslesen
        check_temperature_change(&state);
        // wenn nicht im Edit modus
        if (!state.clock_edit_mode) {
        	// Neue Zeit vom RTC holen
            check_time_change(&state);
        }
        // Menü verarbeitn (Tastencheck und neu Zeichnen)
        menu_loop_once(&state);
        // Uhr verarbeitn (Tastencheck und neu Zeichnen)
        clock_loop_once(&state);
        // Aktuelle Eventanzahl auf LEDs ausgeben
        i2cleds_set_leds(events_get_count(event_data));
    }
    // Timer1 stoppen und Resetten
    timer_deinit_timer1();
    // Serielles Protokoll für Uhr stoppen
    ledkey_deinit();
    // I2c Stoppen
    i2c_exit();
    return 0;
}
