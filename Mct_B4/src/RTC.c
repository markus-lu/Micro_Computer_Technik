#include "RTC.h"

// BCD = binary decimal code
static uint8_t decode_bcd(uint8_t byte) {
    uint8_t tens = (byte >> 4) & 0b1111;
    uint8_t ones = byte & 0b1111;
    return (tens * 10) + ones;
}

// BCD = binary decimal code
static uint8_t encode_bcd(uint8_t number) {
    uint8_t tens = number / 10;
    uint8_t ones = number % 10;
    return (tens << 4) | ones;
}

static void decode_time(uint8_t *bytes, struct DateTime *dateTime) {
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

static void init() {
    I2C.init();
}

static void set_register_address(uint8_t address) {
    uint8_t register_address[1];
    register_address[0] = address;
    I2C.write(DS3231_ADDRESS, register_address, 1);
}

/******************************************************************************/
/**
Funktion zum Auslesen der Temperatur

\return  temperature
         Unteres Byte enthält ganzzahlige Gradzahl
         Oberes Byte enthält 2 bits, die die Kommastelle angeben
         (in 0.25 Grad Schritten)
         Bytes sind absichtlich entgegen der logischen Reihenfolge angeordnet,
         damit man weniger Shiften muss und bei einem Cast zu
         einen int8 direkt die ganzzahlige Gradzahl erhält.
         (oberes Byte: 0 - 3 [repräsentiert 0, 0.25, 0.5, 0.75 Grad])
         (unteres Byte: -128 - +128 Grad)

\version 22.11.2021
*******************************************************************************/
static uint16_t read_temp() {
    set_register_address(0x11);

    uint8_t bytes[2];
    I2C.read(DS3231_ADDRESS, bytes, 2);

    uint16_t temperature = bytes[0]<<8;
    temperature |= bytes[1]>>6;
    return temperature;
}

static void read_time(struct DateTime *time) {
    set_register_address(0);

    uint8_t bytes[7];
    I2C.read(DS3231_ADDRESS, bytes, 7);

    decode_time(bytes, time);
}

static void write_time(struct DateTime *time) {
    uint8_t bytes[8];
    bytes[0] = 0; // register address

    encode_time(time, &bytes[1]);

    I2C.write(DS3231_ADDRESS, bytes, 8);
}

static void deinit() {
    I2C.deinit();
}

const struct rtc RTC = {
        .init = init,
        .read_temp = read_temp,
        .read_time = read_time,
        .write_time = write_time,
        .deinit = deinit,
};
