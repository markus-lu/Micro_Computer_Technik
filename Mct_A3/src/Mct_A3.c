/*
 ===============================================================================
 Name        : Mct_A3.c
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Rechenmaschine
 ===============================================================================
 */

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "Mct_A3.h"

/**
 *  \file     Mct_A3.c
*/

/**
 *  \mainpage A3: Rechenmaschine
 *
 *  Dies ist unsere Lösung zu Aufgabe A3: Rechenmaschine.<BR><BR>
 *  Dieses Programm multipliziert zwei Zahlen miteinander,
 *  die über Schalter binär angegeben werden.<BR>
 *
 *  <HR>
 *
 *  \section sec1 Aufgabenstellung
 *  Programmierung einer einfachen Rechenmaschine in der Programmiersprache C
 *  auf dem Mikrocontroller-Entwicklungssystem MCTlite.<BR>
 *  Nach dem Programmstart soll zunächst mittels des Tasters TA1 eine an den
 *  Eingängen E0 .. E3 angelegte Binärzahl als Multiplikator A eingelesen werden.<BR>
 *  Danach wird Multiplikand B an E0 .. E3 angelegt und
 *  über den Taster TA2 eingelesen.<BR>
 *  Im Anschluss daran wird das Produkt C = A * B gebildet und
 *  auf den 8 gelben LEDs ausgegeben.<BR>
 *  Diese LEDs können, optional, durch den Taster TA3 gelöscht werden.<BR>
 *  Danach beginnt das Programm wieder von vorn.<BR>
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


/*********************************************************************/
/**
 *
 * \brief Dies ist das Hauptprogramm

Dieses Programm multipliziert zwei Zahlen miteinander, die über Schalter Binär angegeben werden

\param   -

\return  wird nie erreicht

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
int main(void) {
    uint32_t keys = 0; // Tastencodes
    uint32_t Faktor1 = 0; // Multiplikator
    uint32_t Faktor2 = 0; // Multiplikand
    uint32_t Produkt = 0; // Produkt der Multiplikation
    uint32_t valuesAvailable = A_B_NOT_AVAILABLE; // welche Werte wurden eingelesen ?

    io_init(); // I/O-Ports initialisieren
    show_result(Produkt); // anfangs alle LEDs aus

    while (1) {
        keys = getkey(); // Taster auslesen

        keyhandler(keys, &Faktor1, &Faktor2, &Produkt, &valuesAvailable); // einlesen + berechnen

        if ((valuesAvailable == A_B_AVAILABLE)
            || (valuesAvailable == A_B_NOT_AVAILABLE)) {
            show_result(Produkt); // Ergebnis anzeigen
        }
    }
    return 0;
}


/*********************************************************************/
/**
Funktion zur Initialisierung des verwendeten GPIO-Ports

\param   -

\return  -

\version 25.10.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void io_init(void) {

    // Eingang Pins

    // Nutze als GPIO
    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    // Nutze als Eingang
    LPC_PINCON->PINMODE0 = 0;
    LPC_PINCON->PINMODE1 = 0;

    // Nutze nur einige Pins
    LPC_GPIO0->FIOMASK = ~ALL_INPUTS;

    // Pins als Eingang
    LPC_GPIO0->FIODIR = ~ALL_INPUTS;

    // Ausgangs Pins

    // Nutze als GPIO
    LPC_PINCON->PINSEL4 = 0;

    // Nutze nur Leds
    LPC_GPIO2->FIOMASK = ~LEDS;

    // Pins als Ausgang
    LPC_GPIO2->FIODIR = LEDS;

}


/*********************************************************************/
/**
Funktion zum anzeigen des Rechenergebnisses mittels Output LEDs

\param   res
		 Diese Variable beinhaltet das Rechenergebnis
		 (0x0 - 0xFF)

\return  -

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
void show_result(uint32_t res) {
    LPC_GPIO2->FIOPIN = ~(res << 1);
}


/*********************************************************************/
/**
Funktion zum Einlesen der gedrückten  Schalter und Taster

\param   -

\return  output
         beinhaltet eine Bitreihenfplge für alle Schalter und Taster
         (0x0 - 0x7F)

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
uint32_t getkey(void) {

    uint32_t input = ~LPC_GPIO0->FIOPIN;

    //printf("Input: %X \n", input);

    // Bits die graucht werden
    // TA1 -> P0.6
    // Ta2 -> P0.7
    // Ta3 -> P0.8
    // E0 -> P0.2
    // E1 -> P0.3
    // E2 -> P0.21
    // E3 -> P0.16

    uint32_t buttons = (input & (TA1 | TA2 | TA3)) >> 6;

    uint32_t switches = 0;
    switches |= (input & E0) << 1;
    switches |= (input & E1) << 1;
    switches |= (input & E2) >> 16;
    switches |= (input & E3) >> 10;
    uint32_t output = switches | buttons;

    //printf("Output: %X \n", output);
    return output;

}


/*********************************************************************/
/**
Funktion zur Erzeugung einer variablen Verzögerungszeit

\param   dtime
		 Diese Variable legt die Verzögerungszeit fest

\return  -

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
void delay(uint32_t dtime) {
    volatile uint32_t i;
    for (i = 0; i < dtime; i++) {
        __asm("nop");
    }
}


/*********************************************************************/
/**
Funktion zum Speichern der Multiplikanten und Berechnen des Produkts

\param   A
		 Hier wird der erste Multiplikant gespeichert,
		 sobald der Taster gedrueckt wird.
		 (0 - 0xF)

\param   B
		 Hier wird der zweite Multiplikant gespeichert,
		 sobald der Taster gedrueckt wird.
		 (0 - 0xF)

\param   C
		 Hier wird das Produkt gespeichert,
		 sobald beide Multiplikanten gespeichert wurden.
		 (0 - 0xFF)

\param   valuesAvailable
		 Hier wird eine 2-Bit Flagge gespeichert,
		 die anzeigt, welche Multiplikanten schon gespeichert wurden.
		 (Bit 0 gesetzt: Multiplikant A gespeichert,
		  Bit 1 gesetzt: Multiplikant B gespeichert)


\return  -

\version 25.10.2021

\todo    -
\bug     keine Fehler bekannt
**********************************************************************/
void keyhandler(uint32_t keys, uint32_t *A, uint32_t *B, uint32_t *C,
                uint32_t *valuesAvailable) {

    uint32_t input = keys >> 3;
    if (keys & 1) {
        *A = input;
        *valuesAvailable |= A_AVAILABLE;

    } else if (keys & 2) {
        *B = input;
        *valuesAvailable |= B_AVAILABLE;

    } else if (keys & 4) {
        *A = 0;
        *B = 0;
        *C = 0;
        *valuesAvailable = A_B_NOT_AVAILABLE;
    }

    if (*valuesAvailable == A_B_AVAILABLE) {
        *C = *A * *B;
    }

}

