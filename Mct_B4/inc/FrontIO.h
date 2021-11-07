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
    const struct GPIOPin interrupt;
    void (*init)(void);
    void (*handle_interrupt)(void);
};

extern const struct frontio FrontIO;

#endif
