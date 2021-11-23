#ifndef _STATE_H_
#define _STATE_H_

#include "types.h"
#include "RTC.h"
#include <stdbool.h>

//struct MenuState {
//    const struct MenuScreen *screen;
//    uint8_t last_buttons;
//    uint8_t selected_event;
//    uint8_t selected_event_detail;
//    bool menu_edit_mode;
//    bool menu_should_redraw;
//};


struct State {
    bool blink;
    const struct MenuScreen *screen;
    uint8_t last_buttons;
    uint8_t selected_event;
    uint8_t selected_event_detail;
    bool menu_edit_mode;
    struct Event *event_data;
    bool menu_should_redraw;
    bool rgb_state;
    uint16_t temperature;
    struct DateTime time;
    uint8_t clock_selected_field;
    bool clock_edit_mode;
    bool clock_should_redraw;
};

bool compare_times(struct DateTime *old_time, struct DateTime *new_time);

#endif //_STATE_H_
