/*
 ===============================================================================
 Name        : RTC.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Lesen und schreiben von Datum, Uhrzeit und Temperatur
 ===============================================================================
 */

#ifndef _RTC_H_
#define _RTC_H_

#include "types.h"
#include "I2C.h"

/**
 *  \file    RTC.h
*/


/**
 *  \brief Minimalwert für Wochentage
 */
#define MONDAY 1

/**
 *  \brief Maximalwert für Wochentage
 */
#define SUNDAY 7



/**
 *\brief Struct zum abspeichern der Uhrzeit und des Datum
 */
struct DateTime {
	/**
	 *\brief Integer der die aktuellen Sekunden enthält
	 *
	 *Wertebereich: 0 - 59
	 */
    uint8_t seconds;
    /**
	 *\brief Integer der die aktuellen Minuten enthält
	 *
	 *Wertebereich: 0 - 59
	 */
    uint8_t minutes;
    /**
	 *\brief Integer der die aktuellen Stunden enthält
	 *
	 *Wertebereich: 0 - 23
	 */
    uint8_t hours;
    /**
	 *\brief Integer der den aktuellen Wochentag enthält
	 * 1 = Montag,...,7 = Sontag
	 *
	 *Wertebereich: 1 - 7
	 */
    uint8_t weekday;
    /**
   	 *\brief Integer der den aktuellen Tag enthält
   	 *
   	 *Wertebereich: 1 - 31
   	 */
    uint8_t day;
    /**
	 *\brief Integer der den aktuellen Monat enthält
	 *
	 *Wertebereich: 1 - 12
	 */
    uint8_t month;
    /**
	 *\brief Bool der das aktuelle Jahundert enthält
	 * true = 2000 false = 1900
	 *
	 *Wertebereich: true / false
	 */
    bool century;
    /**
	 *\brief Integer der das aktuelle Jahr enthält
	 *
	 *Wertebereich: 0 - 99
	 */
    uint8_t year;
};

void rtc_init(void);

uint16_t rtc_read_temp(void);

void rtc_read_time(struct DateTime *);

void rtc_write_time(struct DateTime *);

#endif
