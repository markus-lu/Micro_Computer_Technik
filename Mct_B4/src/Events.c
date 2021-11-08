#include "Events.h"

static struct Event event_data[EVENT_COUNT];

static void init() {
    memset(event_data, 0, sizeof(struct Event) * EVENT_COUNT);
}

static uint16_t get_programmed_events() {
    uint16_t count = 0;
    for (int i = 0; i < EVENT_COUNT; ++i) {
        count += event_data[i].enabled;
    }
    return count;
}

static void add_event(uint8_t position, struct Event event) {
    memcpy(&event_data[position], &event, sizeof(struct Event));
}

static void delete_event(uint8_t position) {
    event_data[position].enabled = false;
}

struct Event get_event(uint8_t position) {
    return event_data[position];
}

const struct events Events = {
        .init = init,
        .get_programmed_events = get_programmed_events,
        .add_event = add_event,
        .delete_event = delete_event,
        .get_event = get_event,
};
