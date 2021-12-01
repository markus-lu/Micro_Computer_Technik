/*
 ===============================================================================
 Name        : State.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Struct definitionen für den aktuellen Zustand des Menüs
 ===============================================================================
 */

#ifndef _STATE_H_
#define _STATE_H_

#include "types.h"
#include "RTC.h"

/**
 *  \file     State.h
*/

/**
 * \brief Dieses Struct enthält den Zustand des Menüs und der Uhr.
 */
struct State {
    /**
     * \brief Steuert das Blinken des ausgewählten Feldes im Menü und der Uhr.
     *
     * Wird alle 500 ms geflippt.
     * Wenn true, dann werden im Menü und in der Uhr das ausgewählte Feld nicht angezeigt, um zu signalisieren,
     * dass sich der Cursor in diesem Feld befindet.
     * Wenn false, dann wird alles normal angezeigt.
     *
     * Wertebereich: false / true
     */
    bool blink;
    /**
     * \brief Steuert, welches Menü gerade auf dem LCD angezeigt werden soll.
     *
     * Das Ändern dieser Variable alleine reicht aber nicht,
     * um das angezeigte Menü zu wechseln.
     * Hierzu muss zusätzlich das Feld menu_should_redraw auf true gesetzt werden.
     *
     * Wertebereich:
     *  - SCREEN_MAIN_MENU
     *  - SCREEN_EVENTS_MENU
     *  - SCREEN_EVENT_DETAILS_MENU
     */
    uint32_t menu_screen;
    /**
     * \brief Speichert die zuletzt gedrückten Taster an der Frontplatte.
     *
     * Dieses Feld wird verwendet, damit nur auf veränderte Tastendrücke reagiert wird.
     * Jedes Mal, wenn auf einen Tastendruck geprüft wird,
     * wird auch verglichen, ob sich die gedrückten Tasten von diesem Wert unterscheiden.
     * Danach wird der neue Tastendruck hier gespeichert.
     *
     * Wertebereich: 0x00 - 0x0F (ein Bit pro Taster)
     */
    uint8_t menu_last_buttons;
    /**
     * \brief Speichert welches Event ausgewählt ist.
     *
     * Dies wird sowohl vom EventsMenu für den Cursor,
     * als auch vom EventDetailsMenu zum anzeigen der Event Details verwendet.
     *
     * Wertebereich: 0 - (EVENT_COUNT - 1)
     */
    uint8_t selected_event;
    /**
     * \brief Speichert den Cursor im EventDetailsMenu.
     *
     * Wird mit den Tastern Hoch und Runter inkrementiert und dekrementiert.
     *
     * Wertebereich: SELECTED_MONDAY (0) - SELECTED_ENABLED (10)
     *               Siehe EventDetailsMenu.h für Konstanten
     */
    uint8_t selected_event_detail;
    /**
     * \brief Steuert, ob dein Tastendruck auf die Tasten Hoch oder Runter
     * die Stunden / Minuten verändern oder das nächste Detail selektieren.
     *
     * Wenn true, dann wird das Event Detail bei einem Tastendruck verändert.
     *
     * Wenn false, dann wird zum nächsten oder vorherigen Event Detail bei einem Tastendruck gesprungen.
     *
     * Wertebereich: false / true
     */
    bool menu_event_time_edit_mode;
    /**
     * \brief Pointer auf ein Array, welches die programmierbaren Events enthält.
     *
     * Die Länge des Arrays wird durch die Konstante EVENT_COUNT kontrolliert.
     *
     * Wertebereich: Siehe Struct Event
     */
    struct Event *event_data;
    /**
     * \brief Kontrolliert, ob das Menü komplett neu zeichnen soll.
     *
     * Wird bei Menüwechseln auf true gesetzt.<br>
     * Wird bei Temperaturveränderungen auf true gesetzt.<br>
     * Wird nach dem neu zeichnen des Menüs auf false gesetzt.
     *
     * Wertebereich: false / true
     */
    bool menu_should_redraw;
    /**
     * \brief Speichert den aktuellen Zustand der RGBLED, damit Dieser im MainMenu ausgegeben werden kann.
     *
     * Wird beim Schalten der RGBLED beschrieben.
     *
     * Wertebereich: false (rot) / true (grün)
     */
    bool rgb_state;
    /**
     * \brief Enthält die Temperatur zur Anzeige im MainMenu.
     *
     *  Oberes Byte enthält ganzzahlige Gradzahl.
     *  Unteres Byte enthält 2 bits, die die Kommastelle angeben (in 0.25 Grad Schritten).
     *
     *  Wertebereich:
     *      Oberes Byte: -128 - +128 Grad
     *      Unteres Byte: 0 - 3 (repräsentiert 0, 0.25, 0.5, 0.75 Grad)
     */
    uint16_t temperature;
    /**
     * \brief Enthält die aktuelle Zeit.
     *
     * Wird von der RTC eingelesen und von der Uhr zur Anzeige verwendet.
     *
     * Wertebereich: Siehe Struct DateTime in RTC.h
     */
    struct DateTime time;
    /**
     * \brief Speichert den Cursor der Uhr.
     *
     * Wird mit den Tastern am LED&KEY inkrementiert und dekrementiert.
     *
     * Wertebereich: SELECTED_HOUR (1 << 0) - SELECTED_WEEKDAY (1 << 6)
     */
    uint8_t clock_selected_field;
    /**
     * \brief Steuert, ob die Uhrzeit / Datum gerade editiert wird.
     *
     * Wenn true, blinkt das aktuell ausgewählte Feld auf dem LED&KEY.<br>
     * Ein Tastendruck inkrementiert oder dekrementiert das ausgewählte Feld.<br>
     * Hierbei wird die Uhrzeit nicht mehr von der RTC gelesen,
     * da dies das Bearbeiten nicht möglich machen würde.
     *
     * Wenn false, wird die Uhrzeit regelmäßig von der RTC gelesen und angezeigt.
     *
     * Beim Wechsel von true auf false wird die eingestellte Zeit auf die RTC geschrieben.
     *
     * Wertebereich: false / true
     */
    bool clock_edit_mode;
    /**
     * \brief Kontrolliert, ob die Uhrzeit oder das Datum neu gezeichnet werden soll.
     *
     * Wird von Tastendrücken auf dem LED&KEY auf true gesetzt.<br>
     * Wird von einer veränderten Uhrzeit auf true gesetzt.<br>
     * Wird nach dem Neuzeichnen des LED&KEY auf false gesetzt.
     *
     * Wertebereich: false / true
     */
    bool clock_should_redraw;
    /**
     * \brief Kontrolliert, ob das LED&KEY die Uhrzeit oder das Datum anzeigen soll.
     *
     * Wenn true, wird die Uhrzeit angezeigt.<br>
     * Wenn false, wird das Datum angezeigt.
     *
     * Wertebereich: false / true
     */
    bool clock_show_time;
    /**
     * \brief Speichert die zuletzt gedrückten Taster am LED&KEY.
     *
     * Dieses Feld wird verwendet, damit nur auf veränderte Tastendrücke reagiert wird.
     * Jedes Mal, wenn auf Tastendruck geprüft wird,
     * wird auch verglichen ob die gedrückten Tasten von diesem Wert unterscheiden.
     * Danach wird der neue Tastendruck hier gespeichert.
     *
     * Wertebereich: 0x00 - 0xFF (ein Bit pro Taster)
     */
    uint8_t clock_last_buttons;
    /**
     * \brief Speichert die Helligkeit des 7-Segment-Displays des LED&KEY.
     *
     * -1 wird daher verwendet, weil die Helligkeit des LED&KEY von 0 - 7 kodiert wird.
     * Ob das Display an oder aus ist, wird durch ein weiteres Bit kodiert.
     *
     * Da hier nur eine Variable zur Kodierung der Helligkeit des Displays verwendet wird,
     * repräsentiert -1 ein ausgeschaltetes Display und die Werte 0 - 7 repräsentieren somit weiterhin die Helligkeitsstufen.
     *
     * Wertebereich: -1 (Display aus) - 7 (volle Helligkeit)
     */
    int8_t clock_brightness;
};

bool times_are_different(struct DateTime *old_time, struct DateTime *new_time);

#endif //_STATE_H_
