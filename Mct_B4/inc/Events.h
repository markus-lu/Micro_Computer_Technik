/*
 ===============================================================================
 Name        : Events.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Speicher für Events
 ===============================================================================
 */

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#define EVENT_COUNT 16

struct Event {
    bool enabled;
    uint8_t hour;
    uint8_t minute;
    uint8_t weekdays; // MSB = 0, Monday, ..., LSB = Sunday
    bool on_or_off; // 1 = on, 0 = off
};


struct events {
    void (*init)(void);
    uint16_t (*get_programmed_events)(void);
    void (*add_event)(uint8_t position, struct Event event);
    void (*delete_event)(uint8_t position);
    struct Event (*get_event)(uint8_t position);
};

extern const struct events Events;

#endif
