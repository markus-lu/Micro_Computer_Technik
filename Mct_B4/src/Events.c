#include "Events.h"

void events_init(struct Event *event_data) {
	// Schleife die Startwerte für die Events Festsetzen
    for (int i = 0; i < EVENT_COUNT; ++i) {
        event_data[i].enabled = false;
        event_data[i].hour = 0;
        event_data[i].minute = 0;
        event_data[i].on_or_off = false;
        event_data[i].weekdays = 0;
    }
}

uint16_t events_get_count(const struct Event *event_data) {
    uint16_t count = 0;
    for (int i = 0; i < EVENT_COUNT; ++i) {
        count += event_data[i].enabled;
    }
    return count;
}
