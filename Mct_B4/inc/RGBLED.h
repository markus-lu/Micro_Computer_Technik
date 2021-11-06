/*
 ===============================================================================
 Name        : RGBLED.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Input Output Frontplatte
 ===============================================================================
 */

struct rgbled {
    void (*init)(void);
    void (*set_green)(void);
    void (*set_red)(void);
};

extern const struct rgbled RGBLED;
