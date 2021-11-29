#include "RTC.h"
#include "I2C.h"

// BCD = binary decimal code
static uint8_t decode_bcd(uint8_t byte) {
	// 10er sind im höherem Nibble
    uint8_t tens = (byte >> 4) & 0b1111;
    // 1er sind im unterem Nibble
    uint8_t ones = byte & 0b1111;
    // Zehener mit 10 multipliziern und Einer addieren
    return (tens * 10) + ones;
}

// BCD = binary decimal code
static uint8_t encode_bcd(uint8_t number) {
    uint8_t tens = number / 10;
    uint8_t ones = number % 10;
    return (tens << 4) | ones;
}

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

static void encode_time(struct DateTime *dateTime, uint8_t *bytes) {
    bytes[0] = encode_bcd(dateTime->seconds);
    bytes[1] = encode_bcd(dateTime->minutes);
    bytes[2] = encode_bcd(dateTime->hours);
    bytes[3] = encode_bcd(dateTime->weekday);
    bytes[4] = encode_bcd(dateTime->day);
    bytes[5] = encode_bcd(dateTime->month);
    bytes[5] |= dateTime->century << 7;
    bytes[6] = encode_bcd(dateTime->year);
}

void rtc_init() {
    i2c_init(I2C_SPEED);
}

static void set_register_address(uint8_t address) {
	// Array erstellen
    uint8_t register_address[1];
    // Registeradresse ins Array schreiben
    register_address[0] = address;
    // Registeradresse an Chip senden
    i2c_write(DS3231_ADDRESS, register_address, 1);
}

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

void rtc_write_time(struct DateTime *time) {
    uint8_t bytes[8];
    bytes[0] = 0; // register address

    encode_time(time, &bytes[1]);

    i2c_write(DS3231_ADDRESS, bytes, 8);
}
