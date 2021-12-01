/*
 ===============================================================================
 Name        : Events.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Funktionen für Events
 ===============================================================================
 */

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "types.h"

/**
 *  \file    Events.h
*/

/**
 * Anzahl der möglichen Events
 */
#define EVENT_COUNT 16

/**
 * Bitcodieung für Montag
 */
#define Monday (1 << 6)

/**
 * Bitcodieung für Dienstag
 */
#define Tuesday (1 << 5)

/**
 * Bitcodieung für Mitwoch
 */
#define Wednesday (1 << 4)

/**
 * Bitcodieung für Donerstag
 */
#define Thursday (1 << 3)

/**
 * Bitcodieung für Freitag
 */
#define Friday (1 << 2)

/**
 * Bitcodieung für Samstag
 */
#define Saturday (1 << 1)

/**
 * Bitcodieung für Sontag
 */
#define Sunday (1 << 0)


/**
 * \brief Ein Event Struct in welchem alle wichtigen Daten für ein Event gespeichert wird.
 */
struct Event {
	/**
	 * \brief Bool der anzeigt ob ein Event aktiviert (true) oder deaktiviert (false) ist.
	 *
	 * Wertebereich: false / true
	 */
    bool enabled;
    /**
     * \brief Integer der den Stundenanteil des Events enthält
     *
     * Wertebereich: 0 - 23
     */
    uint8_t hour;
    /**
	 * \brief Integer der den Minutenanteil des Events enthält
	 *
	 * Wertebereich: 0 - 59
	 */
    uint8_t minute;
    /**
	 * \brief Integer der den Wochentag des Events enthält
	 * MSB = Monday, ..., Sunday, LSB = 0
	 *
	 * Wertebereich: 0x80 - 0x02
	 */
    uint8_t weekdays;
    /**
     * \brief Bool der anzeigt ob die RGB_LED rot (off) oder grün (on) leuchten soll
     * true = on, false = off
     *
     * Wertebereich:  false / true
     */
    bool on_or_off;
};

void events_init(struct Event *event_data);

uint16_t events_get_count(const struct Event *event_data);

#endif
