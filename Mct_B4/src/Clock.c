#include "Clock.h"
#include "LEDKey.h"

void init() {
    LEDKey.init();
    RTC.init();
}

static void draw_time(){
    // TODO: Output time to LEDKey
    // LEDKey.set_display_data();
}

void loop_once(struct State *state) {
    if (state->clock_should_redraw) {
        draw_time();
    }
}

const struct clock Clock = {
        .init = init,
        .loop_once = loop_once,
};