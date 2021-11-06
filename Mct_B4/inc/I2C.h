/*
 ===============================================================================
 Name        : I2C.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : I2C Treiber
 ===============================================================================
 */

#include <stdint.h>
#include "mclite/i2c_1769.h"

#define I2C_PCA9539_ADDR     0xE8  // Portexpander für 16 rote LEDs
#define I2C_LM75_ADDR        0x90  // Temperatursensor
#define I2C_PCF8574A_ADDR    0x7E  // Portexpander Frontplatte
#define I2C_PCF8574_LCD_ADDR 0x4E  // Portexpander für das LCD
#define I2C_PCF8574_MX_ADDR  0x40  // Portexpander für die Matrix-Tastatur
#define I2C_DS3231_ADDR  0x68      // RTC

struct i2c {
    void (*init)(void);
    void (*write)(uint8_t address, uint8_t *data, uint32_t length);
    void (*read)(uint8_t address, uint8_t *data, uint32_t length);
    void (*deinit)(void);
};

extern const struct i2c I2C;
