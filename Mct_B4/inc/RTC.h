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

#include "types.h"
#include "I2C.h"

#define MONDAY 1
#define SUNDAY 7


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

void rtc_init(void);

uint16_t rtc_read_temp(void);

void rtc_read_time(struct DateTime *);

void rtc_write_time(struct DateTime *);

#endif
