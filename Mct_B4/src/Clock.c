#include "Clock.h"
#include "RTC.h"
#include "LEDKey.h"

void init(struct ClockState* state) {
    LEDKey.init();
    RTC.init();
    RTC.read_time(&state->time);
    state->sel_field = 0;
    state->edit_mode = false;
}

void loop_once(struct ClockState* state) {

}

const struct clock Clock = {
        .init = init,
        .loop_once = loop_once,
};