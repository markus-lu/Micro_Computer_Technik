#include "Events.h"

static struct Event *event_data[EVENT_COUNT];

void init() {
    memset(event_data, 0, EVENT_COUNT);
}

uint16_t get_programmed_events() {
    uint16_t out = 0;
    for (int i = 0; i < EVENT_COUNT; ++i) {
        if (event_data[i] != NULL) {
            out |= (1 << i);
        }
    }
    return out;
}

void add_event(uint8_t position, struct Event event) {
    struct Event *persisted_event = malloc(sizeof(struct Event));
    memcpy(persisted_event, &event, sizeof(struct Event));
    event_data[position] = persisted_event;
}

void delete_event(uint8_t position) {
    struct Event *event = event_data[position];
    event_data[position] = NULL;
    free(event);
}

struct Event *get_event(uint8_t position) {
    return event_data[position];
}

const struct events Events = {
        .init = init,
        .get_programmed_events = get_programmed_events,
        .add_event = add_event,
        .delete_event = delete_event,
        .get_event = get_event,
};
