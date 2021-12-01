#include "RGBLED.h"

/**
 *  \file     RGBLED.c
*/


/*********************************************************************/
 /**
  \brief Diese Funktion Initialisiert die benötigten GPIO Pins für die RGB-LED.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void rgbled_init() {
	// Initialisierung der GPIO PINS für RGB LED
    gpio_init_pin(&rgbled_red);
    gpio_init_pin(&rgbled_green);
    gpio_init_pin(&rgbled_blue);

    rgbled_set_red();
}


/*********************************************************************/
 /**
  \brief Diese Funktion schaltet die RGB-LED grün.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void rgbled_set_green() {
	// Rot aus
    gpio_set_high(&rgbled_red);
    // Blau aus
    gpio_set_high(&rgbled_blue);
    // Grün an
    gpio_set_low(&rgbled_green);
}

/*********************************************************************/
 /**
  \brief Diese Funktion schaltet die RGB-LED rot.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void rgbled_set_red() {
	// Grün aus
    gpio_set_high(&rgbled_green);
    // Blau aus
    gpio_set_high(&rgbled_blue);
    // Rot an
    gpio_set_low(&rgbled_red);
}


/*********************************************************************/
 /**
  \brief Diese Funktion schaltet die RGB-LED blau.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void rgbled_set_blue() {
    gpio_set_high(&rgbled_green);
    gpio_set_high(&rgbled_red);
    gpio_set_low(&rgbled_blue);
}


/*********************************************************************/
 /**
  \brief Diese Funktion schaltet die RGB-LED aus.

 \param	-

 \return  -

 \version 30.11.2021

 \todo -
 \bug  keine Fehler bekannt
 **********************************************************************/
void rgbled_off() {
    gpio_set_high(&rgbled_green);
    gpio_set_high(&rgbled_red);
    gpio_set_high(&rgbled_blue);
}

/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für die rote Komponente der RGB-LED.

\version 30.11.2021
**********************************************************************/
const struct GPIOPin rgbled_red = {
        .port = 0,
        .pin = 22,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};


/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für die grüne Komponente der RGB-LED.

\version 30.11.2021
**********************************************************************/
const struct GPIOPin rgbled_green = {
        .port = 3,
        .pin = 25,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};


/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für die blaue Komponente der RGB-LED.

\version 30.11.2021
**********************************************************************/
const struct GPIOPin rgbled_blue = {
        .port = 3,
        .pin = 26,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};
