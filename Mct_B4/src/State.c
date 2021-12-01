#include "State.h"

/**
 *  \file     State.c
*/

/**
 * Kleines Macro zum Vergleichen zweier Felder in zwei DateTime Structen.
 * Was das Vergleichen der Werte lesbarer macht und Schreibarbeit sowie Fehleranfälligkeit verringert.
 */
#define COMPARE(value) old_time->value != new_time->value

/*********************************************************************/
/**
Diese Funktion vergleicht zwei DateTime Structe.
Wenn diese ungleich sind, muss die Zeit auf dem Display aktualisiert werden.

\return Ob beide Structe sich unterscheiden
        (1 = Ungleich, 0 = Gleich)

\version 11.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
bool times_are_different(struct DateTime *old_time, struct DateTime *new_time) {
    return COMPARE(seconds) ||
           COMPARE(minutes) ||
           COMPARE(hours) ||
           COMPARE(weekday) ||
           COMPARE(day) ||
           COMPARE(month) ||
           COMPARE(century) ||
           COMPARE(year);
}
