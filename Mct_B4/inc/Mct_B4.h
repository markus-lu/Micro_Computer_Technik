/*
 ===============================================================================
 Name        : Mct_B4.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Schaltuhr und Thermometer
 ===============================================================================
 */

#ifndef _MCT_B4_H_
#define _MCT_B4_H_


#include "mclite/i2c_1769.h"
#include "I2C.h"
#include "Serial.h"
#include "Menu.h"
#include "Clock.h"
#include "Events.h"
#include "FrontIO.h"

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#include <stdio.h>

int main();

#endif
