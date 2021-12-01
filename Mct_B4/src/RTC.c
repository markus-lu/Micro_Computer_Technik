#include "RTC.h"
#include "I2C.h"

/**
 *  \file     RTC.c
*/

/*********************************************************************/
/**
Diese Funktion wandelt die vom RTC kommende Datenstruktur für die Uhrzeit
in ein für den Menschen lesbares Format um.

\param  byte
		Integer der die vom RTC ausgelesene Uhrzeit im BD-Code enthält

\return  Integer der auch die Uhrzeit aber für Menschen lesbar enthält

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static uint8_t decode_bcd(uint8_t byte) {
	// BCD = binary decimal code
	// 10er sind im höherem Nibble
    uint8_t tens = (byte >> 4) & 0b1111;
    // 1er sind im unterem Nibble
    uint8_t ones = byte & 0b1111;
    // Zehener mit 10 multipliziern und Einer addieren
    return (tens * 10) + ones;
}

/*********************************************************************/
/**
Diese Funktion wandelt die für Menschen lesbare Uhrzeit in BDC um,
damit der RTC beschieben werden kann.

\param  number
		Integer der die für Menschen lesbare Uhrzeit enthält

\return  Integer der die Tempeartur in einem für die RTC lesbarem Format enthält

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static uint8_t encode_bcd(uint8_t number) {
	// BCD = binary decimal code
	// Geteilt durch 10 um die Zehner zu bekommen
    uint8_t tens = number / 10;
    // Modulo 10 um die Einer zu bekommen
    uint8_t ones = number % 10;
    // Zehner im ersten Nibble und Einer im Zweiten Nibble zurückgeben
    return (tens << 4) | ones;
}

/*********************************************************************/
/**
Diese Funktion wandelt die vom RTC kommende Datenstruktuen für die Uhrzeit
in ein für den Menschen lesbares Format um und speichert Diese ab. Die eigentliche
Decodierung ist in eine externe Methode ausgelagert.

\param  bytes
		Pointer zu einem Integer Array welches die ausgelesenden Daten von der RTC enthält
		(Wertebereich siehe Datenblatt vom DS3231)

\param	dateTime
		Pointer zu einem DateTime Struct, welches die Uhrzeit und Datum für Menschen
		lesbar enthält
		(Wertebereich in der Struct-Defenition)

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static void decode_time(uint8_t *bytes, struct DateTime *dateTime) {
	// Struct mit werten aus demArray befüllen
    dateTime->seconds = decode_bcd(bytes[0]);
    dateTime->minutes = decode_bcd(bytes[1]);
    // und Verknüpfung filtert 24 stunden bit heraus
    dateTime->hours = decode_bcd(bytes[2] & 0b111111);
    dateTime->weekday = decode_bcd(bytes[3]);
    dateTime->day = decode_bcd(bytes[4]);
    // und Verknüpfung filtert century bit heraus
    dateTime->month = decode_bcd((bytes[5]) & 0b11111);
    dateTime->century = bytes[5] >> 7;
    dateTime->year = decode_bcd(bytes[6]);
}


/*********************************************************************/
/**
Diese Funktion wandelt die für den Menschen lesbare Datenstruktuen für die Uhrzeit
in ein für den RTC lesbares Format um und speichert Diese ab. Die eigentliche
Decodierung ist in eine externe Methode ausgelagert.

\param  bytes
		Pointer zu einem Integer Array welches die umgewandelten Daten für den RTC enthält
		(Wertebereich siehe Datenblatt vom DS3231)

\param	dateTime
		Pointer zu einem DateTime Struct, welches die Uhrzeit und Datum für Menschen
		lesbar enthält
		(Wertebereich in der Struct-Defenition)

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static void encode_time(struct DateTime *dateTime, uint8_t *bytes) {
	// Speicherung der BDC Codierten Werte für Datum und Uhrzeit
    bytes[0] = encode_bcd(dateTime->seconds);
    bytes[1] = encode_bcd(dateTime->minutes);
    bytes[2] = encode_bcd(dateTime->hours);
    bytes[3] = encode_bcd(dateTime->weekday);
    bytes[4] = encode_bcd(dateTime->day);
    bytes[5] = encode_bcd(dateTime->month);
    // Jahhundert Bit an die entsprechende Stelle schieben
    bytes[5] |= dateTime->century << 7;
    bytes[6] = encode_bcd(dateTime->year);
}


/*********************************************************************/
/**
Diese Funktion initialisiert den I²C Portexpander für die RTC

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void rtc_init() {
    i2c_init(I2C_SPEED);
}


/*********************************************************************/
/**
Diese Funktion setzt die Adresse für den I²C Portexpander und schreibt sie.

\param	address
		Integer der die zu schreibende Adresse enthält

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
static void set_register_address(uint8_t address) {
	// Array erstellen
    uint8_t register_address[1];
    // Registeradresse ins Array schreiben
    register_address[0] = address;
    // Registeradresse an Chip senden
    i2c_write(DS3231_ADDRESS, register_address, 1);
}


/*********************************************************************/
/**
Diese Funktion liest die aktuelle Tempeartur vom RTC.

\return	temperature
   	   	Integer der die gelesende Tempeartur enthält

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint16_t rtc_read_temp() {
	// Register Adresse setzen
	// Es soll aus Regeister 0x11 und 0x12 die Temp gelesen werden
    set_register_address(0x11);

    // Zwei Bytes zum Lesen der Temp reservieren
    uint8_t bytes[2];
    // Temp auslesen
    i2c_read(DS3231_ADDRESS, bytes, 2);

    // Vorkommastellen der Temp sind in Byte 0
    // Nachkommastellen der Temp sind in Byte 1
    // Beide Stellen werden aneinandergehängt
    uint16_t temperature = bytes[0] << 8;
    temperature |= bytes[1] >> 6;
    // Rückgabe der Temp
    return temperature;
}

/*********************************************************************/
/**
Diese Funktion liest die aktuelle Zeit vom RTC.

\param	time
		Pointer zu einem DateTime Struct, welches die Uhrzeit und Datum für Menschen
		lesbar enthält
		(Wertebereich in der Struct-Defenition)

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void rtc_read_time(struct DateTime *time) {
	// Register Adresse Setzen
	// Zeit und Datum könne aus den ersten sieben Bytes ausgelesen werden
    set_register_address(0);

    // Speicherplatz für Zeit Reserviern
    uint8_t bytes[7];

    // Datum und Uhrzeit lesen
    i2c_read(DS3231_ADDRESS, bytes, 7);

    // Datum und Uhrzeit in das Übergebene Struct decodieren
    decode_time(bytes, time);
}


/*********************************************************************/
/**
Diese Funktion schreibt eine aktualisierte Zeit in den RTC.

\param	time
		Pointer zu einem DateTime Struct, welches die Uhrzeit und Datum für Menschen
		lesbar enthällt
		(Wertebereich in der Struct-Defenition)

\return  -

\version 30.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void rtc_write_time(struct DateTime *time) {
	// Speicherreservierung
    uint8_t bytes[8];
    // Registeradresse ab der geschrieben werden soll festslegen
    // Register 0
    bytes[0] = 0; // register address

    // Zeit ins Binary Decimal Format umwandeln
    encode_time(time, &bytes[1]);

    // Schreiben auf RTC-Chip (DS3231)
    i2c_write(DS3231_ADDRESS, bytes, 8);
}
