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


#define EVENT_COUNT 16

#define Monday (1 << 6)
#define Tuesday (1 << 5)
#define Wednesday (1 << 4)
#define Thursday (1 << 3)
#define Friday (1 << 2)
#define Saturday (1 << 1)
#define Sunday (1 << 0)

struct Event {
    bool enabled;
    uint8_t hour;
    uint8_t minute;
    uint8_t weekdays; // MSB = 0, Monday, ..., LSB = Sunday
    bool on_or_off; // 1 = on, 0 = off
};


struct events {
    void (*init)(struct Event *event_data);

    uint16_t (*get_count)(const struct Event *event_data);
};

extern const struct events Events;

#endif
