/*
 ===============================================================================
 Name        : RTC.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Datum Uhrzeit und Temperatur
 ===============================================================================
 */

#ifndef _RTC_H_
#define _RTC_H_


#include "I2C.h"
#include <types.h>

struct DateTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t weekday;
    uint8_t day;
    uint8_t month;
    uint8_t century;
    uint8_t year;
};

struct rtc {
    void (*init)(void);

    uint32_t (*read_temp)(void);

    void (*read_time)(struct DateTime *);

    void (*write_time)(struct DateTime *);
};

extern const struct rtc RTC;

#endif
