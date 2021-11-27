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

#include "types.h"
#include "i2c_1769.h"

// Portexpander für 16 rote LEDs
#define PCA9539_ADDRESS 0xE8
// Portexpander Frontplatte
#define PCF8574A_ADDRESS 0x7E
// Portexpander für das LCD
#define PCF8574_LCD_ADDRESS 0x4E
// RTC Modul
#define DS3231_ADDRESS 0xD0

uint8_t i2c_write(uint8_t address, uint8_t *data, uint32_t length);

uint8_t i2c_read(uint8_t address, uint8_t *data, uint32_t length);

#endif
