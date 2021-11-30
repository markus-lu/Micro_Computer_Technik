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
	// Zähler erstellen
    uint16_t count = 0;
    // Alle abgespeicherten Events zählen
    for (int i = 0; i < EVENT_COUNT; ++i) {
    	// enabled = bool
    	// also +1 bei aktiv
    	// +0 bei nicht aktiv
        count += event_data[i].enabled;
    }
    // Rückgabe des Eventanzahl
    return count;
}
