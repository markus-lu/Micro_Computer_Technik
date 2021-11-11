#include "RTC.h"

static void decode_time(uint8_t *bytes, struct DateTime *dateTime) {
    // TODO: format values in struct BCD code to datetime
}

static void encode_time(struct DateTime *dateTime, uint8_t *out) {
    out[0] = 0; // Register Address
    // TODO: format values in struct BCD code for RTC
}

static void init() {

}

static uint32_t read_temp() {
    // TODO: Read Temperature and convert it to readable value
    return 0;
}

static void read_time(struct DateTime* time) {
    uint8_t slave_address[1] = {0};
    I2C.write(I2C.DS3231_ADDRESS, slave_address, 1);

    uint8_t bytes[7];
    I2C.read(I2C.DS3231_ADDRESS, bytes, 7);

    decode_time(bytes, time);
}

static void write_time(struct DateTime *time) {
    uint8_t bytes[8];

    encode_time(time, bytes);

    I2C.write(I2C.DS3231_ADDRESS, bytes, 8);
}

const struct rtc RTC = {
        .init = init,
        .read_temp = read_temp,
        .read_time = read_time,
        .write_time = write_time,
};
