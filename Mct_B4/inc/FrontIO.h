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

#include "types.h"
#include "GPIO.h"

/**
 *  \file     FrontIO.h
*/

void frontio_set_leds(uint8_t leds);

uint8_t frontio_get_buttons(void);

#endif
