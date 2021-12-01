/*
 ===============================================================================
 Name        : Menu.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen des Menüs und reagieren auf Tastendrücke
 ===============================================================================
 */

#ifndef _MENU_H_
#define _MENU_H_

#include "types.h"
#include "State.h"

/**
 *  \file     Menu.h
*/

/**
 *  \brief LC-Display besitzt vier Zeilen
 */
#define LCD_LINES 4

/**
 *  \brief Bitwert für den Zurück-Taster
 */
#define BUTTON_BACK (1 << 0)

/**
 *  \brief Bitwert für den Runter-Taster
 */
#define BUTTON_DOWN (1 << 1)

/**
 *  \brief Bitwert für den Hoch-Taster
 */
#define BUTTON_UP (1 << 2)

/**
 *  \brief Bitwert für den OK-Taster
 */
#define BUTTON_OK (1 << 3)

/**
 *  \brief Wert für das Main-Menü
 */
#define SCREEN_MAIN_MENU 0

/**
 *  \brief Wert für das Events-Menü
 */
#define SCREEN_EVENTS_MENU 1

/**
 *  \brief Wert für das Events-Details-Menü
 */
#define SCREEN_EVENT_DETAILS_MENU 2

void menu_init();

void menu_loop_once(struct State *state);

#endif
