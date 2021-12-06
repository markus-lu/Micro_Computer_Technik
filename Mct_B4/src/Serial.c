#include <Timer.h>
#include "Serial.h"
#include "GPIO.h"

/**
 *  \file     Serial.c
*/

/*********************************************************************/
/**
 \brief Diese Funktion initialisiert die GPIOPin's und den Timer 2 für das Custom Serial Protokoll des TM1638.

Die Frequenz des Timers wird auf das Doppelte der auf dem TM1638 angegebenen Maximalfrequenz gesetzt,
Ein Interrupt erfolgt aber erst nach zwei Takten, da wir nicht mit der Maximalfrequenz arbeiten wollen,
um ein wenig Spielraum zum Limit des Chips zu lassen.

Die GPIOPin's STB und CLK werden beide auf HIGH gesetzt außerhalb einer Übertragung.

\return -

\version 11.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void serial_init() {
	// GPIO Pins initialisieren
    gpio_init_pin(&serial_clk);
    // Data in óut
    gpio_init_pin(&serial_dio);
    // Strobe
    gpio_init_pin(&serial_stb);

    // Startwerte für GPIO-Pins setzen
    gpio_set_high(&serial_stb);
    gpio_set_high(&serial_clk);

    // Timer 2 initialisieren
    timer_init_timer2();
    // prescaler 48 mal durcklaufen
    // timer 2 ticket
    timer_set_prescaler(LPC_TIM2, SystemCoreClock / SERIAL_CLOCK);
    timer_enable_match_interrupt(LPC_TIM2, 1, 2);
    // Timer 2 starten
    timer_start_timer(LPC_TIM2);
}

/*********************************************************************/
/**
 \brief Diese Funktion ist eine Hilfsfunktion um auf den nächsten Interrupt von Timer2 zu warten.

\return -

\version 12.11.2021

\todo -
\bug  -
**********************************************************************/
static void wait_for_interrupt() {
	// Warten bis timer 2 tickt
    while (!timer_has_timer2_ticked()) {
        __asm("nop");
    }
}

/*********************************************************************/
/**
 \brief Diese Funktion ist eine Hilfsfunktion um den DIO Pin zwischen Ein- und Ausgang zu wechseln.

\param  dir
        Richtung, in die der DIO Pin gesetzt werden soll
        (0 - 1, siehe PinDir)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static void set_dio_pin_dir(enum PinDir dir) {
	// Kopie vom DIO-PIN machen, weil die Konstante nicht verändert werden kann.
	// Alles wird kopiert außer die Richtung
    struct GPIOPin dio = {
            .dir = dir,
            .mode = serial_dio.mode,
            .pin = serial_dio.pin,
            .port = serial_dio.port,
            .open_drain = serial_dio.open_drain,
    };
    // Pin neu initialisieren
    gpio_init_pin(&dio);
}

/*********************************************************************/
/**
 \brief Diese Funktion ist eine Hilfsfunktion um ein Byte an den TM1638 zu senden.

\param  byte
        Byte, was an den TM1638 gesendet werden soll
        (0x00 - 0xFF)

\return -

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static void write_byte(uint8_t byte) {
	// Kommunikationsrichtung einstellen
    set_dio_pin_dir(OUTPUT);

    // In einer for schleife die einzelnen Bits senden
    for (int i = 0; i < 8; ++i) {
    	// Setzen des Datenbits
        gpio_set(&serial_dio, (byte >> i) & 0b1);
        // Ein Takt warten
        wait_for_interrupt();
        // Clock Pin auf 0 ziehen
        gpio_set_low(&serial_clk);
        // Ein Takt warten
        wait_for_interrupt();
        // Clock Pin auf 1 ziehen
        gpio_set_high(&serial_clk);
        // Ein Takt warten
        wait_for_interrupt();
    }
}

/*********************************************************************/
/**
 \brief Diese Funktion ist eine Hilfsfunktion um ein Byte vom TM1638 zu empfangen.

\return Byte, was an den TM1638 gesendet werden soll
        (0x00 - 0xFF)

\version 12.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static uint8_t read_byte() {
	// DIO-PIN auf Input setzen
    set_dio_pin_dir(INPUT);

    // Speicher reservieren
    uint8_t byte = 0;

    // Jedes Bit einzeln in for-Schleife einlesen
    for (int i = 0; i < 8; i++) {
    	// clock auf 0 Ziehen
        gpio_set_low(&serial_clk);
        // Einen Tackt Warten
        wait_for_interrupt();
        // Abfrage ob GPIO-PIN high oder low ist
        uint8_t tmp = gpio_get(&serial_dio);
        // Das Entsprechende Bit an die richtige STelle im Resultatbyte schieben
        // Zuerst das LSB um 0
        // Zuletzt das MSB um 7
        byte |= (tmp << i);
        // Einen Tackt warten
        wait_for_interrupt();
        // Clock auf High stezen
        gpio_set_high(&serial_clk);
        // Einen Tackt warten
        wait_for_interrupt();
    }
    return byte;
}

/*********************************************************************/
/**
 \brief Diese Funktion sendet ein einzelnes Byte and den TM1638.
Dies ist ein Befehl, der zum Beispiel die Helligkeit des 7-Segment-Displays steuert.

\param  command
        Ein Byte, welches einen Befehl an den TM1638 repräsentiert
        (0x00 - 0xFF)

\return -

\version 13.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void serial_write_command(uint8_t command) {
	// Strobeleitung muss zu begin der Übertragung auf 0 gezogen werden
    gpio_set_low(&serial_stb);

    // Zwei Takte abwarten
    wait_for_interrupt();
    wait_for_interrupt();

    // Komandobyte schicken
    write_byte(command);

    // Zwei Takte abwarten
    wait_for_interrupt();
    wait_for_interrupt();

    // Zum Beenden der Übertragung Strobeleitung wieder auf 1 ziehen
    gpio_set_high(&serial_stb);
}

/*********************************************************************/
/**
 \brief Diese Funktion sendet mehrere Bytes and den TM1638.

Das erste Byte wird als Befehl interpretiert und wird daher als separaten Parameter command angegeben.

Die darauf folgenden Bytes werden als Array-Pointer im Parameter data angegeben.
Diese werden vom TM1638 als Parameter zu dem gesendeten Befehl interpretiert.

\param  command
        Ein Byte, welches einen Befehl an den TM1638 repräsentiert

        (0x00 - 0xFF)
\param  data
        Pointer auf ein Byte-Array aus dem die zu sendenden Bytes gelesen werden

        (0x00 - 0xFF für jedes Byte im Array)
\param  length
        Anzahl an Bytes die gesendet werden sollen

        (0 - 4.294.967.295)

\return -

\version 13.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void serial_write(uint8_t command, uint8_t *data, uint32_t length) {
	// Strobeleitung muss zu begin der Übertragung auf 0 gezogen werden
    gpio_set_low(&serial_stb);

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();

    // Komandobyte schicken
    write_byte(command);

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();

    // Daten bytes werden gesendet
    for (int i = 0; i < length; ++i) {
        write_byte(data[i]);
    }

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();

    // Zum Beenden der Übertragung Strobeleitung wieder auf 1 ziehen
    gpio_set_high(&serial_stb);
}

/*********************************************************************/
/**
 \brief Diese Funktion empfängt mehrere Bytes vom TM1638.

Zuerst wird ein Byte gesendet, um dem TM1638 einen Befehl zu geben.
Die Antwort auf diesen Befehl wird dann im Parameter data abgespeichert.

\param  command
        Ein Byte, welches einen Befehl an den TM1638 repräsentiert

        (0x00 - 0xFF)

\param  data
        Pointer auf ein Byte-Array in das die empfangenden Bytes geschrieben werden

        (0x00 - 0xFF für jedes Byte im Array)

\param  length
        Anzahl an Bytes die empfangen werden sollen

        (0 - 4.294.967.295)

\return -

\version 13.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void serial_read(uint8_t command, uint8_t *data, uint32_t length) {
	// Strobeleitung muss zu begin der Übertragung auf 0 gezogen werden
    gpio_set_low(&serial_stb);

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();

    // Komandobyte schicken
    write_byte(command);

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();

    // Antwortbytes lesen
    for (int i = 0; i < length; ++i) {
        data[i] = read_byte();
    }

    // Zwei Tackte warten
    wait_for_interrupt();
    wait_for_interrupt();
    // Strobe wird High gesetzt um Übertragung zu beenden
    gpio_set_high(&serial_stb);
}

/*********************************************************************/
/**
 \brief Diese Funktion de-initialisiert den Timer 2.

\return -

\version 11.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void serial_deinit() {
	// Timer 2 auschalten
    timer_deinit_timer2();
}

/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für den Clock-Pin des Custom Serial Protokoll des TM1638.

\version 11.11.2021
**********************************************************************/
const struct GPIOPin serial_clk = {
        .port = 0,
        .pin = 10,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};

/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für den DIO-Pin des Custom Serial Protokoll des TM1638.

\version 11.11.2021
**********************************************************************/
const struct GPIOPin serial_dio = {
        .port = 0,
        .pin = 11,
        .mode = PULL_DOWN,
        .dir = OUTPUT,
        .open_drain = false,
};

/*********************************************************************/
/**
 \brief Dieses Struct repräsentiert die Konfiguration für den STB-Pin des Custom Serial Protokoll des TM1638.

\version 11.11.2021
**********************************************************************/
const struct GPIOPin serial_stb = {
        .port = 0,
        .pin = 5,
        .mode = NONE,
        .dir = OUTPUT,
        .open_drain = false,
};
