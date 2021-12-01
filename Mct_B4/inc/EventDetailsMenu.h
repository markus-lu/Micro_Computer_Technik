/*
 ===============================================================================
 Name        : EventDetailsMenu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des EventDetailsMenüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _EVENT_DETAILS_MENU_H_
#define _EVENT_DETAILS_MENU_H_

#include "types.h"
#include "State.h"

/**
 *  \file     EventDetailsMenu.h
*/


/**
 *  \brief Fester Wert für das Event Detail Montag
 */
#define SELECTED_MONDAY 0

/**
 *  \brief Fester Wert für das Event Detail Dienstag
 */
#define SELECTED_TUESDAY 1

/**
 *  \brief Fester Wert für das Event Detail Mittwoch
 */
#define SELECTED_WEDNESDAY 2

/**
 *  \brief Fester Wert für das Event Detail Donnerstag
 */
#define SELECTED_THURSDAY 3

/**
 *  \brief Fester Wert für das Event Detail Freitag
 */
#define SELECTED_FRIDAY 4

/**
 *  \brief Fester Wert für das Event Detail Samstag
 */
#define SELECTED_SATURDAY 5

/**
 *  \brief Fester Wert für das Event Detail Sontag
 */
#define SELECTED_SUNDAY 6

/**
 *  \brief Fester Wert für das Event Detail Stunden
 */
#define SELECTED_HOURS 7

/**
 *  \brief Fester Wert für das Event Detail Minuten
 */
#define SELECTED_MINUTES 8

/**
 *  \brief Fester Wert für das Event Detail Schaltzustand
 */
#define SELECTED_ON_OR_OFF 9

/**
 *  \brief Fester Wert für das Event Detail Aktivierung
 */
#define SELECTED_ENABLED 10

void event_details_menu_handle_keypress(struct State* state, uint8_t buttons);

void event_details_menu_draw_menu(struct State *state);

void event_details_menu_update_menu(struct State *state);

#endif // _EVENT_DETAILS_MENU_H_
