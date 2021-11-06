#include "RTC.h"

void convert_bcd_to_binary(uint8_t *bytes, struct DateTime *dateTime) {
    // TODO: format values in struct BCD code to datetime
}

void convert_binary_to_bcd(struct DateTime *dateTime, uint8_t *out) {
    out[0] = 0; // Register Address
    // TODO: format values in struct BCD code for RTC
}

uint32_t read_temp() {

}

struct DateTime read_time() {
    uint8_t slave_address[] = {0};
    I2C.write(I2C_DS3231_ADDR, slave_address, 1);

    uint8_t bytes[7];
    I2C.read(I2C_DS3231_ADDR, bytes, 7);

    struct DateTime time;
    convert_bcd_to_binary(bytes, &time);

    return time;
}

void write_time(struct DateTime dateTime) {
    uint8_t data[8];

    convert_binary_to_bcd(&dateTime, data);

    I2C.write(I2C_DS3231_ADDR, data, 8);
}

const struct rtc RTC = {
        .read_temp = read_temp,
        .read_time = read_time,
        .write_time = write_time,
};