#include "RTC.h"

void decode_time(uint8_t *bytes, struct DateTime *dateTime) {
    // TODO: format values in struct BCD code to datetime
}

void encode_time(struct DateTime *dateTime, uint8_t *out) {
    out[0] = 0; // Register Address
    // TODO: format values in struct BCD code for RTC
}

uint32_t read_temp() {
    // TODO: Read Temperature and convert it to readable value
    return 0;
}

struct DateTime read_time() {
    uint8_t slave_address[1] = {0};
    I2C.write(I2C.DS3231_ADDRESS, slave_address, 1);

    uint8_t bytes[7];
    I2C.read(I2C.DS3231_ADDRESS, bytes, 7);

    struct DateTime time;
    decode_time(bytes, &time);

    return time;
}

void write_time(struct DateTime dateTime) {
    uint8_t bytes[8];

    encode_time(&dateTime, bytes);

    I2C.write(I2C.DS3231_ADDRESS, bytes, 8);
}

const struct rtc RTC = {
        .read_temp = read_temp,
        .read_time = read_time,
        .write_time = write_time,
};