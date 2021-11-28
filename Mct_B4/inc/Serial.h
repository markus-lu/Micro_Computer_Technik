/*
 ===============================================================================
 Name        : Serial.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Custom Serial Protokoll Treiber für TM1638
 ===============================================================================
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "types.h"
#include "GPIO.h"

extern const struct GPIOPin serial_clk;
extern const struct GPIOPin serial_dio;
extern const struct GPIOPin serial_stb;

void serial_init(void);

void serial_write_command(uint8_t command);

void serial_write(uint8_t command, uint8_t *data, uint32_t length);

void serial_read(uint8_t command, uint8_t *data, uint32_t length);

void serial_deinit(void);

#endif
