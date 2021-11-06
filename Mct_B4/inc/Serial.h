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

struct serial {
    void (*init)(void);
    void (*write)(uint8_t address, uint8_t *data, uint32_t length);
    void (*read)(uint8_t address, uint8_t *data, uint32_t length);
    void (*deinit)(void);
};

extern const struct serial Serial;
