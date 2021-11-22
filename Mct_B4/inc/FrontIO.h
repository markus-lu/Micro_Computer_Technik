/*
 ===============================================================================
 Name        : FrontIO.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Input Output Frontplatte
 ===============================================================================
 */

#ifndef _FRONTIO_H_
#define _FRONTIO_H_

#include "GPIO.h"

struct frontio {
    void (*init)(void);

    uint8_t (*get_buttons)(void);
};

extern const struct frontio FrontIO;

#endif
