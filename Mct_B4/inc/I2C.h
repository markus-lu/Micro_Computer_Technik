/*
 ===============================================================================
 Name        : I2C.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : I2C Treiber
 ===============================================================================
 */

#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>
#include "i2c_1769.h"

struct i2c {
    const uint8_t PCA9539_ADDRESS;     // Portexpander für 16 rote LEDs
    const uint8_t PCF8574A_ADDRESS;    // Portexpander Frontplatte
    const uint8_t PCF8574_LCD_ADDRESS; // Portexpander für das LCD
    const uint8_t DS3231_ADDRESS;      // RTC Modul
    void (*init)(void);
    uint8_t (*write)(uint8_t address, uint8_t *data, uint32_t length);
    uint8_t (*read)(uint8_t address, uint8_t *data, uint32_t length);
    void (*deinit)(void);
};

extern const struct i2c I2C;

#endif
