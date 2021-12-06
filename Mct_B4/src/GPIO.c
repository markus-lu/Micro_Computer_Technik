#include "GPIO.h"
#include "LPC17xx.h"

/**
 *  \file     GPIO.c
*/

/*********************************************************************/
/**
 \brief Diese Funktion setzt das LPC_PINCON->PINSEL Register.
In diesem Register wird die Pin-Funktion eingestellt.
Alle Pins, die in diesem Projekt verwendet werden, werden als GPIO genutzt.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set_pin_sel(const struct GPIOPin *pin) {
	// Adresse vom PINSEL-Register für Port 0
	volatile uint32_t *pinsel = &LPC_PINCON->PINSEL0;
    // Um auf PINSEL2 zu kommen, plus Port mal 2
	pinsel += pin->port * 2;

	// wenn Untere hälfte benutzt wir (pin 0 - 15)
    if (pin->pin < 16) {
    	// GPIO Modus an die Bits für den Pin schreiben
        *pinsel |= (MODE_GPIO << (pin->pin * 2));
    } else {
    	// wenn 16 - 31
    	// ein Pinselregister weiterspringen
        pinsel += 1;
    	// GPIO Modus an die Bits für den Pin schreiben
        *pinsel |= (MODE_GPIO << ((pin->pin - 16) * 2));
    }
}

/*********************************************************************/
/**
 \brief Diese Funktion setzt das LPC_PINCON->PINMODE Register.
In diesem Register wird eingestellt, ob ein Pullup-Widerstand,
ein Pulldown-Widerstand oder die Repeater-Funktion aktiviert werden soll.
Diese Information ist im GPIOPin Struct enthalten.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set_pin_mode(const struct GPIOPin *pin) {
	// Rausuchen des Richtigen Pinmodus
	volatile uint32_t *pinmode = &LPC_PINCON->PINMODE0;
	// Um auf PINMODE2 zu kommen plus port mal 2
    pinmode += pin->port * 2;

    // wenn Untere hälfte benutzt wir (pin 0 - 15)
    if (pin->pin < 16) {
    	// GPIO Modus an die Bits für den Pin schreiben
    	// Mögliche Pull-Ups oder Pull-Downs defenieren
        *pinmode |= (pin->mode << (pin->pin * 2));
    } else {
    	// wenn 16 - 31
    	// ein Pinmoderegister weiterspringen
        pinmode += 1;
        // GPIO Modus an die Bits für den Pin schreiben
        // Mögliche Pull-Ups oder Pull-Downs defenieren
        *pinmode |= (pin->mode << ((pin->pin - 16) * 2));
    }
}

/*********************************************************************/
/**
 \brief Diese Funktion setzt das LPC_PINCON->PINMODE_OD0 Register.
In diesem Register wird der Open-Drain-Modus eingestellt.
Diese Information ist im GPIOPin Struct enthalten.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set_pin_open_drain(const struct GPIOPin *pin) {
	// Rausuchen des Pinmode für Port 0
    volatile uint32_t *pinmode_od = &LPC_PINCON->PINMODE_OD0;
    // Um zum Richtigen Port zu kommen Port-offset hinzuaddieren
    pinmode_od += pin->port;

    // Opendrain Pin Setzen oder nicht setzetn
    *pinmode_od |= (pin->open_drain << (pin->pin));
}

/*********************************************************************/
/**
 \brief Diese Funktion ist eine Hilfsfunktion um den als Integer gespeicherten Port
zu dem korrekten Pointer zu wandeln.

Zum Beispiel würde ein Funktionsaufruf mit dem Wert 2 den Pointer LPC_GPIO2 zurück geben.

\param  port
        Port zu dem der Pointer benötigt wird.
        (0 - 4)

\return -

\version 12.11.2021

\todo -
\bug  Bei Überschreitung des Wertebereichs werden invalide Pointer zurückgegeben.
**********************************************************************/
static LPC_GPIO_TypeDef *get_gpio_port(uint8_t port) {
	// GPIO Basisadresse holen
	LPC_GPIO_TypeDef *ptr = (LPC_GPIO_TypeDef *) LPC_GPIO_BASE;
	// Addition des Ports um zum richtigen GPIO Struckt zu Springen
    ptr += port;
    // Rückgabe der Portadresse
    return ptr;
}

/*********************************************************************/
/**
 \brief Diese Funktion (re-)initialisiert einen GPIOPin.

Diese Funktion nutzt Hilfsfunktionen um die folgenden Aktionen durchzuführen:
 1. Pinfunktion festlegen (PIN function SELect, PINSEL)
 2. Eingangsmodus von Portbits einstellen (PIN MODE select, PINMODE)
 3. OpenDrain von Portbits einstellen (PIN MODE select, PINMODE_OD)
 4. Fast gpIO port DIRection control register (FIODIR)
 5. Fast MASK register for ports (FIOMASK)


\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_init_pin(const struct GPIOPin *pin) {
	// Initialisierung GPIO
	// Pin als GPIO verwenden
	gpio_set_pin_sel(pin);
	// Pull up / Pull Down
    gpio_set_pin_mode(pin);
    // Open Drain
    gpio_set_pin_open_drain(pin);

    //Rausuchen der Richtigen GPIO Adresse für den Port
    volatile LPC_GPIO_TypeDef *gpio = get_gpio_port(pin->port);

    // wenn Ausgang
    if (pin->dir == OUTPUT) {
    	// FIODIR-Bit Setzen
        gpio->FIODIR |= (1 << pin->pin);
    } else {
    	// FIODIR-Bit Löschen (Eingang)
        gpio->FIODIR &= ~(1 << pin->pin);
    }
    // Pin in GPIO-Maske freigeben
    gpio->FIOMASK &= ~(1 << pin->pin);
}

/*********************************************************************/
/**
 \brief Diese Funktion setzt den Ausgang eines GPIOPin's auf HIGH.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set_high(const struct GPIOPin *pin) {
	// Pin mit dem Register FIOSET einschalten
	// Nullen werden Ignoriert
    get_gpio_port(pin->port)->FIOSET = (1 << pin->pin);
}

/*********************************************************************/
/**
 \brief Diese Funktion setzt den Ausgang eines GPIOPin's auf LOW.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set_low(const struct GPIOPin *pin) {
	// Pin mit dem Register FIOCLR ausschalten
	// Nullen werden Ignoriert
    get_gpio_port(pin->port)->FIOCLR = (1 << pin->pin);
}

/*********************************************************************/
/**
 \brief Diese Funktion setzt den Ausgang eines GPIOPin's auf HIGH oder LOW,
abhängig von dem Parameter state.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)
\param  state
        Steuert, ob der Pin auf HIGH oder LOW gesetzt wird.
        (0 = LOW, 1 = HIGH)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void gpio_set(const struct GPIOPin *pin, bool state) {
	// Setzt high oder low abhängig vom bool state
    if (state) {
        gpio_set_high(pin);
    } else {
        gpio_set_low(pin);
    }
}

/*********************************************************************/
/**
 \brief Diese Funktion liest einen GPIOPin aus und gibt zurück,
ob an diesem Pin ein HIGH oder LOW Pegel anliegt.

\param  pin
        Das struct GPIOPin enthält die Einstellungen für diesen Pin.
        (Wertebereich in der Struct-Definition)

\return Den Zustand des Pins
        (0 = LOW, 1 = HIGH)

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
bool gpio_get(const struct GPIOPin *pin) {
	// FIOPIN-Register für diesen GPIO-Port auslesen
    volatile uint32_t fiopin = get_gpio_port(pin->port)->FIOPIN;
    // Filtern des Ports zum herausfinden des benötigten Pins
    uint32_t masked_pin = fiopin & (1 << pin->pin);
    // Gibt ein Wahr Zurück wen der gesuchte Pin High war
    return masked_pin != 0;
}
