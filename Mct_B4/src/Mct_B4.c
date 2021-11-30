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
 *  \mainpage B4: Schaltuhr und Thermometer
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
 *  \subsection sec1_2 Teilaufgabe 2
 *  Programmieren Sie einen Treiber für den vollständigen Zugriff auf die TM1638-Platine, die sich
 *  auf dem beiliegenden I/O-Board befindet. Es sollen alle 8 LEDs und die achtstellige
 *  Siebensegmentanzeige (incl. Dezimalpunkten) ansteuerbar sein und alle 8 Taster ausgelesen
 *  werden können.
 *
 *  \subsection sec1_3 Teilaufgabe 3
 *  Programmieren Sie, basierend auf den oben genannten Treibern, eine ergonomisch und intuitiv
 *  bedienbare Schaltuhr mit zusätzlicher Temperaturanzeige.
 *  Diese soll über folgende Leitungsmerkmale verfügen:
 *  - aktuelle Uhrzeit einstellbar
 *  - bis zu 16 Schalt-Events programmierbar
 *  - für jedes Schalt-Event sind folgende Merkmale einzeln einstellbar:
 *  - Stunde und Minute
 *  - Wochentag
 *  - Wiederholung für jeden einzelnen Wochentag getrennt ein-/ausschaltbar
 *  - Ein- oder Ausschaltvorgang
 *  - die Anzahl der aktuell programmierten Events wird durch rote LEDs am Portexpander
 *  PCA9539 dargestellt. ( Anzahl leuchtender LEDs von links = Anzahl Events).
 *  - Der Einschaltzustand wird durch die RGB-LED (grün : EIN, rot : AUS) signaliert
 *  - Die Programmierung und Darstellung von Uhrzeit, Datum und Wochentag geschieht
 *  primär über das TM1638-Ein-/Ausgabemodul
 *  - Die Temperaturinformation sowie der Status der Schaltuhr (Anzahl programmierter
 *  Events, derzeitiger Schaltzustand) werden auf dem LCD dargestellt.
 *  - Über das LCD soll, nach entsprechender Anforderung, eine Liste aller programmierten
 *  Events darstellbar und editierbar sein. Die Bedienung dieser Funktion geschieht über die
 *  Taster T1 .. T4, eine Rückmeldung erfolgt über die LEDs LED1 .. LED4.
 *
 *	<HR>
 *
 *  \section sec2 Eingebundene Dateien
 *
 *  \subsection sec2_1 Clock
 *	Logik für die Uhrzeit auf dem LEDKey und die Buttons.
 *
 *  \subsection sec2_2 Events
 *	Funktionen für Events
 *
 *  \subsection sec2_3 FrontIO
 *  Input Output Frontplatte
 *
 *  \subsection sec2_4 GPIO
 *  Funktionen und Structe zur Interaktion mit GPIO
 *
 *  \subsection sec2_5 I2C
 *	I2C Treiber
 *
 *  \subsection sec2_6 I2CLED
 *	Ausgabe der Evenanzahl über LEDs
 *
 *  \subsection sec2_7 LEDKey
 *  Anzeigen der Uhrzeit und Datum + Auslesen der Taster
 *
 *  \subsection sec2_8 Menu
 *	Anzeigen des Menüs und reagieren auf Tastendrücke
 *
 *  \subsubsection sec2_9_1  - MainMenu
 *	Anzeigen des Hauptmenüs und reagieren auf Tastendrücke
 *  \subsubsection sec2_9_2  - EventsMenu
 *	 Anzeigen des Eventmenüs und reagieren auf Tastendrücke
 *  \subsubsection sec2_9_3  - EventDetailsMenu
 *  Anzeigen des EventDetailsMenüs und reagieren auf Tastendrücke
 *
 *  \subsection sec2_10 RGBLED
 *  RGB LED Treiber
 *
 *  \subsection sec2_11 RTC
 *  Lesen und schreiben von Datum, Uhrzeit und Temperatur
 *
 *  \subsection sec2_12 Serial
 *  Custom Serial Protokoll Treiber für TM1638
 *
 *  \subsection sec2_13 State
 *  Struct definitionen für den aktuellen Zustand des Menüs
 *
 *  \subsection sec2_14 Timer
 *  Timer Treiber
 *
 *
 *  <HR>
 *
 *   \author	Markus Luckau, Simon Struck
 *   \date		28.11.2021
 *   \version 	1.0             Fertige Version
 */


void init_state(struct State *state, struct Event *event_data) {
	// Startwerte für das Menü festsetzen
    state->blink = false;
    state->menu_screen = SCREEN_MAIN_MENU;
    state->menu_last_buttons = 0;
    state->selected_event = 0;
    state->selected_event_detail = 0;
    state->menu_event_time_edit_mode = false;
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

	// Schleife die Durch alle Events geht
    for (int i = 0; i < EVENT_COUNT; ++i) {
    	// Pointer zum aktuellen Event erstellen
        struct Event *event = &state->event_data[i];
        // Wenn Event aktiviert ist,
        // wenn Stunden und Minuten passen,
        // und der Wochentag passt
        if (event->enabled &&
            event->hour == state->time.hours &&
            event->minute == state->time.minutes &&
            ((event->weekdays & (1 << (state->time.weekday-1))) != 0)) {
        	// Zustand der RGB-LED vom Event übernehmen
            state->rgb_state = event->on_or_off;
            // Wenn event an
            if (event->on_or_off) {
            	// RGB-Led Grün
                rgbled_set_green();
            } else {
            	// Ansonsten Rot schalten
                rgbled_set_red();
            }
        }
    }

}

void check_time_change(struct State *state) {
	// Speicherplatz für die Zeit Reservieren
    struct DateTime new_time;
    // Zeit von der RTC lesen
    rtc_read_time(&new_time);
    // Wenn Zeit sich geändert hat
    if (compare_times(&state->time, &new_time)) {
    	// Wenn sich die Minuten geändert haben
        if (state->time.minutes != new_time.minutes) {
        	// Neue Zeit abspeichern
        	state->time = new_time;
        	// Prüfen ob Event auslösst
        	check_events(state);
        }
        state->time = new_time;
        state->clock_should_redraw = true;
    }
}

void check_temperature_change(struct State *state) {
	// neue Tempeartur einlesen
    uint16_t new_temperature = rtc_read_temp();
    // Wenn die Temp sich verändert hat
    if (new_temperature != state->temperature) {
    	// Temp Aktualisiern
        state->temperature = new_temperature;
        // Menü sagen das es sich aktualisieren soll
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

    rgbled_init();
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
